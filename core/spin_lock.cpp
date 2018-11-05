/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: spin_lock
* Author:   aiyong
* Date:     2018/10/6 23:43
*/
//
// Created by 艾勇 on 2018/10/6.
//

#include "spin_lock.hpp"


namespace tianxing {


void spin_lock::lock()
{
    lock_node *node = m_local.get(),*head = nullptr;
    node->m_waiting = true;
    do
    {
        head = m_queue.load();
    }while(!m_queue.compare_exchange_weak(head,node));
    if(head == nullptr)
    {
        return;
    }
    head->m_next = node;
    while(node->m_waiting)
        asm("pause");
}


void spin_lock::unlock()
{
    lock_node *node = m_local.get(),*head = nullptr;
    head = m_queue.load();
    if(head->m_next == nullptr && m_queue.compare_exchange_weak(head, nullptr))
    {
        return;
    }
    while(head->m_next == nullptr)
        asm("pause");
    head->m_next->m_waiting = false;
}


bool spin_lock::try_lock()
{
    lock_node *node = m_local.get(),*head = nullptr;
    head = m_queue.load();
    if(head == nullptr && m_queue.compare_exchange_weak(head,node))
    {
        return true;
    }
    return false;
}

}
