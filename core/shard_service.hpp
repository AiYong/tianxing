/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: shard_service
* Author:   aiyong
* Date:     2018/10/28 18:39
*/
#pragma once

#include <boost/program_options.hpp>

namespace tianxing{

using namespace boost::program_options;

class shard_service
{
public:

    shard_service()
    {};

    virtual ~shard_service()
    {}

public:

    virtual void configure(variables_map const& options) =0;
};


}