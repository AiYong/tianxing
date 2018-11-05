/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: wait_queue
* Author:   aiyong
* Date:     2018/10/7 22:04
*/

#pragma once

#include "platform.hpp"

#include "queue.hpp"

namespace tianxing {

class coroutine_impl;

class wait_queue
{
private:

    queue<coroutine_impl*> m_queue;

public:

    /**
     * 加入队列，挂起协程c
     * @param c
     */
    void suspend(coroutine_impl* c);

    /**
     * 唤醒一个协程
     */
    void wakeup_one();

    /**
     * 唤醒在本队列等待的所有协程
     */
    void wakeup_all();


};


}

