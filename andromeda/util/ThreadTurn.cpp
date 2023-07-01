#include "ThreadTurn.hpp"

using namespace andromeda::util;

void ThreadTurn::turn(ThreadTurn* turn) //������һ���̵߳�ThreadTurn�����������øú������̣߳��ָ�ִ�д�����߳�
{
	std::unique_lock<std::mutex> lock(_mutex);
	turn->_is_running=true;
	_is_running=false;
	turn->_condition.notify_all(); //֪ͨ�����̣߳�����һ���̣߳�
	while(!_is_running)
		_condition.wait(lock); //�������̣߳�ֱ������ThreadTurn�������turn(������)�ſ��Իָ�ִ��
}
