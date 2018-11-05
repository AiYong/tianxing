/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: queue
* Author:   aiyong
* Date:     2018/10/18 23:07
*/
#pragma once

#include <atomic>
#include "tagged_ptr.hpp"

namespace tianxing {

/**
 * 多生产者多消费者无锁队列，入队数据使用线程
 * @tparam T
 */
template<typename T>
class queue
{
private:
    template<typename TT>
    class node
    {
    public:
        typedef tagged_ptr<node> tagged_node_ptr;
    public:
        std::atomic<tagged_node_ptr> m_next;
        TT m_data;
    public:

        node()
            :m_data(),m_next(tagged_node_ptr(nullptr,0))
        {}

        node(TT const&data)
            :m_data(data),m_next(tagged_node_ptr(nullptr,0))
        {}
    };

    typedef node<T> node_type;
    typedef typename node<T>::tagged_node_ptr tagged_node_ptr;
    std::atomic<tagged_node_ptr> m_head;
    std::atomic<tagged_node_ptr> m_tail;
    std::atomic_size_t  m_size;

public:
    queue()
    {
        node_type* n = new node_type();
        tagged_node_ptr dummy_node(n,0);
        m_head.store(dummy_node);
        m_tail.store(dummy_node);
        m_size.store(0);
    }

    ~queue()
    {
    }

public:

    bool push(T const&t)
    {
        node_type* n = new node_type(t);
        if(n == nullptr)
        {
            return false;
        }
        for(;;)
        {
            tagged_node_ptr tail = m_tail.load(std::memory_order_acquire);
            node_type* tail_node = tail.get_ptr();
            tagged_node_ptr next = tail_node->m_next.load(std::memory_order_acquire);
            node_type* next_node = next.get_ptr();
            tagged_node_ptr tail2 = m_tail.load(std::memory_order_acquire);
            if(tail == tail2)
            {
                if(next_node == nullptr)
                {
                    tagged_node_ptr new_tail_next(n, next.get_next_tag());
                    if(tail_node->m_next.compare_exchange_weak(next, new_tail_next))
                    {
                        tagged_node_ptr new_tail(n, tail.get_next_tag());
                        m_tail.compare_exchange_strong(tail, new_tail);
                        m_size++;
                        return true;
                    }
                }
            }
        }
        return false;
    }


    bool pop(T &t)
    {
        for(;;)
        {
            tagged_node_ptr head = m_head.load(std::memory_order_acquire);
            node_type *head_node = head.get_ptr();
            tagged_node_ptr tail = m_tail.load(std::memory_order_acquire);
            tagged_node_ptr next = head_node->m_next.load(std::memory_order_acquire);
            node_type *next_node = next.get_ptr();
            tagged_node_ptr head2 = m_head.load(std::memory_order_acquire);
            if(head == head2)
            {
                if(head.get_ptr() == tail.get_ptr())
                {
                    if(next_node == nullptr)
                    {
                        return false;
                    }
                }
                else
                {
                    if(next_node == nullptr)
                    {
                        continue;
                    }
                    t = next_node->m_data;
                    tagged_node_ptr new_head(next_node,head.get_next_tag());
                    if(m_head.compare_exchange_weak(head, new_head))
                    {
                        delete head_node;
                        m_size--;
                        return true;
                    }
                }
            }
        }
        return false;
    }

public:

    size_t size()
    {
        return m_size.load(std::memory_order_acquire);
    }

public:

    template <typename Functor>
    bool consume_one(Functor & f)
    {
        T element;
        bool success = pop(element);
        if (success)
            f(element);
        return success;
    }


    template <typename Functor>
    bool consume_one(Functor const & f)
    {
        T element;
        bool success = pop(element);
        if (success)
            f(element);

        return success;
    }

    template <typename Functor>
    size_t consume_all(Functor & f)
    {
        size_t element_count = 0;
        while (consume_one(f))
            element_count += 1;
        return element_count;
    }

    template <typename Functor>
    size_t consume_all(Functor const & f)
    {
        size_t element_count = 0;
        while (consume_one(f))
            element_count += 1;

        return element_count;
    }
};


}

