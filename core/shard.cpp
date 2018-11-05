/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: Shard
* Author:   aiyong
* Date:     2018/9/25 20:46
*/

#include "shard.hpp"
#include "scheduler.hpp"
#include <boost/fiber/all.hpp>
namespace tianxing {

thread_local shard* shard::current_shard = nullptr;
u8 shard::shard_count = 0;
std::array<shard*, 256> shard::shards;

shard::shard()
{
    m_scheduler = new scheduler(this);
}

shard::~shard()
{
    delete m_scheduler;
}

void shard::configure(variables_map const &options)
{
    m_scheduler->configure(options);
}

scheduler *shard::get_scheduler()
{
    return m_scheduler;
}
}