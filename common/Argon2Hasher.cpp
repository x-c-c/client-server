#include "Argon2Hasher.h"
#include "argon2.h"
#include <QRandomGenerator>
// Параметры хеширования: длина хеша 32 байта, соль 16 байт
static constexpr int HASH_LEN = 32;
static constexpr int SALT_LEB = 16;

QByteArray Argon2Hasher::hashPassword(const QString& password)
{
	// Генерируем случайную соль
	QByteArray salt(SALT_LEN, '\0');
	for (int i = 0; i < SALT_LEN; ++i)
	{
		salt[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
	}
	
	QByteArray hash(HASH_LEN, '\0');
	
	// argon2id_hash_raw: время = 2, память = 19 МБ, потоки = 1
	int t_cost = 2;			//количество проходов
	int m_cost = 19 * 1024*1024;	//использование памяти в MB
	int parallelism = 1;		//степень параллелизма
	argon2id_hash_raw(	t_cost, 
				m_cost, 
				parallelism, 
				password.toUtf8().constData(), 
				password.length(),
       				salt.constData(), 
       		 		salt.length(),
        			hash.data(), 
        			hash.length()
        		);
        // Возвращаем в формате base64(salt):base64(hash)
        return salt.toBase64() + ":" + hash.toBase64();
}

bool Argon2Hasher::verifyPassword(const QString& password, const QByteArray& storedHash)
{
	QList<QByteArray> parts = storedHash.split(':');
	if (parts.size() != 2)
	{
		return false;
	}
	
	QByteArray salt = QByteArray::fromBase64(parts[0]);
	QByteArray expectedHash = QByteArray::fromBase64(parts[1]);
	QByteArray computedHash(HASH_LEN, '\0');
	
	int t_cost = 2;			//количество проходов
	int m_cost = 19 * 1024*1024;	//использование памяти в MB
	int parallelism = 1;		//степень параллелизма
	argon2id_hash_raw(	t_cost, 
				m_cost, 
				parallelism, 
				password.toUtf8().constData(), 
				password.length(),
       				salt.constData(), 
       		 		salt.length(),
        			computedHash.data(), 
        			computedHash.length()
        		);
        // Сравниваем побайтово
        return computedHash == expectedHash;
}





