/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: context_executor
* Author:   aiyong
* Date:     2018/7/7 22:52
*/
#pragma once

#include <set>
#include <queue>
#include <thread>
#include <functional>
#include <boost/program_options.hpp>
#include "queue.hpp"
#include "stack_allocator.hpp"



namespace tianxing {

using namespace boost::program_options;

class task;
class shard;
class coroutine_impl;


class scheduler
{
public:
    struct sleep_info
    {
        coroutine_impl *m_ci;
        std::chrono::high_resolution_clock::time_point m_tp;
    };

    struct sleep_info_less
    {
        bool operator()(sleep_info const& l,sleep_info const& r)
        {
            return l.m_tp < r.m_tp;
        }
    };
    typedef std::priority_queue<sleep_info,std::vector<sleep_info>,sleep_info_less> sleep_queue;
    typedef std::queue<coroutine_impl*>     new_queue;
    typedef std::deque<coroutine_impl*>     ready_queue;
    typedef std::set<coroutine_impl*>       suspend_set;
    typedef std::queue<coroutine_impl*>     terminated_queue;
    typedef std::function<void()>           transaction;
    typedef queue<transaction>              transaction_queue;
private:
    bool                m_stop;
    ready_queue         *m_ready_queue;
    new_queue           *m_new_queue;
    suspend_set         *m_suspend_set;
    sleep_queue         *m_sleep_queue;
    terminated_queue    *m_terminated_queue;
    transaction_queue   *m_transaction_queue;
    coroutine_impl      *m_schedule_task;
    stack_pool          *m_stock_pool;
public:

    static thread_local scheduler*  current_scheduler;

public:

    scheduler(shard* s);


    ~scheduler();

public:

    void configure(variables_map const& options);

public:


    /**
     *
     * @param task
     * @return
     */
    coroutine_impl* spawn(task *t);


    /**
     *
     * @param ctx
     */
    void schedule(coroutine_impl* ci);


    /**
     *
     */
    void schedule_next();

    /**
     *
     * @param ctx
     */
    void kill(coroutine_impl* ci);


    /**
     *
     * @param tp
     */
    void sleep_until(coroutine_impl* ci,std::chrono::high_resolution_clock::time_point tp);


    /**
     *
     * @param ctx
     */
    void suspend(coroutine_impl* ci);


public:

    /**
     *
     */
    void stop();

public:

    void operator()();

private:

    void poll_transaction();

    void poll_new();

    void poll_terminated();

    void poll_sleep();

    void poll_ready();
};

}

