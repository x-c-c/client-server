// Проверка корректности имени пользователя, пароля, сообщений

#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <QString>

class InputValidator
{
public:
	// Имя пользователя: допустимые символы и длина
	static bool validateUsername(const QString& username);

	// Пароль: минимальная длина (можно расширить требования)
	static bool validatePassword(const QString& password);

	// Сообщение: не пустое и не превышает максимальную длину
	static bool validateMessage(const QString& message);
};

#endif
