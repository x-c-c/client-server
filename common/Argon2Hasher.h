// Безопасное хеширование паролей с использованием алгоритма Argon2id

#ifndef ARGON2HASHER_H
#define ARGON2HASHER_H

#include <QByteArray>

class Argon2Hasher
{
public:
	// Возвращает строку вида "base64(salt):base64(hash)"
	static QByteArray hashPassword(const QString& password);

	// Проверяет пароль по сохранённому хешу
	static bool verifyPassword(const QString& password, const QByteArray& storedHash);
};

#endif
