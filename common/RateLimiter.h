// Простой ограничитель частоты действий (например, попыток входа)
// Хранит для каждого ключа (IP, имя пользователя) количество запросов за временное окно

#ifndef RATELIMITER_H
#define RATELIMITER_H

#include <QString>
#include <QMap>
#include <QMutex>
#include <chrono>

class RateLimiter
{
public:
	// maxRequests - максимальное число запросов за window
	RateLimiter(int maxRequests, std::chrono::seconds window);

	// Проверить, разрешено ли действие для данного ключа
	// Возвращает true, если лимит не превышен, и учитывает этот запрос
	bool isAllowed(const QString& key);

	// Удалить устаревшие записи (можно вызывать периодически)
	void cleanExpired();

private:
	struct Record
	{
		int count;					// число запросов в текущем окне
		std::chrono::steady_clock::time_point start;	// начало окна
	};

	QMap<QString, Record> records_;
	QMutex mutex_;
	int maxRequests_;
	std::chrono::seconds window_;
};

#endif
