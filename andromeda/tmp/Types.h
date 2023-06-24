#ifndef ANDROMEDA_TMP_TYPES
#define ANDROMEDA_TMP_TYPES

#include <functional>
#include <type_traits>
#include "Logic.h"

namespace andromeda {
	namespace tmp {
		//�ж����������Ƿ���ͬ
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
		//�õ���ͨ�������Ա�����ķ������ͣ�Func����Ϊ��ͨ������ClassΪ����������(ʵ����������Func�ж��Ƿ�Ϊ��Ա������ֻ��FuncΪ��Ա����ʱClass����������)��Args������ʾָ�����������bind_thisʹ�ã�����ȡ��RetType
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

		//�õ�����ָ������ͣ�Class=void��������ͨ�������ͣ�����������Ա�������ͣ���������ת����ָ��ʹ��std::decay
		template<typename Class,typename RetType,typename ... ArgsType>
		struct func_type
		{
			typedef typename _if<type_equal<Class,void>::result,RetType (*)(ArgsType...),RetType (_if<type_equal<Class,void>::result,func_type<Class,RetType,ArgsType...>,Class>::result_type::*)(ArgsType...)>::result_type result_type;
		};

		//�õ�����cls=this��Class��Ա����std::function�����Class=void�򲻰�this��ָ��������ͨ����ָ�롣�÷� bind_this<>::bind(func,cls)
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

	}
}

#endif//ANDROMEDA_TMP_TYPES