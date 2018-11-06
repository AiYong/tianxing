/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: main
* Author:   aiyong
* Date:     2018/7/20 23:42
*/
#include "core.hpp"

int main(int argc ,char** argv)
{
    tianxing::configure(argc,argv);
    int ret = 0;
    tianxing::run();
    tianxing::exit();
    return ret;
}
