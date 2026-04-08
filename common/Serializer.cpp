#include "Serializer.h"
#include <QJsonDocument>
#include <QJsonObject>

QByteArray Serializer::serialize(Command command, const QVariantMap& data)
{
	// Создаём обёртку {"cmd": <число>, "data": { ... }}
	QVariantMap wrapper;
	wrapper["cmd"] = static_cast<uint16_t>(command);
	wrapper["data"] = data;

	QJsonObject obj = QJsonObject::fromVariantMap(wrapper);
	QJsonDocument doc(obj);
	// Возвращаем компактный JSON без лишних пробелов
	return doc.toJson(QJsonDocument::Compact);
}

bool Serializer::deserialize(const QByteArray& rawData,	Command& outCommand, QVariantMap& outData)
{
	QJsonDocument doc = QJsonDocument::fromJson(rawData);
	if (!doc.isObject())
	{
		return false;
	}

	QVariantMap wrapper = doc.object().toVariantMap();
	if (!wrapper.contains("cmd"))
	{
		return false;
	}

	outCommand = static_cast<Command>(wrapper["cmd"].toUInt());
	outData = wrapper["data"].toMap();
	return true;
}



