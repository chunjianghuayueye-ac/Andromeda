#ifndef ANDROMEDA_MACROS_DEBUG
#define ANDROMEDA_MACROS_DEBUG
//���������صĺ꣬�������������Ϣ�ķ�ʽ

#include <iostream>
#include "../util/Utils.hpp"

#define PRINT_MESSAGE(m...) {std::cout<<(andromeda::util::str_join(m...))<<std::endl;}

#endif//ANDROMEDA_MACROS_DEBUG
