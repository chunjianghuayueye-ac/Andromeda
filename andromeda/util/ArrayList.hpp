#ifndef ANDROMEDA_UTIL_ARRAYLIST
#define ANDROMEDA_UTIL_ARRAYLIST

#include <malloc.h>
#include <string.h>

namespace andromeda {
	namespace util {
		template<typename E>
		class ArrayList
		{
		protected:
			int length,extend_capcity,last;
			E* elements;
		public:
			ArrayList() :
					length(0), last(-1), extend_capcity(32), elements(nullptr)
			{
			}

			ArrayList(int arr_length) :
					length(arr_length), last(-1), extend_capcity(arr_length), elements((E*)malloc(sizeof(E)*arr_length))
			{
			}

			ArrayList(int arr_length,int arr_extend_capcity) :
					length(arr_length), last(-1), extend_capcity(arr_extend_capcity), elements((E*)malloc(sizeof(E)*arr_length))
			{
			}

			ArrayList(ArrayList<E>& arr) :
					length(arr.length), last(arr.last), extend_capcity(arr.extend_capcity), elements((E*)malloc(sizeof(E)*arr.length))
			{
				memcpy(elements,arr.elements,sizeof(E)*arr.length);
			}

			ArrayList(ArrayList<E>&& arr) :
					length(arr.length), last(arr.last), extend_capcity(arr.extend_capcity), elements(arr.elements)
			{
				arr.elements=nullptr;
			}

			~ArrayList()
			{
				free(elements);
			}

			void operator=(ArrayList& arr)
			{
				length=arr.length;
				last=arr.last;
				extend_capcity=arr.extend_capcity;
				elements=(E*)malloc(sizeof(E)*arr.length);
				memcpy(elements,arr.elements,sizeof(E)*arr.length);
			}

			void operator=(ArrayList&& arr)
			{
				length=arr.length;
				last=arr.last;
				extend_capcity=arr.extend_capcity;
				elements=arr.elements;
				arr.elements=nullptr;
			}

			void add(E& e)
			{
				if(last>=length-1)
					extendArray(extend_capcity);
				*(elements+(last++))=e;
			}

			void add(E&& e)
			{
				if(last>=length-1)
					extendArray(extend_capcity);
				*(elements+(last++))=e;
			}

			void add(E* es,int num)
			{
				START_EXTEND:if(last+num>=length)
				{
					extendArray(extend_capcity);
					goto START_EXTEND;
				}
				memcpy(elements+last,es,sizeof(E)*num);
			}

			void add(E& e1,E& e2)
			{
				add(e1);
				add(e2);
			}

			template<typename ...Es>
			void add(E& e,Es ...es)
			{
				add(e);
				add(es...);
			}

			void add(ArrayList& arr_lst)
			{
				add(arr_lst.getElements(),arr_lst.getLastIndex());
			}

			void add(ArrayList&& arr_lst)
			{
				add(arr_lst.getElements(),arr_lst.getLastIndex());
			}

			E& remove()
			{
				--last;
				return *(elements+last+1);
			}

			E* remove(int num)
			{
				last-=num;
				return elements+last+1;
			}

			E& getLastElement()
			{
				return *(elements+last);
			}

			E& get(int index)
			{
				return *(elements+index);
			}

			E& operator[](int index)
			{
				return *(elements+index);
			}

			inline int getAvailableLength() //�����ʹ�õ��ܳ��ȣ�����û�и�ֵ��Ԫ��
			{
				return length;
			}

			inline int getLastIndex()
			{
				return last;
			}

			inline int getLength()//����ʵ��ʹ�õĳ���
			{
				return last+1;
			}

			inline int getExtendCapcity()
			{
				return extend_capcity;
			}

			inline E* getElements()
			{
				return elements;
			}
			//�ֶ��ͷ��ڴ棬���ú�Ͳ�Ӧ���ٶԸö�������κβ�������������
			inline void release()
			{
				free(elements);
			}
			//����������ǿ��ת��Ϊָ������
			template<typename T>
			inline ArrayList<T>& cast()
			{
				return *(ArrayList<T>*)this;
			}
		protected:

			void extendArray(int extend_cap)
			{
				E* tmp=(E*)malloc(sizeof(E)*(length+extend_cap));
				memcpy(tmp,elements,sizeof(E)*length);
				free(elements);
				length+=extend_cap;
				elements=tmp;
			}
		};
	}
}

#endif // ANDROMEDA_UTIL_ARRAYLIST
