#include "Cache.hpp"

namespace
{
	const int REDIS_CACHE_MAX_MB = 8;
} // namespace

namespace HTTP
{
	Cache::Cache()
	    : Cache(REDIS_CACHE_MAX_MB)
	{
	}

	Cache::Cache(const int cache_capacity)
	    : m_cache{new sw::redis::Redis("tcp://127.0.0.1:6379")}
	    , m_cache_capacity{cache_capacity}
	{
		// set LRU mode of redis instance
		m_cache->command("config", "set", "maxmemory",
		                 std::to_string(m_cache_capacity) + "mb");
		m_cache->command("config", "set", "maxmemory-policy", "allkeys-lru");
	}

	std::string Cache::get(const std::string& uri)
	{
		auto result = m_cache->get(uri);

		if (!result)
		{
			return "";
		}

		return *result;
	}

	bool Cache::insert(const std::string& uri, const std::string& resource)
	{
		return m_cache->set(uri, resource);
	}

	bool Cache::erase(const std::string& uri) { return m_cache->del(uri) == 1; }
} // namespace HTTP