/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: coroutine_local
* Author:   aiyong
* Date:     2018/10/6 23:48
*/

#include "coroutine_impl.hpp"


namespace tianxing {

template <typename T>
class coroutine_local
{
public:

    T* get()
    {
        T* result = nullptr;
        auto cur = coroutine_impl::current_coroutine;
        auto iter = cur->m_css_data.find(this);
        if(iter == cur->m_css_data.end())
        {
            result = new T();
            cur->m_css_data[this] = css_data {result,[](void* t){
                auto* tt = reinterpret_cast<T*>(t);
                delete tt;
            }};
        }
        else
        {
            result = reinterpret_cast<T*>(iter->second.m_2data);
        }
        return result;
    }


    void set(T* t)
    {
        auto cur = coroutine_impl::current_coroutine;
        auto iter = cur->m_css_data.find(this);
        if(iter != cur->m_css_data.end())
        {
            iter->second.m_clean_function(iter->second.m_2data);
            iter->second.m_2data = t;
        }
        else
        {
            cur->m_css_data[this] = css_data {t,[](void* t){
                auto* tt = reinterpret_cast<T*>(t);
                delete tt;
            }};
        }
    }

};

}

