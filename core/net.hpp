/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: NetAPI
* Author:   aiyong
* Date:     2018/7/20 23:24
*/
#pragma once

#include "platform.hpp"

namespace TianXing {

enum NetCallType {
    Accept,
    Read,
    Write,
    Connect
};

/**
 *
 * @param socket
 * @param callType
 * @param buf
 * @param length
 */
void enterNetCall(Socket socket,NetCallType callType,char* buf,size_t length);

}

