#ifndef ANDROMEDA_MATH_MATRIX
#define ANDROMEDA_MATH_MATRIX

#include <type_traits>
#include <math.h>
#include <malloc.h>
#include <sstream>
#include "Vector.hpp"

namespace andromeda {
	namespace math {
		template<int Row,int Column,typename T=float>
		class Matrix
		{
		public:
			Vector<Column,T>elem[Row]; //�����С��С�����Row��Columnά������
			typedef T type;
			static const int row=Row;
			static const int column=Column;

			inline operator T*()
			{
				return (T*)&elem;
			}

			template<typename T2>
			inline operator Matrix<Row,Column,T2>()
			{
				Matrix<Row,Column,T2>cast_result;
				for(int i=0;i<Row;++i)
					cast_result[i]=elem[i];
				return cast_result;
			}

			inline Vector<Column,T>& operator[](int i)
			{
				return elem[i];
			}

			inline operator std::string() //ת��Ϊ�ַ���
			{
				return toString();
			}

			inline operator const char*() //ת��ΪC�ַ���
			{
				return toString().c_str();
			}

			template<typename T2>
			inline Matrix<Row,Column,decltype(std::declval<T>()+std::declval<T2>())>operator +(Matrix<Row,Column,T2>m)
			{
				return add(m);
			}

			template<typename T2>
			inline Matrix<Row,Column,decltype(std::declval<T>()-std::declval<T2>())>operator -(Matrix<Row,Column,T2>m)
			{
				return sub(m);
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>operator *(T2 t)
			{
				return mul(t);
			}

			template<int Row2,typename T2>
			Matrix<Row,Row2,decltype(std::declval<T>()*std::declval<T2>())>operator *(Matrix<Column,Row2,T2>m)
			{
				return mul(m);
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()/std::declval<T2>())>operator /(T2 t)
			{
				return div(t);
			}

			Vector<Row,T>extractColumn(int column)
			{
				Vector<Row,T>row_vec;
				for(int i=0;i<Row;++i)
					row_vec[i]=elem[i][column];
				return row_vec;
			}

			inline Vector<Column,T>extractRow(int row)
			{
				return elem[row];
			}

			Matrix<Column,Row,T>transpose() //ת��
			{
				Matrix<Column,Row,T>transpose_result;
				for(int i=0;i<Column;++i)
					for(int j=0;j<Row;++j)
						transpose_result[i][j]=elem[j][i];
				return transpose_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>dot(Matrix<Row,Column,T2>m) //��λ���
			{
				Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>dot_result;
				for(int i=0;i<Row;++i)
					for(int j=0;j<Column;++j)
						dot_result[i][j]=elem[i][j]*m.elem[i][j];
				return dot_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>mul(T2 t) //����
			{
				Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>mul_result;
				for(int i=0;i<Row;++i)
					mul_result[i]=elem[i]*t;
				return mul_result;
			}

			template<int Row2,typename T2>
			Matrix<Row,Row2,decltype(std::declval<T>()*std::declval<T2>())>mul(Matrix<Column,Row2,T2>m)
			{
				Matrix<Row,Row2,decltype(std::declval<T>()*std::declval<T2>())>mul_result;
				for(int i=0;i<Row;++i)
				{
					Vector<Column,T>row_vec=extractRow(i);
					for(int j=0;j<Row2;++j)
						mul_result[i][j]=row_vec.dot(m.extractColumn(j));
				}
				return mul_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()/std::declval<T2>())>div(T2 t) //����
			{
				Matrix<Row,Column,decltype(std::declval<T>()/std::declval<T2>())>div_result;
				for(int i=0;i<Row;++i)
					div_result[i]=elem[i]/t;
				return div_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()+std::declval<T2>())>add(Matrix<Row,Column,T2>m)
			{
				Matrix<Row,Column,decltype(std::declval<T>()+std::declval<T2>())>add_result;
				for(int i=0;i<Row;++i)
					add_result[i]=elem[i]+m.elem[i];
				return add_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()-std::declval<T2>())>sub(Matrix<Row,Column,T2>m)
			{
				Matrix<Row,Column,decltype(std::declval<T>()-std::declval<T2>())>sub_result;
				for(int i=0;i<Row;++i)
					sub_result[i]=elem[i]-m.elem[i];
				return sub_result;
			}

			static Matrix<Row,Column,T>zero()
			{
				Matrix<Row,Column,T>zero_mat;
				memset(&zero_mat.elem,0,Row*Column*sizeof(T));
				return zero_mat;
			}

			std::string toString()
			{
				std::ostringstream oss;
				for(int i=0;i<Row;++i)
				{
					oss<<elem[i][0];
					for(int j=1;j<Column;++j)
						oss<<' '<<elem[i][j];
					oss<<'\n';
				}
				return oss.str();
			}
		};
		//�����ػ�
		template<int Rank,typename T>
		class Matrix<Rank,Rank,T>
		{
		public:
			Vector<Rank,T>elem[Rank]; //�����С��С�����Rank��Rankά������
			typedef T type;
			static const int row=Rank;
			static const int column=Rank;

			inline operator T*()
			{
				return (T*)&elem;
			}

			template<typename T2>
			inline operator Matrix<Rank,Rank,T2>()
			{
				Matrix<Rank,Rank,T2>cast_result;
				for(int i=0;i<Rank;++i)
					cast_result[i]=elem[i];
				return cast_result;
			}

			inline Vector<Rank,T>& operator[](int i)
			{
				return elem[i];
			}

			inline operator std::string() //ת��Ϊ�ַ���
			{
				return toString();
			}

			inline operator const char*() //ת��ΪC�ַ���
			{
				return toString().c_str();
			}

			template<typename T2>
			inline Matrix<Rank,Rank,decltype(std::declval<T>()+std::declval<T2>())>operator +(Matrix<Rank,Rank,T2>m)
			{
				return add(m);
			}

			template<typename T2>
			inline Matrix<Rank,Rank,decltype(std::declval<T>()-std::declval<T2>())>operator -(Matrix<Rank,Rank,T2>m)
			{
				return sub(m);
			}

			template<typename T2>
			Matrix<Rank,Rank,decltype(std::declval<T>()*std::declval<T2>())>operator *(T2 t)
			{
				return mul(t);
			}

			template<typename T2>
			Matrix<Rank,Rank,decltype(std::declval<T>()*std::declval<T2>())>operator *(Matrix<Rank,Rank,T2>m)
			{
				return mul(m);
			}

			template<typename T2>
			Matrix<Rank,Rank,decltype(std::declval<T>()/std::declval<T2>())>operator /(T2 t)
			{
				return div(t);
			}

			Vector<Rank,T>extractColumn(int column)
			{
				Vector<Rank,T>row_vec;
				for(int i=0;i<Rank;++i)
					row_vec[i]=elem[i][column];
				return row_vec;
			}

			inline Vector<Rank,T>extractRow(int row)
			{
				return elem[row];
			}

			Matrix<Rank,Rank,T>transpose() //ת��
			{
				Matrix<Rank,Rank,T>transpose_result;
				for(int i=0;i<Rank;++i)
					for(int j=0;j<Rank;++j)
						transpose_result[i][j]=elem[j][i];
				return transpose_result;
			}

			template<typename T2>
			Matrix<Rank,Rank,decltype(std::declval<T>()*std::declval<T2>())>dot(Matrix<Rank,Rank,T2>m) //��λ���
			{
				Matrix<Rank,Rank,decltype(std::declval<T>()*std::declval<T2>())>dot_result;
				for(int i=0;i<Rank;++i)
					for(int j=0;j<Rank;++j)
						dot_result[i][j]=elem[i][j]*m.elem[i][j];
				return dot_result;
			}

			template<typename T2>
			Matrix<Rank,Rank,decltype(std::declval<T>()*std::declval<T2>())>mul(T2 t) //����
			{
				Matrix<Rank,Rank,decltype(std::declval<T>()*std::declval<T2>())>mul_result;
				for(int i=0;i<Rank;++i)
					mul_result[i]=elem[i]*t;
				return mul_result;
			}

			template<typename T2>
			Matrix<Rank,Rank,decltype(std::declval<T>()*std::declval<T2>())>mul(Matrix<Rank,Rank,T2>m)
			{
				Matrix<Rank,Rank,decltype(std::declval<T>()*std::declval<T2>())>mul_result;
				for(int i=0;i<Rank;++i)
				{
					Vector<Rank,T>row_vec=extractRow(i);
					for(int j=0;j<Rank;++j)
						mul_result[i][j]=row_vec.dot(m.extractColumn(j));
				}
				return mul_result;
			}

			template<typename T2>
			Matrix<Rank,Rank,decltype(std::declval<T>()/std::declval<T2>())>div(T2 t) //����
			{
				Matrix<Rank,Rank,decltype(std::declval<T>()/std::declval<T2>())>div_result;
				for(int i=0;i<Rank;++i)
					div_result[i]=elem[i]/t;
				return div_result;
			}

			template<typename T2>
			Matrix<Rank,Rank,decltype(std::declval<T>()+std::declval<T2>())>add(Matrix<Rank,Rank,T2>m)
			{
				Matrix<Rank,Rank,decltype(std::declval<T>()+std::declval<T2>())>add_result;
				for(int i=0;i<Rank;++i)
					add_result[i]=elem[i]+m.elem[i];
				return add_result;
			}

			template<typename T2>
			Matrix<Rank,Rank,decltype(std::declval<T>()-std::declval<T2>())>sub(Matrix<Rank,Rank,T2>m)
			{
				Matrix<Rank,Rank,decltype(std::declval<T>()-std::declval<T2>())>sub_result;
				for(int i=0;i<Rank;++i)
					sub_result[i]=elem[i]-m.elem[i];
				return sub_result;
			}

			static Matrix<Rank,Rank,T>zero()
			{
				Matrix<Rank,Rank,T>zero_mat;
				memset(&zero_mat.elem,0,Rank*Rank*sizeof(T));
				return zero_mat;
			}

			std::string toString()
			{
				std::ostringstream oss;
				for(int i=0;i<Rank;++i)
				{
					oss<<elem[i][0];
					for(int j=1;j<Rank;++j)
						oss<<' '<<elem[i][j];
					oss<<'\n';
				}
				return oss.str();
			}

			//�ػ������
			inline constexpr int rank()
			{
				return Rank;
			}

			auto det()->decltype(std::declval<T>()*std::declval<T>()); //����ʽ����д�˸߽׵��ò���

			auto trace()->decltype(std::declval<T>()+std::declval<T>()) //��
			{
				decltype(std::declval<T>()+std::declval<T>()) tr=0;
				for(int i=0;i<Rank;++i)
					tr+=elem[i][i];
				return tr;
			}

			static Matrix<Rank,Rank,T>identify()
			{
				Matrix<Rank,Rank,T>id=zero();
				for(int i=0;i<Rank;++i)
					id[i][i]=1;
				return id;
			}
		};

		//3x3�����ػ�
		template<typename T>
		class Matrix<3,3,T>
		{
		public:
			Vector<3,T>elem[3]; //�����С��С�����3��3ά������
			typedef T type;
			static const int row=3;
			static const int column=3;

			inline operator T*()
			{
				return (T*)&elem;
			}

			template<typename T2>
			inline operator Matrix<3,3,T2>()
			{
				return
				{
					(T2)elem[0][0],(T2)elem[0][1],(T2)elem[0][2],
					(T2)elem[1][0],(T2)elem[1][1],(T2)elem[1][2],
					(T2)elem[2][0],(T2)elem[2][1],(T2)elem[2][2]
				};
			}

			inline Vector<3,T>& operator[](int i)
			{
				return elem[i];
			}

			inline operator std::string() //ת��Ϊ�ַ���
			{
				return toString();
			}

			inline operator const char*() //ת��ΪC�ַ���
			{
				return toString().c_str();
			}

			template<typename T2>
			inline Matrix<3,3,decltype(std::declval<T>()+std::declval<T2>())>operator +(Matrix<3,3,T2>m)
			{
				return add(m);
			}

			template<typename T2>
			inline Matrix<3,3,decltype(std::declval<T>()-std::declval<T2>())>operator -(Matrix<3,3,T2>m)
			{
				return sub(m);
			}

			template<typename T2>
			Matrix<3,3,decltype(std::declval<T>()*std::declval<T2>())>operator *(T2 t)
			{
				return mul(t);
			}

			template<typename T2>
			Matrix<3,3,decltype(std::declval<T>()*std::declval<T2>())>operator *(Matrix<3,3,T2>m)
			{
				return mul(m);
			}

			template<typename T2>
			Matrix<3,3,decltype(std::declval<T>()/std::declval<T2>())>operator /(T2 t)
			{
				return div(t);
			}

			Vector<3,T>extractColumn(int column)
			{
				return
				{
					elem[0][column],elem[1][column],elem[2][column]
				};
			}

			inline Vector<3,T>extractRow(int row)
			{
				return elem[row];
			}

			Matrix<3,3,T>transpose() //ת��
			{
				return
				{
					elem[0][0],elem[1][0],elem[2][0],
					elem[0][1],elem[1][1],elem[2][1],
					elem[0][2],elem[1][2],elem[2][2]
				};
			}

			template<typename T2>
			Matrix<3,3,decltype(std::declval<T>()*std::declval<T2>())>dot(Matrix<3,3,T2>m) //��λ���
			{
				return
				{
					elem[0][0]*m.elem[0][0],elem[0][1]*m.elem[0][1],elem[0][2]*m.elem[0][2],
					elem[1][0]*m.elem[1][0],elem[1][1]*m.elem[1][1],elem[1][2]*m.elem[1][2],
					elem[2][0]*m.elem[2][0],elem[2][1]*m.elem[2][1],elem[2][2]*m.elem[2][2]
				};
			}

			template<typename T2>
			Matrix<3,3,decltype(std::declval<T>()*std::declval<T2>())>mul(T2 t) //����
			{
				return
				{
					elem[0][0]*t,elem[0][1]*t,elem[0][2]*t,
					elem[1][0]*t,elem[1][1]*t,elem[1][2]*t,
					elem[2][0]*t,elem[2][1]*t,elem[2][2]*t
				};
			}

			template<typename T2>
			Matrix<3,3,decltype(std::declval<T>()*std::declval<T2>())>mul(Matrix<3,3,T2>m)
			{
				return
				{
					elem[0][0]*m.elem[0][0]+elem[0][1]*m.elem[1][0]+elem[0][2]*m.elem[2][0],elem[0][0]*m.elem[0][1]+elem[0][1]*m.elem[1][1]+elem[0][2]*m.elem[2][1],elem[0][0]*m.elem[0][2]+elem[0][1]*m.elem[1][2]+elem[0][2]*m.elem[2][2],
					elem[1][0]*m.elem[0][0]+elem[1][1]*m.elem[1][0]+elem[1][2]*m.elem[2][0],elem[1][0]*m.elem[0][1]+elem[1][1]*m.elem[1][1]+elem[1][2]*m.elem[2][1],elem[1][0]*m.elem[0][2]+elem[1][1]*m.elem[1][2]+elem[1][2]*m.elem[2][2],
					elem[2][0]*m.elem[0][0]+elem[2][1]*m.elem[1][0]+elem[2][2]*m.elem[2][0],elem[2][0]*m.elem[0][1]+elem[2][1]*m.elem[1][1]+elem[2][2]*m.elem[2][1],elem[2][0]*m.elem[0][2]+elem[2][1]*m.elem[1][2]+elem[2][2]*m.elem[2][2]
				};
			}

			template<typename T2>
			Matrix<3,3,decltype(std::declval<T>()/std::declval<T2>())>div(T2 t) //����
			{
				return
				{
					elem[0][0]/t,elem[0][1]/t,elem[0][2]/t,
					elem[1][0]/t,elem[1][1]/t,elem[1][2]/t,
					elem[2][0]/t,elem[2][1]/t,elem[2][2]/t
				};
			}

			template<typename T2>
			Matrix<3,3,decltype(std::declval<T>()+std::declval<T2>())>add(Matrix<3,3,T2>m)
			{
				return
				{
					elem[0][0]+m.elem[0][0],elem[0][1]+m.elem[0][1],elem[0][2]+m.elem[0][2],
					elem[1][0]+m.elem[1][0],elem[1][1]+m.elem[1][1],elem[1][2]+m.elem[1][2],
					elem[2][0]+m.elem[2][0],elem[2][1]+m.elem[2][1],elem[2][2]+m.elem[2][2]
				};
			}

			template<typename T2>
			Matrix<3,3,decltype(std::declval<T>()-std::declval<T2>())>sub(Matrix<3,3,T2>m)
			{
				return
				{
					elem[0][0]-m.elem[0][0],elem[0][1]-m.elem[0][1],elem[0][2]-m.elem[0][2],
					elem[1][0]-m.elem[1][0],elem[1][1]-m.elem[1][1],elem[1][2]-m.elem[1][2],
					elem[2][0]-m.elem[2][0],elem[2][1]-m.elem[2][1],elem[2][2]-m.elem[2][2]
				};
			}

			static Matrix<3,3,T>zero()
			{
				return
				{
					0,0,0,
					0,0,0,
					0,0,0
				};
			}

			std::string toString()
			{
				std::ostringstream oss;
				for(int i=0;i<3;++i)
				{
					oss<<elem[i][0];
					for(int j=1;j<3;++j)
						oss<<' '<<elem[i][j];
					oss<<'\n';
				}
				return oss.str();
			}

			//�ػ������
			inline constexpr int rank()
			{
				return 3;
			}

			auto det()->decltype(std::declval<T>()*std::declval<T>())
			{
				return elem[0][0]*(elem[1][1]*elem[2][2]-elem[1][2]*elem[2][1])+elem[0][1]*(elem[1][2]*elem[2][0]-elem[1][0]*elem[2][2])+elem[0][2]*(elem[1][0]*elem[2][1]-elem[1][1]*elem[2][0]);
			}

			auto trace()->decltype(std::declval<T>()+std::declval<T>()) //��
			{
				return elem[0][0]+elem[1][1]+elem[2][2];
			}

			static Matrix<3,3,T>identity()
			{
				return
				{
					1,0,0,
					0,1,0,
					0,0,1
				};
			}
		};

		using Matrix3x3f=Matrix<3,3,float>;
	}
}
#endif//ANDROMEDA_MATH_MATRIX
