/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: coroutine
* Author:   aiyong
* Date:     2018/10/19 00:09
*/


#include <chrono>

#include "schedule.hpp"


namespace tianxing {


class coroutine_impl;
/**
 *
 */
class coroutine
{
private:
    coroutine_impl *m_impl;
public:

    /**
     * 默认构造
     */
    coroutine() noexcept;

    /**
     *
     * @tparam Fun
     * @tparam Args
     * @param fun
     * @param args
     */
    template< class Fun, class... Args >
    explicit coroutine( Fun&& fun, Args&&... args)
    {
        m_impl = spawn(fun,std::forward(args)...);
    };

    /**
     *
     * @param other
     */
    coroutine(coroutine &&other) noexcept ;

    /**
     *
     * @param co
     * @return
     */
    coroutine& operator=(coroutine &&co) noexcept ;


    coroutine(coroutine const&) = delete;
    coroutine&operator=(coroutine const&) = delete;

public:
    /**
     * 返回硬件并发度
     * @return
     */
    static size_t  hardware_concurrency();

public:

    /**
     * 终止协程运行
     */
    void kill();


    /**
     * 阻塞当前协程，直到协程退出
     * @param waiter
     */
    void join();


    /**
     * 检测是否关联运行的协程，默认情况下协程是非关联的
     * @return
     */
    bool joinable();


    /**
     *
     */
    void detach();

    /**
     *
     * @param o
     */
    void swap(coroutine& o);

};


namespace this_coroutine {

/**
 *  调度其他协程运行
 */
void yield();


/**
 * 阻塞当前协程一段时间
 * @tparam Rep
 * @tparam Period
 * @param sleep_duration 阻塞时间
 */
template< class Rep, class Period >
void sleep_for( const std::chrono::duration<Rep, Period>& sleep_duration )
{
    auto sleep_time = std::chrono::high_resolution_clock::now() + sleep_duration;
    tianxing::sleep_until(sleep_time);
};


/**
 * 阻塞协程到指定时间点
 * @tparam Clock
 * @tparam Duration
 * @param sleep_time
 */
template< class Clock, class Duration >
void sleep_until( const std::chrono::time_point<Clock,Duration>& sleep_time )
{
    std::chrono::time_point<Clock,Duration> now = Clock::now();
    Duration d = sleep_time - now;
    if(d.count() > 0)
    {
        sleep_for(d);
    }
};

}


}