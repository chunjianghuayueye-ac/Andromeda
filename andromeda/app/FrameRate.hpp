#ifndef ANDROMEDA_APP_FRAMERATE
#define ANDROMEDA_APP_FRAMERATE

#include <chrono>
//�߾���ʱ�ӵ�time_pointת������
#define HRC_TIME_DURATION_TO_SEC(delta_time) ((double)std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time).count()/1E9)

namespace andromeda {
	namespace app {
		class FrameRate
		{
		public:
			typedef std::chrono::time_point<std::chrono::high_resolution_clock> high_resolution_time_point; //���õĸ߾���ʱ�������

		private:
			high_resolution_time_point this_time,last_time;
			double time_unit;
			double delta_t=0,tpf=0,tpf_limit=-1;
			bool limit_fps=false;
			int fps=0,fps_count=0,fps_limit=-1; //fps_limitΪ���֡�����ƣ�<=0��ʾ�����ƣ����ڵ���calc()ʱ�����֡��ʱ�䲻��1/fps_limit�����sleep���߳�ֱ����֡������ռ�ôﵽ1/fps_limit��
		public:

			void init(); //��ʼ�����ã�ֻ����һ�Ρ��ٴε������൱������ʱ�ӻع�ɵ�һ�ε���init()���״̬
			void calc(); //����ѭ����ִ�и��²�����Ϻ���ã���������ϴε��øú�������init()���ú󣩾�����ʱ��tpf��ÿ����һ��fps+1��delta_t�ۼ�һ���fps����

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
