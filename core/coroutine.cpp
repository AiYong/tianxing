/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: coroutine
* Author:   aiyong
* Date:     2018/10/19 00:09
*/
//
// Created by 艾勇 on 2018/10/19.
//

#include "coroutine.hpp"

#include "context.hpp"

namespace tianxing {

coroutine::coroutine() noexcept
    :m_impl(nullptr)
{
}

coroutine::coroutine(coroutine &&other) noexcept
{
    if(joinable())
    {
        std::terminate();
    }
    m_impl = other.m_impl;
    other.m_impl = nullptr;
}

coroutine &coroutine::operator=(coroutine &&other) noexcept
{
    if(joinable())
    {
        std::terminate();
    }
    m_impl = other.m_impl;
    other.m_impl = nullptr;
    return *this;
}

size_t coroutine::hardware_concurrency()
{
    return std::thread::hardware_concurrency();
}

void coroutine::kill()
{
    if(m_impl)
    {
        m_impl->kill();
    }
}

void coroutine::join()
{
    if(!joinable())
    {
        std::terminate();
    }
    m_impl->join();
}

bool coroutine::joinable()
{
    return m_impl != nullptr;
}

void coroutine::detach()
{
    m_impl = nullptr;
}

void coroutine::swap(coroutine &o)
{
    std::swap(m_impl, o.m_impl);
}

namespace this_coroutine {

void yield()
{
    current_context()->yield();
}

}
}
