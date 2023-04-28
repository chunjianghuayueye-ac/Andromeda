#ifndef ANDROMEDA_MACROS_DEBUG
#define ANDROMEDA_MACROS_DEBUG
//���������صĺ꣬�������������Ϣ�ķ�ʽ

#define DEBUG_CMD

#ifdef DEBUG_CMD

#include <iostream>
#include "../util/Utils.hpp"

#define PRINT_MESSAGE(...) {std::cout<<(andromeda::util::str_join<>(__VA_ARGS__))<<std::endl;}

#endif

#endif//ANDROMEDA_MACROS_DEBUG
