#ifndef ANDROMEDA_UTIL_THREADTURN
#define ANDROMEDA_UTIL_THREADTURN

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace andromeda {
	namespace util {
		class ThreadTurn //�߳��ֻ�ִ�еĸ����࣬A�߳̿�ʼִ��ʱB�߳���ͣ��A�߳���ͣʱB�ָ�ִ�У�ֻ���������̴߳����ֻ���ϵ
		{
		private:
			std::atomic_bool _is_running;
			std::mutex _mutex;
			std::condition_variable _condition;

		public:
			void turn(ThreadTurn* turn); //������һ���̵߳�ThreadTurn�����������øú������̣߳��ָ�ִ�д�����߳�
		};
	}
}

#endif//ANDROMEDA_UTIL_THREADTURN
