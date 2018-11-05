/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: run_context.hpp
* Author:   aiyong
* Date:     2018/7/7 22:35
*/
#pragma once

#include <assert.h>

#include <map>
#include <chrono>
#include <atomic>
#include <boost/context/fiber.hpp>

#include "task.hpp"
#include "css_data.hpp"
#include "wait_queue.hpp"
#include "stack_allocator.hpp"

namespace tianxing {


class scheduler;

/**
 *
 */
class coroutine_impl
{
private:

    friend class wait_queue;
    template <typename T>
    friend class coroutine_local;

private:
    task *m_task;
    scheduler* m_scheduler;
    wait_queue m_wait_queue;
    boost::context::fiber m_fiber;
    std::map<void*,css_data> m_css_data;

public:
    static thread_local coroutine_impl* current_coroutine;
public:

    coroutine_impl(task *t);

    ~coroutine_impl();

    coroutine_impl(coroutine_impl&&) = delete;
    coroutine_impl(coroutine_impl const&&) = delete;

    coroutine_impl& operator=(coroutine_impl&&) = delete;
    coroutine_impl& operator=(coroutine_impl const&) = delete;

public:

    scheduler* get_scheduler();


    /**
     * 启动协程运行。
     * @param s
     * @param alloctor
     */
    void bind_to(scheduler *s, stack_alloctor &&alloctor);

    /**
     * 恢复协程运行
     */
    void resume();

public:

    /**
     * 执行下一个上下文
     */
    void yield();


    /**
     * 当前上下文睡眠至指定时间点
     * @param tp
     */
    void sleep_until(std::chrono::high_resolution_clock::time_point tp);

    /**
     * 终止当前上下文
     */
    void kill();


    /**
     * 等待协程退出
     * @param waiter
     */
    void join();


    /**
     *挂起当前协程
     */
    void suspend();

    /**
     * 调度coroutine 运行
     * @param c
     */
    void schedule(coroutine_impl *c);

    /**
     * 调度自己运行
     */
    void schedule();

public:

    void run();

};

}
