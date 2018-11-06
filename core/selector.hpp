/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: poll
* Author:   aiyong
* Date:     2018/7/7 22:56
*/
#pragma once

#include <stddef.h>

#include "platform.hpp"
#include "context.hpp"


namespace TianXing {


/**
 *
 */
class Selector
{
public:

    Selector();

    ~Selector();

public:

    void select(Context::ReadyQueue &readyQueue);

public:

    void open();

    void accept(Context* ctx,Handle handle);

    handle connect();

    void waitRead(Context* ctx,Handle handle);

    void waitWrite(Context* ctx,Handle handle);

    void close();

private:
    class Impl;
    Impl* impl;
};


}