/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: Schedule
* Author:   aiyong
* Date:     2018/9/25 20:46
*/
#pragma once

#include <chrono>

#include "task.hpp"
#include "platform.hpp"



namespace tianxing {


class coroutine_impl;


/**
 * 设置时间片
 * @param time_slice
 */
void set_time_slice(std::chrono::high_resolution_clock::duration time_slice);


/**
 * 设置调度粒度
 * @param granularity
 */
void set_schedule_granularity(u32 granularity);


/**
 * 判断当前是否抵达抢断点
 */
bool is_preempt_point();



/**
 * 创建新协程，并在当前shard运行
 * @param task
 * @return
 */
coroutine_impl* spawn(task* task);


/**
 * 创建新协程
 * @param task
 * @return
 */
coroutine_impl* spawn(task* task, u8 shard_id);


/**
 * 创建新协程
 * @tparam Fn
 * @tparam Args
 * @param fn
 * @param args
 * @return
 */
template <typename Fn,typename ...Args>
coroutine_impl* spawn(Fn&& fn,Args&&... args)
{
    return spawn(make_task(std::forward<Fn>(fn),std::forward<Args>(args)...));
};

/**
 * 创建新协程,并在指定shard上运行
 * @tparam Fn
 * @tparam Args
 * @param fn
 * @param args
 * @return
 */
template <typename Fn,typename ...Args>
coroutine_impl* spawn(u8 shard_id,Fn&& fn,Args&&... args)
{
    spawn(make_task(fn,std::forward<Args>(args)...),shard_id);
};



/**
 * 返回当前正在运行的任务的协程
 * @return
 */
coroutine_impl* current_coroutine();


/**
* 请求调度协程运行
* @param co 要调度的协程
* @return
*/
void schedule(coroutine_impl* co);



/**
 * 等待指定协程退出
 * @param co
 */
void wait(coroutine_impl* co);


/**
 * 切换运行权到下一个协程
 */
void yield();



/**
 * 睡眠当前协程
 * @param tp
 */
void sleep_until(std::chrono::high_resolution_clock::time_point tp);



/**
 * 挂起当前协程
 */
void suspend();


/**
 * 挂起指定协程
 * @param co
 */
void suspend(coroutine_impl* co);



/**
 * 停止指定协程
 * @param co
 */
void kill(coroutine_impl* co);


/**
 * 执行自定函数，如果到达抢断点，切换到另一个协程运行
 * @tparam Fn
 * @tparam Args
 * @param fn
 * @param args
 */
template <typename Fn,typename ...Args>
void execute(Fn&& fn,Args&&... args)
{
    if(is_preempt_point())
    {
        yield();
    }
    fn(std::forward<Args>(args)...);
};


}