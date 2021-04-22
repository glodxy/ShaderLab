//
// Created by glodxy on 2021/4/22.
//

#ifndef SHADERLAB_HASH_H
#define SHADERLAB_HASH_H

#include <cstdint>
//from google
namespace GCore::Utils::Hash{
    inline uint32_t murmur3(const uint32_t* key, size_t wordCount, uint32_t seed) noexcept {
        uint32_t h = seed;
        size_t i = wordCount;
        do {
            uint32_t k = *key++;
            k *= 0xcc9e2d51u;
            k = (k << 15u) | (k >> 17u);
            k *= 0x1b873593u;
            h ^= k;
            h = (h << 13u) | (h >> 19u);
            h = (h * 5u) + 0xe6546b64u;
        } while (--i);
        h ^= wordCount;
        h ^= h >> 16u;
        h *= 0x85ebca6bu;
        h ^= h >> 13u;
        h *= 0xc2b2ae35u;
        h ^= h >> 16u;
        return h;
    }

    template<typename T>
    struct MurmurHashFn {
        uint32_t operator()(const T& key) const noexcept {
            static_assert(0 == (sizeof(key) & 3u), "Hashing requires a size that is a multiple of 4.");
            return murmur3((const uint32_t*) &key, sizeof(key) / 4, 0);
        }
    };

// combines two hashes together
    template<class T>
    inline void combine(size_t& seed, const T& v) noexcept {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9u + (seed << 6u) + (seed >> 2u);
    }

// combines two hashes together, faster but less good
    template<class T>
    inline void combine_fast(size_t& seed, const T& v) noexcept {
        std::hash<T> hasher;
        seed ^= hasher(v) << 1u;
    }
}
#endif //SHADERLAB_HASH_H
