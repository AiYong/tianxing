/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: mcs_lock
* Author:   aiyong
* Date:     2018/10/6 23:43
*/

#include <atomic>
#include "platform.hpp"
#include "coroutine_local.hpp"

namespace tianxing {

class coroutine_impl;


class spin_lock
{
private:

    struct lock_node{
        volatile bool m_waiting ;
        volatile lock_node *m_next;
    };
    coroutine_local<lock_node> m_local;
    std::atomic<lock_node*> m_queue;

public:


    /**
     *
     */
    void lock();

    /**
     *
     */
    void unlock();

    /**
     *
     * @return
     */
    bool try_lock();


};

}