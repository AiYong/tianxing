/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: core
* Author:   aiyong
* Date:     2018/10/19 00:12
*/


#include "core.hpp"

#include <array>
#include <thread>
#include <boost/program_options.hpp>

#include "shard.hpp"
#include "scheduler.hpp"
#include "shard_module.hpp"
#include "task.hpp"
#include "schedule.hpp"

extern int txmain(int, char**);

namespace tianxing {


struct tx_runtime {
    size_t                          m_shard_count;
    std::array<shard*,256>          m_shards;
    std::array<std::thread*,256>    m_threads;
    std::list<shard_module*>        m_module;
    options_description             m_options;
    int argc;
    char** argv;
};


static tx_runtime tx;

/**
 * 配置框架
 * @param argc
 * @param argv
 */
void configure(int argc,char** argv)
{
    variables_map options;
    store(parse_command_line(argc,argv,tx.m_options),options);
    notify(options);
    for(auto* m : tx.m_module)
    {
        m->configure(options);
    }
    size_t shard_count = 1;
    auto* s = new shard();
    s->configure(options);
    shard::current_shard = s;
    tx.m_shards[0] = s;
    for(auto* m : tx.m_module)
    {
        m->configure_for(s);
    }
    for(size_t i = 1 ; i < shard_count ; i++)
    {
        s = new shard();
        s->configure(options);
        auto* t = new std::thread([&](){
            shard::current_shard = s;
            for(auto* m : tx.m_module)
            {
                m->configure_for(s);
            }
            auto* current_scheduler = s->get_scheduler();
            current_scheduler->operator()();
        });
        tx.m_threads[i] = t;
        tx.m_shards[i] = s;
    }
    tx.m_shard_count = shard_count;
}

void run()
{
    shard *main_shard = tx.m_shards[0];
    auto* main_scheduler = main_shard->get_scheduler();
	int ret = 0;
    spawn([&](){
        auto *ci = spawn([&](){
            ret = txmain(tx.argc,tx.argv);
        });
        wait(ci);
        main_scheduler->stop();
    });
    main_scheduler->operator()();
}


/**
 *  退出框架清理资源
 */
void exit()
{

}


void register_module(shard_module *sm)
{
    tx.m_module.push_back(sm);
    sm->register_options(tx.m_options);
}

}