/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: ExecutorContext
* Author:   aiyong
* Date:     2018/7/23 21:54
*/


#include "scheduler.hpp"

#include "context.hpp"

namespace tianxing {

thread_local scheduler* scheduler::current_scheduler = nullptr;

scheduler::scheduler(shard* s)
{
}


scheduler::~scheduler()
{
}


void scheduler::operator()()
{
    current_scheduler = this;
    m_stop = false;
    poll_transaction();
    poll_new();
    coroutine_impl::current_context = m_schedule_task;
    m_schedule_task->resume();
//    while(!m_stop)
//    {
//        m_schedule_task->resume();
//    }
}

void scheduler::poll_transaction()
{
    size_t task_size = m_transaction_queue->size();
    for(size_t i = 0; i < task_size ;i++)
    {
        m_transaction_queue->consume_one([](transaction& t){
            t();
        });
    }
}

void scheduler::poll_new()
{
    while(!m_new_queue->empty())
    {
        auto *ci = m_new_queue->front();
        ci->bind_to(this,stack_alloctor(m_stock_pool));
        m_ready_queue->push_back(ci);
        m_new_queue->pop();
    }
}

void scheduler::poll_terminated()
{
    terminated_queue tmp;
    swap(tmp,*m_terminated_queue);
}

void scheduler::poll_sleep()
{
    if(m_sleep_queue->empty())
    {
        return;
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto t = m_sleep_queue->top();
    for(;;)
    {
        if(t.m_tp > now)
        {
            break;
        }
        m_ready_queue->push_back(t.m_ci);
        m_sleep_queue->pop();
        t = m_sleep_queue->top();
    }
}


void scheduler::poll_ready()
{
    if(m_ready_queue->empty())
    {
        std::this_thread::yield();
    }
    else
    {
        schedule_next();
    }
}


void scheduler::configure(variables_map const& options)
{
    m_new_queue = new new_queue();
    m_ready_queue = new ready_queue();
    m_suspend_set = new suspend_set();
    m_sleep_queue = new sleep_queue();
    m_terminated_queue = new terminated_queue();
    m_transaction_queue = new transaction_queue();
    size_t stack_size = 4 * 1024;
    if(options.count("stack_size") > 0)
    {
        stack_size = options["stack_size"].as<size_t >();
    }
    m_stock_pool = new stack_pool(stack_size);
    m_schedule_task = spawn(make_task([&](){
        while(!m_stop)
        {
            poll_transaction();
            poll_new();
            poll_sleep();
            poll_terminated();
            poll_ready();
        }
    }));
}


void scheduler::stop()
{
    m_stop = true;
}


coroutine_impl *scheduler::spawn(task *t)
{
    auto *ci = new coroutine_impl(t);
    struct new_transaction{
        void operator()()
        {
            m_new_queue->push(m_ci);
        };
        new_queue *m_new_queue;
        coroutine_impl *m_ci;
    };
    new_transaction nt;
    nt.m_new_queue = m_new_queue;
    nt.m_ci = ci;
    m_transaction_queue->push(nt);
    return ci;
}


void scheduler::schedule(coroutine_impl *ci)
{
    m_transaction_queue->push([&](){
        m_suspend_set->erase(ci);
        m_ready_queue->push_back(ci);
    });
}


void scheduler::schedule_next()
{
    m_ready_queue->push_back(coroutine_impl::current_context);
    m_ready_queue->pop_front();
    coroutine_impl::current_context = m_ready_queue->front();
    coroutine_impl::current_context->resume();
}


void scheduler::kill(coroutine_impl *ci)
{
    m_transaction_queue->push([&](){
        auto iter = std::find(m_ready_queue->begin(),m_ready_queue->end(),ci);
        m_ready_queue->erase(iter);
        m_terminated_queue->push(ci);
    });
}


void scheduler::sleep_until(coroutine_impl *ci, std::chrono::high_resolution_clock::time_point tp)
{
    m_sleep_queue->push(sleep_info{ci,tp});
    m_ready_queue->pop_front();
    coroutine_impl::current_context = m_ready_queue->front();
    coroutine_impl::current_context->resume();
}


void scheduler::suspend(coroutine_impl *ci)
{
    m_suspend_set->insert(ci);
    m_ready_queue->pop_front();
    coroutine_impl::current_context = m_ready_queue->front();
    coroutine_impl::current_context->resume();
}


}