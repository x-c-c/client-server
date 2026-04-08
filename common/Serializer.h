// Преобразует QVariantMap (ассоциативный массив) в JSON-строку и обратно
// Упаковывает команду и данные в единую JSON-структуру

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QByteArray>
#include <QVariantMap>
#include "Protocol.h"

class Serializer
{
public:
	// Упаковать команду и данные в JSON (возвращает QByteArray с JSON-текстом)
	static QByteArray serialize(Command command, const QVariantMap& data);

	// Распаковать JSON, извлечь команду и данные
	static bool deserialize(const QByteArray& rawData, Command& outCommand, QVariantMap& outData);
};

#endif
