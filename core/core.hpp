/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: core
* Author:   aiyong
* Date:     2018/10/19 00:12
*/

#include <boost/program_options.hpp>

namespace tianxing {

using namespace boost::program_options;

class shard_module;

/**
 * 配置框架
 * @param argc
 * @param argv
 */
void configure(int argc,char** argv);


/**
 * 启动主协成
 * @tparam Fn
 * @param fn
 */

void run();


/**
 *  退出框架清理资源
 */
void exit();


/**
 *
 * @param sm
 */
void register_module(shard_module *sm);


}