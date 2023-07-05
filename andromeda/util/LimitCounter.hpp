#ifndef ANDROMEDA_UTIL_LIMITCOUNTER
#define ANDROMEDA_UTIL_LIMITCOUNTER

namespace andromeda {
	namespace util {
		//������ʼֵ����ֵֹ����ѭ�����У����������Լ�������ֵ����������С�ֻ�����ڻ�����������
		template<typename T>
		class LimitCounter
		{
		private:
			T _start;
			T _end;
			T counter;
		public:
			LimitCounter(T start_value,T end_value) :
					_start(start_value), _end(end_value)
			{
				counter=start_value;
			}

			__attribute__((always_inline)) inline operator T&()
			{
				return counter;
			}

			__attribute__((always_inline)) inline T& inc() //���������Ľ��ֵ
			{
				if(++counter>_end)
					counter=_start; //���Խ��_end��ص�_start
				return counter;
			}

			__attribute__((always_inline)) inline T& dec() //�����Լ��Ľ��ֵ
			{
				if(--counter<_start)
					counter=_end;
				return counter;
			}

			T& operator++() //ǰ��++
			{
				return counter.inc();
			}

			T& operator--()
			{
				return counter.dec();
			}

			T operator++(int) //����++
			{
				T old_counter=counter;
				if(++counter>_end)
					counter=_start; //���Խ��_end��ص�_start
				return old_counter;
			}

			T operator--(int)
			{
				T old_counter=counter;
				if(--counter<_start)
					counter=_end;
				return old_counter;
			}
		};
	}
}

#endif//ANDROMEDA_UTIL_LIMITCOUNTER
