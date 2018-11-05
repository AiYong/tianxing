/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: Poller
* Author:   aiyong
* Date:     2018/7/20 23:23
*/
#pragma once

#include "platform.hpp"
#include "poll_descriptor.hpp"

namespace TianXing {


class Poller
{
public:

    virtual ~Poller()
    {}

public:

    void poll(Context::ReadyQueue& readyQueue) = 0;

public:

    /**
     *
     * @return
     */
    virtual bool initialize() =0;

    /**
     *
     * @param descriptor
     */
    virtual void open(PollDescriptor const& descriptor) = 0;

    /**
     *
     * @param handle
     * @param buf
     * @param length
     */
    virtual bool tryRead(Handle handle,char* buf,size_t length) = 0;

    /**
     *
     * @param handle
     * @param buf
     * @param length
     */
    virtual bool tryWrite(Handle handle,char const* buf,size_t length) = 0;

    /**
     *
     * @param handle
     */
    virtual void close(Handle handle) = 0;

    /**
     *
     */
    virtual void destory() = 0;
};
}