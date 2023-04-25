#ifndef ANDROMEDA_MACROS_DEBUG
#define ANDROMEDA_MACROS_DEBUG
//定义调试相关的宏，例如输出报错信息的方式

#include <iostream>
#include "../util/Utils.hpp"

#define PRINT_MESSAGE(m...) {std::cout<<(andromeda::util::str_join(m...))<<std::endl;}

#endif//ANDROMEDA_MACROS_DEBUG
