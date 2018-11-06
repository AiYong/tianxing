/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: schedule
* Author:   aiyong
* Date:     2018/10/1 12:49
*/

#include "schedule.hpp"
#include "shard.hpp"
#include "scheduler.hpp"
#include "context.hpp"

namespace tianxing {

static std::chrono::high_resolution_clock::duration sched_time_slice = std::chrono::nanoseconds(10000);
static u32 sched_schedule_granularity = 1;
static std::chrono::high_resolution_clock::time_point sched_start_time = std::chrono::high_resolution_clock::now();


void set_time_slice(std::chrono::high_resolution_clock::duration time_slice)
{
    sched_time_slice = time_slice;
}

void set_schedule_granularity(u32 granularity)
{
    sched_schedule_granularity = granularity;
}


bool is_preempt_point()
{
    static thread_local auto preempt_point = sched_start_time + sched_time_slice * sched_schedule_granularity;
    auto now = std::chrono::high_resolution_clock::now();
    bool ret = false;
    while(now >= preempt_point)
    {
        ret = true;
        preempt_point = preempt_point + sched_time_slice * sched_schedule_granularity;
    }
    return ret;
}


coroutine_impl* spawn(task *task)
{
    auto* scheduler = shard::current_shard->get_scheduler();
    auto* c = scheduler->spawn(task);
    return c;
}

coroutine_impl* spawn(task *task, u8 shard_id)
{
    shard_id = shard_id % shard::shard_count;
    auto* cur_shard = shard::shards[shard_id];
    auto* scheduler = cur_shard->get_scheduler();
    auto* c = scheduler->spawn(task);
    return c;
}


void schedule(coroutine_impl *co)
{
    co->get_scheduler()->schedule(co);
}


void wait(coroutine_impl *co)
{
    co->join();
}


void sleep_until(std::chrono::high_resolution_clock::time_point tp)
{
    auto* co = current_context();
    co->sleep_until(tp);
}


void suspend()
{
    auto* co = current_context();
    co->suspend();
}


void suspend(coroutine_impl* co)
{
    co->suspend();
}


void kill(coroutine_impl *co)
{
    co->kill();
}


coroutine_impl *current_context()
{
    return coroutine_impl::current_context;
}


void yield()
{
    return coroutine_impl::current_context->yield();
}
}
