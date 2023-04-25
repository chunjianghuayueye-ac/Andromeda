#ifndef ANDROMEDA_APP_FRAMERATE
#define ANDROMEDA_APP_FRAMERATE

#include <chrono>
//高精度时钟的time_point转换成秒
#define HRC_TIME_DURATION_TO_SEC(delta_time) ((double)std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time).count()/1E9)

namespace andromeda {
	namespace app {
		class FrameRate
		{
		public:
			typedef std::chrono::time_point<std::chrono::high_resolution_clock> high_resolution_time_point; //采用的高精度时间点类型

		private:
			high_resolution_time_point this_time,last_time;
			double time_unit;
			double delta_t=0,tpf=0,tpf_limit=-1;
			bool limit_fps=false;
			int fps=0,fps_limit=-1; //fps_limit为最大帧率限制（<0表示不限制，=0将会把该线程挂起，需要通过异步操作resume()恢复），在调用calc()时如果单帧的时间不足1/fps_limit秒则会sleep该线程直到该帧的秒数占用达到1/fps_limit秒
		public:

			void init(); //初始化调用，只调用一次。再次调用则相当于重置时钟回归成第一次调用init()后的状态
			void calc(); //在主循环里执行更新操作完毕后调用，计算距离上次调用该函数（或init()调用后）经过的时间tpf，每调用一次fps+1。delta_t累计一秒后fps置零

			__attribute__((always_inline)) inline double get_tpf()
			{
				return tpf;
			}

			__attribute__((always_inline)) inline int get_fps()
			{
				return fps;
			}

			void set_fps_limit(int _fps_limit);
		};
	}
}

#endif//ANDROMEDA_APP_FRAMERATE
