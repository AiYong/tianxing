/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: PollDescriptor
* Author:   aiyong
* Date:     2018/7/22 13:05
*/
#pragma once

#include <string>

namespace tianxing {

/**
 *
 */
struct poll_descriptor {
    int         type;
    std::string path;
};

}