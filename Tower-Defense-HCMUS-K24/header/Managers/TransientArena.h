#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>

struct Arena {
    uint8_t* buffer;
    size_t   capacity;
    size_t   offset;

    void init(void* buf, long long cap) 
    {
        buffer = static_cast<uint8_t*>(buf);
        capacity = cap;
        offset = 0;
    }

    void* alloc(long long sz) 
    {
        size_t aligned = (sz + 15) & ~15;
        if (offset + aligned > capacity) 
        {
            assert(false && "Arena out of memory");
            return nullptr;
        }
        void* ptr = buffer + offset;
        offset += aligned;
        return ptr;
    }

    void reset() 
    {
        offset = 0;
    }
};

