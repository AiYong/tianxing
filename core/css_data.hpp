/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: css_data
* Author:   aiyong
* Date:     2018/10/9 23:22
*/
#pragma once

#include <functional>

namespace tianxing {

typedef std::function<void(void*)> clean_function;

struct css_data {
    void* m_2data;
    clean_function m_clean_function;
};

}

