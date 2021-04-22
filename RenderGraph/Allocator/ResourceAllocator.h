//
// Created by glodxy on 2021/4/22.
//

#ifndef SHADERLAB_RESOURCEALLOCATOR_H
#define SHADERLAB_RESOURCEALLOCATOR_H

#include <vector>
#include "../../Backend/Handle.h"
#include "../../Backend/DriverEnums.h"
#include "../../Backend/TargetBufferInfo.h"
#include "../../libs/utils/Hash.h"
namespace GCore::RG {
    namespace Backend{
        class DriverApi;
    }

    /*
     *
     * */
    class ResourceAllocator {
    public:
        explicit ResourceAllocator(Backend::DriverApi &driverApi) noexcept;

        ~ResourceAllocator() noexcept=default;

        void Terminate() noexcept;
        //Render Target---------------------------------------------
        Backend::RenderTargetHandle CreateRenderTarget(const char *name,
                                                       Backend::TargetBufferFlags targetBufferFlags,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint8_t samples,
                                                       Backend::MRT color,
                                                       Backend::TargetBufferInfo depth,
                                                       Backend::TargetBufferInfo stencil) noexcept;

        void DestroyRenderTarget(Backend::RenderTargetHandle h) noexcept;
        //Texture----------------------------------------------------
        Backend::TextureHandle CreateTexture(const char *name, Backend::SamplerType target,
                                             uint8_t levels, Backend::TextureFormat format, uint8_t samples,
                                             uint32_t width, uint32_t height, uint32_t depth,
                                             std::array<Backend::TextureSwizzle, 4> swizzle,
                                             Backend::TextureUsage usage) noexcept;

        void DestroyTexture(Backend::TextureHandle h) noexcept;

        void gc() noexcept;

    private:
        // TODO: these should be settings of the engine
        static constexpr size_t CACHE_CAPACITY = 64u << 20u;   // 64 MiB
        static constexpr size_t CACHE_MAX_AGE = 30u;

        //描述一个纹理
        struct TextureKey {
            const char *name; // doesn't participate in the hash
            Backend::SamplerType target;
            uint8_t levels;
            Backend::TextureFormat format;
            uint8_t samples;
            uint32_t width;
            uint32_t height;
            uint32_t depth;
            Backend::TextureUsage usage;
            std::array<Backend::TextureSwizzle, 4> swizzle;

            size_t getSize() const noexcept;

            bool operator==(const TextureKey &other) const noexcept {
                return target == other.target &&
                       levels == other.levels &&
                       format == other.format &&
                       samples == other.samples &&
                       width == other.width &&
                       height == other.height &&
                       depth == other.depth &&
                       usage == other.usage &&
                       swizzle == other.swizzle;
            }

            friend size_t hash_value(TextureKey const &k) {
                size_t seed = 0;
                Utils::Hash::combine_fast(seed, k.target);
                Utils::Hash::combine_fast(seed, k.levels);
                Utils::Hash::combine_fast(seed, k.format);
                Utils::Hash::combine_fast(seed, k.samples);
                Utils::Hash::combine_fast(seed, k.width);
                Utils::Hash::combine_fast(seed, k.height);
                Utils::Hash::combine_fast(seed, k.depth);
                Utils::Hash::combine_fast(seed, k.usage);
                Utils::Hash::combine_fast(seed, k.swizzle[0]);
                Utils::Hash::combine_fast(seed, k.swizzle[1]);
                Utils::Hash::combine_fast(seed, k.swizzle[2]);
                Utils::Hash::combine_fast(seed, k.swizzle[3]);
                return seed;
            }
        };
//todo:待研究----------------------------------------------------------
        struct TextureCachePayload {
            Backend::TextureHandle handle;
            size_t age = 0;
            uint32_t size = 0;
        };

        template<typename T>
        struct Hasher {
            std::size_t operator()(T const& s) const noexcept {
                return hash_value(s);
            }
        };

        template<typename T>
        struct Hasher<Backend::Handle<T>> {
            std::size_t operator()(Backend::Handle<T> const& s) const noexcept {
                    std::hash<typename backend::Handle<T>::HandleId> hash{};
                    return hash(s.getId());
            }
        };

        inline void dump(bool brief = false) const noexcept;

        template<typename Key, typename Value, typename Hasher = Hasher<Key>>
        class AssociativeContainer {
            // We use a std::vector instead of a std::multimap because we don't expect many items
            // in the cache and std::multimap generates tons of code. Even with more items, we
            // could improve this trivially by using a sorted std::vector.
            using Container = std::vector<std::pair<Key, Value>>;
            Container mContainer;
        public:
            using iterator = typename Container::iterator;
            using const_iterator = typename Container::const_iterator;
            using key_type = typename Container::value_type::first_type;
            using value_type = typename Container::value_type::second_type;

            size_t size() const { return mContainer.size(); }
            iterator begin() { return mContainer.begin(); }
            const_iterator begin() const { return mContainer.begin(); }
            iterator end() { return mContainer.end(); }
            const_iterator end() const  { return mContainer.end(); }
            iterator erase(iterator it);
            const_iterator find(key_type const& key) const;
            iterator find(key_type const& key);
            template<typename ... ARGS>
            void emplace(ARGS&&... args);
        };

        using CacheContainer = AssociativeContainer<TextureKey, TextureCachePayload>;

        CacheContainer::iterator purge(CacheContainer::iterator const& pos);

        Backend::DriverApi& mBackend;
        CacheContainer mTextureCache;
        AssociativeContainer<Backend::TextureHandle, TextureKey> mInUseTextures;
        size_t mAge = 0;
        uint32_t mCacheSize = 0;
        const bool mEnabled = true;

    };
}



#endif //SHADERLAB_RESOURCEALLOCATOR_H
