#ifndef RAPIDJSON_ALLOCATORS_H_
#define RAPIDJSON_ALLOCATORS_H_

#include "rapidjson.h"

RAPIDJSON_NAMESPACE_BEGIN

class CrtAllocator {
    public:
        static const bool kNeedFree = true;
        void* Malloc(size_t size) { return std::malloc(size); }
        void* Realloc(void* originalPtr, size_t originalSize, size_t newSize) { (void)originalSize; return std::realloc(originalPtr, newSize); }
        static void Free(void *ptr) { std::free(ptr); }
};


template <typename BaseAllocator = CrtAllocator>
class MemoryPoolAllocator {
    public:
        static const bool kNeedFree = false;

        MemoryPoolAllocator(size_t chunkSize = kDefaultChunkCapacity, BaseAllocator* baseAllocator = 0) :
            chunkHead_(0), chunk_capacity_(chunkSize), userBuffer_(0), baseAllocator_(baseAllocator), ownBaseAllocator_(0) { }
        MemoryPoolAllocator(void *buffer, size_t size, size_t chunkSize = kDefaultChunkCapacity, BaseAllocator* baseAllocator = 0) :
            chunkHead_(0), chunk_capacity_(chunkSize), userBuffer_(buffer), baseAllocator_(baseAllocator), ownBaseAllocator_(0)
        {
            RAPIDJSON_ASSERT(buffer != 0);
            RAPIDJSON_ASSERT(size > sizeof(ChunkHeader));
            chunkHead_ = reinterpret_cast<ChunkHeader*>(buffer);
            chunkHead_->capacity = size - sizeof(ChunkHeader);
            chunkHead_->size = 0;
            chunkHead_->next = 0;
        }
        ~MemoryPoolAllocator() {
            Clear();
            RAPIDJSON_DELETE(ownBaseAllocator_);
        }

        void Clear() {
            while (chunkHead_ != 0 && chunkHead_ != userBuffer_) {
                ChunkHeader* next = chunkHead_->next;
                baseAllocator_->Free(chunkHead_);
                chunkHead_ = next;
            }
        }

        size_t Capacity() const {
            size_t capacity = 0;
            for (ChunkHeader* c = chunkHead_; c != 0; c = c->next) {
                capacity += c->capacity;
            }
            return capacity;
        }

        size_t Size() const {
            size_t size = 0;
            for (ChunkHeader* c = chunkHead_; c != 0; c= c->next) {
                size += c->size;
            }
            return size;
        }

        void* Malloc(size_t size) {
            size = RAPIDJSON_ALIGN(size);
            if (chunkHead_ == 0 || chunkHead_->size + size > chunkHead_->capacity) {
                addChunk(chunk_capacity_ > size ? chunk_capacity_ : size);
            }
            void *buffer = reinterpret_cast<char *>(chunkHead_ + 1) + chunkHead_->size;
            chunkHead_->size += size;
            return buffer;
        }

        void* Realloc(void* originalPtr, size_t originalSize, size_t newSize) {
            if (originalPtr == 0) {
                return Malloc(newSize);
            }
            if (originalSize >= newSize) {
                return originalPtr;
            }

            if (originalPtr == (char *)(chunkHead_ + 1) + chunkHead_->size - originalSize) {
                size_t increment = static_cast<size_t>(newSize - originalSize);
                increment = RAPIDJSON_ALIGN(increment);
                if (chunkHead_->size + increment <= chunkHead_->capacity) {
                    chunkHead_->size += increment;
                    return originalPtr;
                }
            }

            void* newBuffer = Malloc(newSize);
            RAPIDJSON_ASSERT(newBuffer != 0);
            return std::memcpy(newBuffer, originalPtr, originalSize);
        }

        static void Free(void *ptr) { (void)ptr; }

    private:
        MemoryPoolAllocator(const MemoryPoolAllocator& rhs);
        MemoryPoolAllocator& operator=(const MemoryPoolAllocator& rhs);

        void addChunk(size_t capacity) {
            if (!baseAllocator_) {
                ownBaseAllocator_ = baseAllocator_ = RAPIDJSON_NEW(BaseAllocator());
            }
            ChunkHeader* chunk = reinterpret_cast<ChunkHeader*>(baseAllocator_->Malloc(sizeof(ChunkHeader) + capacity));
            chunk->capacity = capacity;
            chunk->size = 0;
            chunk->next = chunkHead_;
            chunkHead_ = chunk;
        }

        static const int kDefaultChunkCapacity = 64 *1024;
        struct ChunkHeader {
            size_t capacity;
            size_t size;
            ChunkHeader *next;
        };

        ChunkHeader *chunkHead_;
        size_t chunk_capacity_;
        void *userBuffer_;
        BaseAllocator* baseAllocator_;
        BaseAllocator* ownBaseAllocator_;
};




RAPIDJSON_NAMESPACE_END

#endif //RAPIDJSON_ALLOCATORS_H
