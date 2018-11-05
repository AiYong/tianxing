/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: StackAllocator
* Author:   aiyong
* Date:     2018/8/2 00:08
*/


#include "stack_allocator.hpp"


namespace tianxing {

stack_pool::stack_pool(std::size_t stack_size)
    :m_stack_size(stack_size)
{
    for(int i = 0 ; i < 1024 ; i++)
    {
        char* sp = new char[m_stack_size];
        m_stack_contexts.push_back(stack_context{m_stack_size,sp});
    }
}


stack_pool::~stack_pool()
{
    for(auto sc: m_stack_contexts)
    {
        char *sp = reinterpret_cast<char*>(sc.sp);
        delete sp;
    }
}

stack_context stack_pool::allocate()
{
    if(m_stack_contexts.empty())
    {
        for(int i = 0 ; i < 1024 ; i++)
        {
            char* sp = new char[m_stack_size];
            m_stack_contexts.push_back(stack_context{m_stack_size,sp});
        }
    }
    stack_context sc = m_stack_contexts.front();
    m_stack_contexts.pop_front();
    return sc;
}

void stack_pool::deallocate(stack_context const&sc) noexcept
{
    m_stack_contexts.push_back(sc);
}


stack_alloctor::stack_alloctor(stack_pool *sp)
    :m_stack_pool(sp)
{}

stack_alloctor::~stack_alloctor()
{}



stack_context stack_alloctor::allocate()
{
    return m_stack_pool->allocate();
}

void stack_alloctor::deallocate( stack_context & sctx) noexcept
{
    m_stack_pool->deallocate(sctx);
}


}