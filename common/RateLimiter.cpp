#include "RateLimiter.h"

RateLimiter::RateLimiter(int maxRequests, std::chrono::seconds window) : maxRequests_(maxRequests), window_(window){}

bool RateLimiter::isAllowed(const QString& key)
{
	QMutexLocker locker(&mutex_);
	auto now = std::chrono::steady_clock::now();
	auto it = records_.find(key);

	if (it == records_.end())
	{
		// Первый запрос от этого ключа
		records_[key] = {1, now};
		return true;
	}

	Record& rec = it.value();
	if (now - rec.start > window_)
	{
		// Окно истекло, начинаем новое
		rec.count = 1;
		rec.start = now;
		return true;
	}

	if (rec.count < maxRequests_)
	{
		++rec.count;
		return true;
	}

	// Лимит превышен
	return false;
}

void RateLimiter::cleanExpired()
{
	QMutexLocker locker(&mutex_);
	auto now = std::chrono::steady_clock::now();
	auto it = records_.begin();
	while (it != records_.end())
	{
		// Удаляем записи, старше двух окон (точно уже не нужны)
		if (now - it->start > window_ * 2)
			{
				it = records_.erase(it);
			}
		else
			{
				++it;
			}
	}
}



