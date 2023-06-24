#ifndef ANDROMEDA_TMP_TYPES
#define ANDROMEDA_TMP_TYPES

#include <functional>
#include <type_traits>
#include "Logic.h"

namespace andromeda {
	namespace tmp {
		//判断两个类型是否相同
		template<typename T1,typename T2>
		struct type_equal
		{
			static const bool result=false;
		};

		template<typename T1>
		struct type_equal<T1,T1>
		{
			static const bool result=true;
		};
		//得到普通函数或成员函数的返回类型，Func必须为普通函数，Class为函数所属类(实际上是依靠Func判断是否为成员函数，只有Func为成员函数时Class参数才有用)，Args必须显示指定。可以配合bind_this使用，用于取得RetType
		template<bool IsMemFunc,typename Class,typename Func,typename ...Args>
		struct __get_func_ret_type_impl
		{
		};

		template<typename Class,typename Func,typename ...Args>
		struct __get_func_ret_type_impl<true,Class,Func,Args...>
		{
			typedef decltype((std::declval<Class>().*std::declval<Func>())(std::declval<Args>()...)) result_type;
		};

		template<typename Class,typename Func,typename ...Args>
		struct __get_func_ret_type_impl<false,Class,Func,Args...>
		{
			typedef decltype(std::function<Func>()(std::declval<Args>()...)) result_type;
		};

		template<typename Class,typename Func,typename ...Args>
		struct get_func_ret_type:public __get_func_ret_type_impl<std::is_member_function_pointer<Func>::value,Class,Func,Args...>
		{
			using __get_func_ret_type_impl<std::is_member_function_pointer<Func>::value,Class,Func,Args...>::result_type;
		};

		//得到函数指针的类型，Class=void则视作普通函数类型，否则视作成员函数类型，函数类型转函数指针使用std::decay
		template<typename Class,typename RetType,typename ... ArgsType>
		struct func_type
		{
			typedef typename _if<type_equal<Class,void>::result,RetType (*)(ArgsType...),RetType (_if<type_equal<Class,void>::result,func_type<Class,RetType,ArgsType...>,Class>::result_type::*)(ArgsType...)>::result_type result_type;
		};

		//得到绑定了cls=this的Class成员函数std::function，如果Class=void则不绑定this，指针视作普通函数指针。用法 bind_this<>::bind(func,cls)
		template<typename Class,typename RetType,typename ... ArgsType>
		struct bind_this
		{
			std::function<RetType(ArgsType...)> operator()(typename func_type<Class,RetType,ArgsType...>::result_type func,Class* cls)
			{
				return [=](ArgsType&&... args)
				{
					return (cls->*func)(std::forward<decltype(args)>(args)...);
				};
			}

			static bind_this<Class,RetType,ArgsType...>bind;
		};

		template<typename Class,typename RetType,typename ... ArgsType>
		bind_this<Class,RetType,ArgsType...>bind_this<Class,RetType,ArgsType...>::bind=bind_this<Class,RetType,ArgsType...>();

		template<typename RetType,typename ... ArgsType>
		struct bind_this<void,RetType,ArgsType...>
		{
			std::function<RetType(ArgsType...)> operator()(typename func_type<void,RetType,ArgsType...>::result_type func,void* cls=nullptr)
			{
				return [=](ArgsType&&... args)
				{
					return (*func)(std::forward<decltype(args)>(args)...);
				};
			}

			static bind_this<void,RetType,ArgsType...>bind;
		};

		template<typename RetType,typename ... ArgsType>
		bind_this<void,RetType,ArgsType...>bind_this<void,RetType,ArgsType...>::bind=bind_this<void,RetType,ArgsType...>();

		//std::decay的拓展，可以得到指针、引用等的原本类型，即传入T*或T**甚至更高次数的指针可以得到T
		template<bool IsPtr,typename T>
		struct __get_type_impl
		{
		};

		template<typename T>
		struct __get_type_impl<true,T*>
		{
			typedef typename __get_type_impl<std::is_pointer<typename std::decay<T>::type>::value,typename std::decay<T>::type>::result_type result_type;
		};

		template<typename T>
		struct __get_type_impl<false,T>
		{
			typedef typename std::decay<T>::type result_type;
		};

		template<typename T>
		struct get_type:public __get_type_impl<std::is_pointer<typename std::decay<T>::type>::value,typename std::decay<T>::type>
		{
			typedef typename __get_type_impl<std::is_pointer<typename std::decay<T>::type>::value,typename std::decay<T>::type>::result_type result_type;
		};

		//判断是否是基础类型，如果是则结果为true，如果不是（是类）则返回false。可以传入指针、引用等，得到的依旧是原本类型
		template<typename T>
		struct __is_basic_type_impl
		{
			static const bool result=false;
		};

#define	IS_BASIC_TYPE(T)	template<>\
							struct __is_basic_type_impl<T>\
							{\
								static const bool result=true;\
							};
		//8位
		IS_BASIC_TYPE(char)
		IS_BASIC_TYPE(unsigned char)
		//16位
		IS_BASIC_TYPE(short int)
		IS_BASIC_TYPE(unsigned short int)
		//32位
		IS_BASIC_TYPE(int)
		IS_BASIC_TYPE(unsigned int)
		//32位
		IS_BASIC_TYPE(long int)
		IS_BASIC_TYPE(unsigned long int)
		//64位
		IS_BASIC_TYPE(long long int)
		IS_BASIC_TYPE(unsigned long long int)
		//32位
		IS_BASIC_TYPE(float)
		//64位
		IS_BASIC_TYPE(double)
		//64位
		IS_BASIC_TYPE(long double)
		//32位
		IS_BASIC_TYPE(bool)
		//void类型
		IS_BASIC_TYPE(void)

#undef IS_BASIC_TYPE

		template<typename T>
		struct is_basic_type:public __is_basic_type_impl<typename get_type<T>::result_type>
		{
			using __is_basic_type_impl<typename get_type<T>::result_type>::result;
		};
		//判断T是否是类，结果实际上为!(is_basic_type<T>)
		template<typename T>
		struct is_class
		{
			static const bool result=!is_basic_type<T>::result;
		};
	}
}

#endif//ANDROMEDA_TMP_TYPES