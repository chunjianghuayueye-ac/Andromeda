#include "FrameRate.hpp"

#include <thread>

using namespace andromeda::app;
using namespace std::chrono;

void FrameRate::init()
{
	delta_t=0;
	tpf=0;
	fps=0;
	last_time=high_resolution_clock::now();
}

void FrameRate::set_fps_limit(int _fps_limit)
{
	fps_limit=_fps_limit;
	if(_fps_limit<0)
		limit_fps=false;
	else
	{
		limit_fps=true;
		tpf_limit=1.0/fps_limit; //只要fps_limit>0，则计算每帧时间
	}
}

void FrameRate::calc()
{
	++fps;
	this_time=high_resolution_clock::now();
	tpf=HRC_TIME_DURATION_TO_SEC(this_time-last_time);
	if(limit_fps)
	{
		if(fps_limit)//fps_limit>0时根据耗时情况休眠
		{
			double sleep_time=tpf_limit-tpf;
			if(sleep_time>0)
				std::this_thread::sleep_for(nanoseconds(sleep_time*1E9));
		}
		else
			std::this_thread::suspend(); //fps限制为0则挂起线程
	}
	delta_t+=tpf;
	if(delta_t>=1)
	{
		--delta_t;
		fps=0;
	}
	last_time=this_time;
}
