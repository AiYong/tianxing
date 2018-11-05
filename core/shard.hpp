/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: Shard
* Author:   aiyong
* Date:     2018/9/25 20:46
*/

#include <array>
#include <boost/program_options.hpp>

#include "platform.hpp"


namespace tianxing {

using namespace boost::program_options;

class scheduler;
class shard_service;

class shard
{
public:

    static thread_local shard *current_shard;
    static std::array<shard*, 256> shards;
    static u8 shard_count;
private:
    scheduler   *m_scheduler;
public:

    shard();

    ~shard();

public:

    void configure(variables_map const& options);

public:

    scheduler* get_scheduler ();


};

}