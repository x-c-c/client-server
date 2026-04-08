// Реализация методов упаковки/распаковки пакетов

#include "PacketHandler.h"
#include <QDataStream>

QByteArray PacketHandler::buildPacket(Command command, const QByteArray& payload)
{
	QByteArray packet;
	QDataStream stream(&packet, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);		// Фиксируем версию сериализации Qt

	// Общий размер пакета = заголовок (4+2+2+4 = 12 байт) + размер payload
	quint32 totalSize = sizeof(quint32) + sizeof(quint16) + sizeof(quint16)	+ sizeof(quint32) + payload.size();

	// Записываем заголовок
	stream << totalSize;			 // 4 байта: общий размер
	stream << static_cast<quint16>(command); // 2 байта: код команды
	stream << ProtocolVersion;		 // 2 байта: версия протокола
	stream << quint32(0);			 // 4 байта: место для контрольной суммы (пока 0)

	// Пишем полезную нагрузку
	stream.writeRawData(payload.constData(), payload.size());

	// Вычисляем контрольную сумму по всему пакету, кроме поля checksum
	// (заголовок 8 байт + payload)
	QByteArray dataForChecksum = packet.left(8) + packet.mid(12);
	quint32 checksum = calculateChecksum(dataForChecksum);

	// Вставляем вычисленную сумму на место (смещение 8 байт от начала)
	QDataStream patch(&packet, QIODevice::ReadWrite);
	patch.skipRawData(sizeof(quint32) + sizeof(quint16) + sizeof(quint16));
	patch << checksum;

	return packet;
}

bool PacketHandler::parsePacket(const QByteArray& rawData, Command& outCommand, QByteArray& outPayload)
{
	// Минимальный размер пакета - 12 байт (заголовок без payload)
	if (rawData.size() < 12)
	{
		return false;
	}

	QDataStream stream(rawData);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);

	quint32 totalSize;
	stream >> totalSize;
	// Проверяем, что размер в заголовке совпадает с реальным и не превышает лимит
	if (totalSize > MaxPacketSize || static_cast<quint32>(rawData.size()) != totalSize)
	{
		return false;
	}

	quint16 rawCommand;
	stream >> rawCommand;
	outCommand = static_cast<Command>(rawCommand);

	quint16 version;
	stream >> version;
	if (version != ProtocolVersion)   // Несовпадение версии - отклоняем пакет
	{
		return false;
	}

	quint32 receivedChecksum;
	stream >> receivedChecksum;

	// Извлекаем payload
	outPayload = rawData.mid(12);

	// Проверяем контрольную сумму
	QByteArray dataForChecksum = rawData.left(8) + rawData.mid(12);
	quint32 computedChecksum = calculateChecksum(dataForChecksum);
	return computedChecksum == receivedChecksum;
}

quint32 PacketHandler::calculateChecksum(const QByteArray& data)
{
	// Используем встроенную в Qt функцию вычисления контрольной суммы
	return qChecksum(data.constData(), data.size());
}





