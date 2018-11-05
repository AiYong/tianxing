/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: wait_queue
* Author:   aiyong
* Date:     2018/10/7 22:04
*/


#include "wait_queue.hpp"
#include "coroutine_impl.hpp"

namespace tianxing {


void wait_queue::suspend(tianxing::coroutine_impl *ci)
{
    m_queue.push(ci);
    ci->suspend();
}


void wait_queue::wakeup_one()
{
    m_queue.consume_one([](coroutine_impl* ci){
        ci->schedule();
    });
}


void wait_queue::wakeup_all()
{
    m_queue.consume_all([](coroutine_impl* ci){
        ci->schedule();
    });
}

}
