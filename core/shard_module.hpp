/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: shard_module
* Author:   aiyong
* Date:     2018/10/28 18:42
*/
#pragma once

#include <type_traits>
#include <boost/program_options.hpp>

#include "core.hpp"
#include "shard_service.hpp"

namespace tianxing {

using namespace boost::program_options;


class shard;


class shard_module
{
public:

    virtual ~shard_module()
    {}

public:

    /**
     *
     * @param options
     */
    virtual void register_options(options_description& options) = 0;

    /**
     *
     * @param options
     */
    virtual void configure(variables_map const& options) = 0;

public:

    /**
     *
     * @param s
     * @return
     */
    virtual shard_service* configure_for(shard *s) = 0;

    /**
     *
     * @param s
     */
    virtual void destory_service(shard_service *s) = 0;
};


namespace details {

template<typename T>
class shard_module_register
{
public:
    static_assert(std::is_convertible<T,shard_module>::value,"T must is subclass of shard_module");
    shard_module_register()
    {
        T *t = new T();
        register_module(t);
    }
};

}

#define declare_module(type)\
    static details::shard_module_register<type> __##type##module_register

}
