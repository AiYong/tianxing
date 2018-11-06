/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: context
* Author:   aiyong
* Date:     2018/7/15 23:31
*/

#include "context.hpp"
#include "scheduler.hpp"

namespace tianxing {


thread_local context *context::current_context = nullptr;

context::context(task *task)
        : m_task(task)
{
}


context::~context()
{
    delete m_task;
}


void context::bind_to(scheduler *s, stack_alloctor &&alloctor)
{
    this->m_scheduler = s;
    m_fiber = boost::context::fiber(std::allocator_arg, alloctor, [&](boost::context::fiber &&f)
    {
        run();
        return boost::context::fiber();
    });
}


void context::resume()
{
    std::move(m_fiber).resume_with([&](boost::context::fiber &&f)
    {
        m_fiber = std::move(f);
        return boost::context::fiber();
    });
}


void context::yield()
{
    m_scheduler->schedule_next();
}


void context::sleep_until(std::chrono::high_resolution_clock::time_point tp)
{
    m_scheduler->sleep_until(this, tp);
}


void context::kill()
{
    m_scheduler->kill(this);
}


void context::join()
{
    context *waiter = context::current_context;
    m_wait_queue.suspend(waiter);
}


scheduler *context::get_scheduler()
{
    return m_scheduler;
}


void context::suspend()
{
    m_scheduler->suspend(this);
}

void context::schedule(context *c)
{
    m_scheduler->schedule(c);
}

void context::schedule()
{
    m_scheduler->schedule(this);
}

void context::run()
{
    m_task->run();
}
}
