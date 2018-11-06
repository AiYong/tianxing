/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: context
* Author:   aiyong
* Date:     2018/7/15 23:31
*/

#include "coroutine_impl.hpp"
#include "scheduler.hpp"

namespace tianxing {


thread_local coroutine_impl *coroutine_impl::current_coroutine = nullptr;

coroutine_impl::coroutine_impl(task *task)
        : m_task(task)
{
}


coroutine_impl::~coroutine_impl()
{
    delete m_task;
}


void coroutine_impl::bind_to(scheduler *s, stack_alloctor &&alloctor)
{
    this->m_scheduler = s;
    m_fiber = boost::context::fiber(std::allocator_arg, alloctor, [&](boost::context::fiber &&f)
    {
        run();
        return boost::context::fiber();
    });
}


void coroutine_impl::resume()
{
    std::move(m_fiber).resume_with([&](boost::context::fiber &&f)
    {
        m_fiber = std::move(f);
        return boost::context::fiber();
    });
}


void coroutine_impl::yield()
{
    m_scheduler->schedule_next();
}


void coroutine_impl::sleep_until(std::chrono::high_resolution_clock::time_point tp)
{
    m_scheduler->sleep_until(this, tp);
}


void coroutine_impl::kill()
{
    m_scheduler->kill(this);
}


void coroutine_impl::join()
{
    coroutine_impl *waiter = coroutine_impl::current_coroutine;
    m_wait_queue.suspend(waiter);
}


scheduler *coroutine_impl::get_scheduler()
{
    return m_scheduler;
}


void coroutine_impl::suspend()
{
    m_scheduler->suspend(this);
}

void coroutine_impl::schedule(coroutine_impl *c)
{
    m_scheduler->schedule(c);
}

void coroutine_impl::schedule()
{
    m_scheduler->schedule(this);
}

void coroutine_impl::run()
{
    m_task->run();
}
}
