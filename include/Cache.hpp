#pragma once

#include <sw/redis++/redis++.h>

#include <string>
#include <memory>

using namespace sw::redis;

namespace HTTP
{
    /**
     * @brief LRU for recently visited resource.
     * 
     * Resource (mainly GET method)
     *      1. Queries into databse;
     *      2. Files (e.g. HTML, CSS, Javascript files);
     * 
     * @see https://redis.io/topics/lru-cache
     */
    class Cache
    {
        public:
            Cache();
            ~Cache();

            Cache(const Cache& other) = delete;
            Cache& operator=(const Cache& other) = delete;

            Cache(Cache&& other) = delete;
            Cache& operator=(Cache&& other) = delete;

        public:
            explicit Cache(const int cache_capacity);

        public:
            std::string get(const std::string& uri);
            bool insert(const std::string& uri, const std::string& resource);
            bool erase(const std::string& uri);
            
        private:
            std::unique_ptr<Redis> m_cache;

            /**
             * If cache size is greater than m_cache_capacity,
             * it evicts least recently used entry from the cache.
             * The unit is megabyte.
             */
            int m_cache_capacity;
    };

}