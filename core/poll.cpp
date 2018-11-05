/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: Poll
* Author:   aiyong
* Date:     2018/8/15 23:54
*/


#include "poll.hpp"

namespace tianxing {


Handle pollOpen(poll_descriptor const &descriptor)
{
    return 0;
}

int pollRead(Handle handle, char *buf, size_t length)
{
    return 0;
}

int pollWrite(Handle handle, char const *buf, size_t length)
{
    return 0;
}

void pollClose(Handle handle)
{
}


char const* pollError()
{
    return nullptr;
}

}