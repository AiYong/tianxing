/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: PollAPI
* Author:   aiyong
* Date:     2018/7/20 23:24
*/
#pragma once

#include <string>
#include "platform.hpp"
#include "poll_descriptor.hpp"

namespace tianxing {


/**
 *
 * @param descriptor
 * @return
 */
Handle pollOpen(poll_descriptor const& descriptor);

/**
 *
 * @param handle
 * @param buf
 * @param length
 */
int pollRead(Handle handle,char* buf,size_t length);

/**
 *
 * @param handle
 * @param buf
 * @param length
 */
int pollWrite(Handle handle,char const* buf,size_t length);

/**
 *
 * @param handle
 */
void pollClose(Handle handle);

/**
 *
 * @return
 */
char const* pollError();


}