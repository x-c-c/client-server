// test.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "serverstartstop.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <chrono>

struct ServerFixture {
    ServerStartStop server;
    bool start_and_check(int port = 18080) {
        return server.start(port);
    }
    ~ServerFixture() {
        server.stop();
    }
};

// Успешные запуски
TEST_CASE_FIXTURE(ServerFixture, "Start on default port succeeds") {
    CHECK(start_and_check());
    CHECK(server.serverFileDescriptor > 0);
}

TEST_CASE_FIXTURE(ServerFixture, "Start on specific port") {
    CHECK(start_and_check(19000));
    CHECK(server.serverFileDescriptor > 0);
}

TEST_CASE_FIXTURE(ServerFixture, "Start on port 0 (auto-assign)") {
    CHECK(start_and_check(0));
    CHECK(server.serverFileDescriptor > 0);
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    CHECK(getsockname(server.serverFileDescriptor, reinterpret_cast<sockaddr*>(&addr), &len) == 0);
    CHECK(ntohs(addr.sin_port) > 0);
}

TEST_CASE_FIXTURE(ServerFixture, "Start on maximum valid port") {
    CHECK(start_and_check(65535));
}

// Перезапуск
TEST_CASE_FIXTURE(ServerFixture, "Restart after stop works") {
    CHECK(start_and_check());
    server.stop();
    CHECK(server.serverFileDescriptor == -1);
    CHECK(start_and_check());
}

TEST_CASE_FIXTURE(ServerFixture, "Start while already running on same port fails") {
    REQUIRE(start_and_check(18090));
    ServerStartStop second;
    CHECK_FALSE(second.start(18090));   // порт занят
    CHECK(server.serverFileDescriptor > 0);
    server.stop();
    // теперь второй может занять порт
    CHECK(second.start(18090));
    second.stop();
}

TEST_CASE_FIXTURE(ServerFixture, "Stop before start sets fd to -1") {
    server.stop();
    CHECK(server.serverFileDescriptor == -1);
}

TEST_CASE_FIXTURE(ServerFixture, "Double stop does not crash") {
    REQUIRE(start_and_check());
    server.stop();
    CHECK_NOTHROW(server.stop());
    CHECK(server.serverFileDescriptor == -1);
}

// Здесь исправленный тест: не требуем неравенства fd
TEST_CASE_FIXTURE(ServerFixture, "Start after stop gives valid fd") {
    REQUIRE(start_and_check());
    int firstFd = server.serverFileDescriptor;
    CHECK(firstFd > 0);
    server.stop();
    REQUIRE(start_and_check());
    int secondFd = server.serverFileDescriptor;
    CHECK(secondFd > 0);
    // fd может совпасть — не проверяем
}

// Ошибки параметров
TEST_CASE("Bind fails if port is already in use") {
    int port = 18081;
    ServerStartStop first;
    REQUIRE(first.start(port));
    ServerStartStop second;
    CHECK_FALSE(second.start(port));
    first.stop();
}

TEST_CASE("Start fails with invalid domain") {
    ServerStartStop s(AF_UNSPEC, SOCK_STREAM, 0);
    CHECK_FALSE(s.start(18083));
}

TEST_CASE("Start fails with invalid socket type") {
    ServerStartStop s(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
    CHECK_FALSE(s.start(18084));
}

TEST_CASE("Start fails with invalid protocol") {
    ServerStartStop s(AF_INET, SOCK_STREAM, 999);
    CHECK_FALSE(s.start(18085));
}

TEST_CASE_FIXTURE(ServerFixture, "Start on privileged port fails (if not root)") {
    WARN("This test may fail if run as root");
    CHECK_FALSE(start_and_check(80));
}

// Интеграционные тесты
TEST_CASE("Client can connect to started server") {
    int port = 18092;
    ServerStartStop server;
    REQUIRE(server.start(port));

    int clientFd = socket(AF_INET, SOCK_STREAM, 0);
    REQUIRE(clientFd >= 0);

    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);

    bool connected = (connect(clientFd, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr)) == 0);
    CHECK(connected);

    close(clientFd);
    server.stop();
}

TEST_CASE("Accept returns valid client fd") {
    int port = 18093;
    ServerStartStop server;
    REQUIRE(server.start(port));

    std::thread clientThread([port]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        if (fd >= 0) {
            connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
            close(fd);
        }
    });

    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(server.serverFileDescriptor,
                          reinterpret_cast<sockaddr*>(&clientAddr),
                          &clientLen);
    CHECK(clientFd > 0);
    if (clientFd > 0) close(clientFd);

    clientThread.join();
    server.stop();
}

TEST_CASE("Echo communication with client") {
    int port = 18094;
    ServerStartStop server;
    REQUIRE(server.start(port));

    std::thread client([port]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        if (fd >= 0) {
            connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
            const char* msg = "Hello, Server!";
            send(fd, msg, strlen(msg), 0);
            char buf[1024];
            ssize_t n = recv(fd, buf, sizeof(buf)-1, 0);
            if (n > 0) buf[n] = '\0';
            close(fd);
        }
    });

    int clientFd = accept(server.serverFileDescriptor, nullptr, nullptr);
    REQUIRE(clientFd > 0);

    char buffer[1024];
    ssize_t bytes = recv(clientFd, buffer, sizeof(buffer)-1, 0);
    CHECK(bytes > 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        CHECK(std::string(buffer) == "Hello, Server!");
        const char* reply = "ACK";
        send(clientFd, reply, strlen(reply), 0);
    }
    close(clientFd);
    client.join();
    server.stop();
}
