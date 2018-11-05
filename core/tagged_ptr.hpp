/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: tagged_ptr
* Author:   aiyong
* Date:     2018/10/18 22:37
*/
#pragma once

#include <limits>

#include "platform.hpp"


namespace tianxing {

template <class T>
class tagged_ptr
{
    typedef u64 compressed_ptr_t;
public:
    typedef u16 tag_t;
protected:
    compressed_ptr_t ptr;
private:

    union cast_unit
    {
        compressed_ptr_t value;
        tag_t tag[4];
    };

    static const int tag_index = 3;
    static const compressed_ptr_t ptr_mask = 0xffffffffffffUL;

    static T* extract_ptr(volatile compressed_ptr_t const & i)
    {
        return (T*)(i & ptr_mask);
    }

    static tag_t extract_tag(volatile compressed_ptr_t const & i)
    {
        cast_unit cu;
        cu.value = i;
        return cu.tag[tag_index];
    }

    static compressed_ptr_t pack_ptr(T * ptr, tag_t tag)
    {
        cast_unit ret;
        ret.value = compressed_ptr_t(ptr);
        ret.tag[tag_index] = tag;
        return ret.value;
    }

public:

    tagged_ptr(void) noexcept
    {}

    tagged_ptr(tagged_ptr const & p) = default;

    explicit tagged_ptr(T * p, tag_t t = 0)
            :ptr(pack_ptr(p, t))
    {}

    tagged_ptr & operator= (tagged_ptr const & p) = default;

    void set(T *p, tag_t t)
    {
        ptr = pack_ptr(p, t);
    }

    bool operator== (volatile tagged_ptr const & p) const
    {
        return (ptr == p.ptr);
    }

    bool operator!= (volatile tagged_ptr const & p) const
    {
        return !operator==(p);
    }

    T * get_ptr() const
    {
        return extract_ptr(ptr);
    }

    void set_ptr(T * p)
    {
        tag_t tag = get_tag();
        ptr = pack_ptr(p, tag);
    }

    tag_t get_tag() const
    {
        return extract_tag(ptr);
    }

    tag_t get_next_tag() const
    {
        tag_t next = (get_tag() + 1u) & (std::numeric_limits<tag_t>::max)();
        return next;
    }

    void set_tag(tag_t t)
    {
        T * p = get_ptr();
        ptr = pack_ptr(p, t);
    }

    T & operator*() const
    {
        return *get_ptr();
    }

    T * operator->() const
    {
        return get_ptr();
    }

    operator bool(void) const
    {
        return get_ptr() != 0;
    }

};


}
