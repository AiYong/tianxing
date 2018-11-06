/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: StackAllocator
* Author:   aiyong
* Date:     2018/8/1 23:46
*/
#pragma once

#include <cstddef>

#include <list>
#include <boost/fiber/all.hpp>
#include <boost/context/detail/config.hpp>
#include <boost/context/stack_context.hpp>
#include <boost/context/stack_traits.hpp>

using namespace boost::context;

namespace tianxing {

/**
 *
 */
class stack_pool
{
public:

    stack_pool(std::size_t stack_size);

    ~stack_pool();

public:

    stack_context allocate();

    void deallocate( stack_context const& sctx) noexcept;

private:
    std::size_t  m_stack_size;
    std::list<stack_context> m_stack_contexts;
};


class stack_alloctor
{
public:

    stack_alloctor(stack_pool *sp);

    ~stack_alloctor();

	stack_alloctor& operator=(stack_alloctor const& o)
	{
		this->m_stack_pool = o.m_stack_pool;
	}

	stack_alloctor& operator=(stack_alloctor && o)
	{
		this->m_stack_pool = o.m_stack_pool;
	}

	stack_alloctor(stack_alloctor const& o)
	{
		this->m_stack_pool = o.m_stack_pool;
	}

	stack_alloctor(stack_alloctor && o)
	{
		this->m_stack_pool = o.m_stack_pool;
	}
public:

    stack_context allocate();

    void deallocate( stack_context & sctx) noexcept;

private:
    stack_pool *m_stack_pool;
};


}

