#include "ThreadTurn.hpp"

using namespace andromeda::util;

void ThreadTurn::turn(ThreadTurn* turn) //传入另一个线程的ThreadTurn对象，阻塞调用该函数的线程，恢复执行传入的线程
{
	std::unique_lock<std::mutex> lock(_mutex);
	turn->_is_running=true;
	_is_running=false;
	turn->_condition.notify_all(); //通知所有线程（即另一个线程）
	while(!_is_running)
		_condition.wait(lock); //阻塞该线程，直到其他ThreadTurn对象调用turn(本对象)才可以恢复执行
}
