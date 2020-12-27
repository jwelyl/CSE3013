#include "Array.h"

template <class T>
class GrowableArray : public Array<T> {
private:	//	GrowableArray 내 멤버함수에서만 호출함
	void doubleArray(int idx);	//	idx의 2배 크기로 배열 크기 증가
public:		//	Array 클래스의 operator를 overriding
	GrowableArray(int size);
	T& operator [](int i);
	T operator [](int i) const;
};

template <class T>
GrowableArray<T>::GrowableArray(int size) : Array<T>::Array(size) {
}

template <class T>
void GrowableArray<T>::doubleArray(int idx) {	
	T* temp = new T[idx * 2];		//	idx의 2배 크기의 배열을 할당
		
	for(int i = 0; i < Array<T>::len; i++) 
		temp[i] = Array<T>::data[i];				//	기존 배열 크기만큼 기존 배열 복사
	for(int i = Array<T>::len; i < idx * 2; i++) 
		temp[i] = 0;							//	추가된 공간은 0으로 초기화

	delete[] Array<T>::data;	//	기존에 할당했던 배열 해제
	Array<T>::data = temp;		//	data가 새로 할당한 배열을 가리키게 함
	Array<T>::len = idx * 2;
}

template <class T>
T& GrowableArray<T>::operator [](int i) {
	if(i >= Array<T>::len) {			//	범위 초과 시
		doubleArray(i);		//	배열 크기 2배로 증가
	}				
	return Array<T>::operator [](i);
}
		
template <class T>
T GrowableArray<T>::operator [](int i) const {
	if(i >= Array<T>::len) {
		doubleArray(i);				
	}				
	return Array<T>::operator [](i);
}
