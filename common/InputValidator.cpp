#include "InputValidator.h"
#include <QRegularExpression>

bool InputValidator::validateUsername(const QString& username)
{
	const int maxLen = 32;	// потом можно вынести в конфиг
	if (username.isEmpty() || username.length() > maxLen)
	{
		return false;
	}

	// Разрешены латинские буквы, цифры, точка, подчёркивание, дефис
	static QRegularExpression regularExpression("^[a-zA-Z0-9_.-]+$");
	return regularExpression.match(username).hasMatch();
}

bool InputValidator::validatePassword(const QString& password)
{
	const int minLen = 6;	// потом можно вынести в конфиг
	return password.length() >= minLen;
}

bool InputValidator::validateMessage(const QString& message)
{
	// ограничение на длину одного сообщения
	const int maxLen = 4096;	// потом можно вынести в конфиг
	return !message.isEmpty() && message.length() <= maxLen;
}
