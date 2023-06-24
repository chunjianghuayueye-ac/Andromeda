#ifndef ANDROMEDA_TMP_LOGIC
#define ANDROMEDA_TMP_LOGIC

namespace andromeda {
	namespace tmp {
		//�ж�Cond�Ƿ�Ϊtrue���������result_type����ΪTrue������ΪFalse
		template<bool Cond,typename True,typename False>
		struct _if
		{
		};

		template<typename True,typename False>
		struct _if<true,True,False>
		{
			typedef True result_type;
		};

		template<typename True,typename False>
		struct _if<false,True,False>
		{
			typedef False result_type;
		};
	}
}

#endif//ANDROMEDA_TMP_LOGIC
