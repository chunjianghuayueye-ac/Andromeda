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
			int available_length,extend_capcity,last_element_idx;
			E* elements;
		public:
			ArrayList() :
					available_length(0), last_element_idx(-1), extend_capcity(32), elements(nullptr)
			{
			}

			ArrayList(int arr_length) :
					available_length(arr_length), last_element_idx(-1), extend_capcity(arr_length), elements((E*)malloc(sizeof(E)*arr_length))
			{
			}

			ArrayList(int arr_length,int arr_extend_capcity) :
					available_length(arr_length), last_element_idx(-1), extend_capcity(arr_extend_capcity), elements((E*)malloc(sizeof(E)*arr_length))
			{
			}

			ArrayList(ArrayList<E>& arr) :
					available_length(arr.available_length), last_element_idx(arr.last_element_idx), extend_capcity(arr.extend_capcity), elements((E*)malloc(sizeof(E)*arr.available_length))
			{
				memcpy(elements,arr.elements,sizeof(E)*arr.available_length);
			}

			ArrayList(ArrayList<E>&& arr) :
					available_length(arr.available_length), last_element_idx(arr.last_element_idx), extend_capcity(arr.extend_capcity), elements(arr.elements)
			{
				arr.elements=nullptr;
			}

			~ArrayList()
			{
				free(elements);
			}

			void operator=(ArrayList& arr)
			{
				available_length=arr.available_length;
				last_element_idx=arr.last_element_idx;
				extend_capcity=arr.extend_capcity;
				elements=(E*)malloc(sizeof(E)*arr.available_length);
				memcpy(elements,arr.elements,sizeof(E)*arr.available_length);
			}

			void operator=(ArrayList&& arr)
			{
				available_length=arr.available_length;
				last_element_idx=arr.last_element_idx;
				extend_capcity=arr.extend_capcity;
				elements=arr.elements;
				arr.elements=nullptr;
			}

			void add(E& e)
			{
				if(last_element_idx>=available_length-1)
					extendArray(extend_capcity);
				*(elements+(++last_element_idx))=e;
			}

			void add(E&& e)
			{
				if(last_element_idx>=available_length-1)
					extendArray(extend_capcity);
				*(elements+(++last_element_idx))=e;
			}

			void add(E* es,int num)
			{
				START_EXTEND:if(last_element_idx+num>=available_length)
				{
					extendArray(extend_capcity);
					goto START_EXTEND;
				}
				memcpy(elements+last_element_idx,es,sizeof(E)*num);
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
				--last_element_idx;
				return *(elements+last_element_idx+1);
			}

			E* remove(int num)
			{
				last_element_idx-=num;
				return elements+last_element_idx+1;
			}

			E& getLastElement()
			{
				return *(elements+last_element_idx);
			}

			E& get(int index)
			{
				return *(elements+index);
			}

			E& operator[](int index)
			{
				return *(elements+index);
			}

			inline int getAvailableLength() //数组可使用的总长度，包括没有赋值的元素
			{
				return available_length;
			}

			inline int getLastIndex()
			{
				return last_element_idx;
			}

			inline int getLength()//数组实际使用的长度
			{
				return last_element_idx+1;
			}

			inline int getExtendCapcity()
			{
				return extend_capcity;
			}

			inline E* getElements()
			{
				return elements;
			}
			//手动释放内存，调用后就不应该再对该对象进行任何操作，否则会出错
			inline void release()
			{
				free(elements);
			}
			//将数据类型强制转换为指定类型
			template<typename T>
			inline ArrayList<T>& cast()
			{
				return *(ArrayList<T>*)this;
			}
		protected:

			void extendArray(int extend_cap)
			{
				E* tmp=(E*)malloc(sizeof(E)*(available_length+extend_cap));
				memcpy(tmp,elements,sizeof(E)*available_length);
				free(elements);
				available_length+=extend_cap;
				elements=tmp;
			}
		};
	}
}

#endif // ANDROMEDA_UTIL_ARRAYLIST
