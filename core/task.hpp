/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: ContextFunc
* Author:   aiyong
* Date:     2018/7/14 13:13
*/
#pragma once

#include <tuple>
#include <utility>
#include <functional>

namespace tianxing {

class task
{
public:
    virtual ~task(){};
    virtual void run() = 0;
};

template <typename Fn,typename ...Args>
class task_impl : public task
{
private:

    typedef typename std::decay<Fn>::type fn_type;
    typedef std::tuple<Args...>  arg_type;
    fn_type     fn;
    arg_type    arg;

public:

    task_impl(Fn&& fn,Args&& ... args)
        :fn(std::forward<fn_type>(fn)),arg(std::forward<Args>(args)...)
    {
    }

    ~task_impl()
    {}

public:

    void run()
    {
        std::apply(std::forward<fn_type >(fn),std::forward<arg_type>(arg));
    }

};



template <typename Fn,typename ...Args>
task* make_task(Fn&& fn,Args&&...args)
{
    return new task_impl<Fn,Args...>(std::forward<Fn>(fn),std::forward<Args>(args)...);
};



}

