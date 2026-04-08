// Отвечает за упаковку данных в бинарный пакет с заголовком и контрольной суммой, а также за обратный разбор принятых данных

#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <QByteArray>
#include "Protocol.h"

class PacketHandler
{
public:
	// Собрать пакет: заголовок (размер, команда, версия, checksum) + payload
	static QByteArray buildPacket(Command command, const QByteArray& payload);

	// Разобрать сырые данные: извлечь команду и полезную нагрузку, проверить целостность
	static bool parsePacket(const QByteArray& rawData, Command& outCommand, QByteArray& outPayload);

	// Версия протокола (должна совпадать у клиента и сервера)
	static constexpr quint16 ProtocolVersion = 1;

	// Максимальный допустимый размер пакета (защита от атак на переполнение буфера)
	static constexpr quint32 MaxPacketSize = 10 * 1024 * 1024; // 10 MB

private:
	// Вычисление контрольной суммы (используется Qt-функция qChecksum)
	static quint32 calculateChecksum(const QByteArray& data);
};

#endif
