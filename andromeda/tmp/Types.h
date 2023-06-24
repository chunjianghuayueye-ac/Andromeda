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
		//����Ա����ָ���˻�Ϊ��Ч����ͨ����ָ��
		template<typename Func>
		struct __degenerate_func_impl
		{
			typedef Func result_type;
			typedef void class_type;
		};

		template<typename Class,typename Func>
		struct __degenerate_func_impl<Func Class::*>
		{
			typedef Func result_type;
			typedef Class class_type;
		};

		template<typename Func>
		struct degenerate_func:public __degenerate_func_impl<Func>
		{
			using __degenerate_func_impl<Func>::result_type;
			using __degenerate_func_impl<Func>::class_type;
		};
		//�õ���ͨ�������Ա�����ķ������ͣ�Func����Ϊ��ͨ������ClassΪ����������(ʵ����������Func�ж��Ƿ�Ϊ��Ա������ֻ��FuncΪ��Ա����ʱClass����������)��Args������ʾָ�����������bind_thisʹ�ã�����ȡ��RetType
		//�����Ա����ָ������ʱ����д��&Class::Func��������д��ClassObj.Func����Ϊ����������������ظ����Func����������decltype�޷��ƶϳ�Ա���������͡�������ģ����ã���Ա�����������ⲿ���Ե��õĺ���������������Ϊprivate��protected
		template<typename Func,typename ...Args>
		struct __get_func_ret_type_impl
		{
			typedef decltype(std::declval<std::function<Func> >()(std::declval<Args>()...)) result_type;
			typedef void class_type;//������������
		};

		template<typename Class,typename Func,typename ...Args>
		struct __get_func_ret_type_impl<Func Class::*,Args...>
		{
			typedef decltype((std::declval<Class>().*std::declval<Func Class::*>())(std::declval<Args>()...)) result_type;
			typedef Class class_type;//������������
		};

		template<typename Func,typename ...Args>
		struct get_func_ret_type:public __get_func_ret_type_impl<Func,Args...>
		{
			using __get_func_ret_type_impl<Func,Args...>::result_type;
			using __get_func_ret_type_impl<Func,Args...>::class_type;
		};

		//��װ����ֵ���������������Եõ�����ָ������ͣ�Class=void��������ͨ�������ͣ�����������Ա�������ͣ���������ת����ָ��ʹ��std::decay
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

		//std::decay����չ�����Եõ�ָ�롢���õȵ�ԭ�����ͣ�������T*��T**�������ߴ�����ָ����Եõ�T
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

		//�ж��Ƿ��ǻ������ͣ����������Ϊtrue��������ǣ����ࣩ�򷵻�false�����Դ���ָ�롢���õȣ��õ���������ԭ������
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
		//8λ
		IS_BASIC_TYPE(char)
		IS_BASIC_TYPE(unsigned char)
		//16λ
		IS_BASIC_TYPE(short int)
		IS_BASIC_TYPE(unsigned short int)
		//32λ
		IS_BASIC_TYPE(int)
		IS_BASIC_TYPE(unsigned int)
		//32λ
		IS_BASIC_TYPE(long int)
		IS_BASIC_TYPE(unsigned long int)
		//64λ
		IS_BASIC_TYPE(long long int)
		IS_BASIC_TYPE(unsigned long long int)
		//32λ
		IS_BASIC_TYPE(float)
		//64λ
		IS_BASIC_TYPE(double)
		//64λ
		IS_BASIC_TYPE(long double)
		//32λ
		IS_BASIC_TYPE(bool)
		//void����
		IS_BASIC_TYPE(void)

#undef IS_BASIC_TYPE

		template<typename T>
		struct is_basic_type:public __is_basic_type_impl<typename get_type<T>::result_type>
		{
			using __is_basic_type_impl<typename get_type<T>::result_type>::result;
		};
		//�ж�T�Ƿ����࣬���ʵ����Ϊ!(is_basic_type<T>)
		template<typename T>
		struct is_class
		{
			static const bool result=!is_basic_type<T>::result;
		};
	}
}

#endif//ANDROMEDA_TMP_TYPES