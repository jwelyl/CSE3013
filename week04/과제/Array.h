#ifndef __ARRAY__
#define __ARRAY__
#include <iostream>
#include <stdlib.h>
using namespace std;

template <class T>
class Array {
protected:
	T* data;
	int len;
public:
	Array(int size);	//	생성자 size가 0 이하 값이면 에러 메시지를 출력하고 종료,
										//	양수이면 new를 사용하여 배열 data를 할당. 
										//	len 값 초기화
	~Array();					//	소멸자 : 할당된 메모리 해제
	
	int length() const;				//	배열의 크기 리턴
	T& operator [](int i);	//	left value : 배열에 원소를 삽입
	T operator [](int i) const;		//	right value : 배열의 원소의 값을 반환

	void print();			//	배열의 모든 내용을 출력해주는 함수
};

template <class T>
Array<T>::Array(int size) {
	if(size <= 0) {
		cout<<"Array size error!"<<endl;
		exit(-1);
	}
	len = size;
	data = new T[size];
}

template <class T>
Array<T>::~Array() {
	delete[] data;
}

template <class T>
int Array<T>::length() const {
	return len;				
}

template <class T>
T& Array<T>::operator [](int i) {
	static T tmp;
	if(0 <= i && i <= len - 1)
		return data[i];
	else {
		cout<<"Array bound error!"<<endl;
		return tmp;				
	}				
}

template <class T>
T Array<T>::operator [](int i) const {
	if(0 <= i && i <= len - 1)
		return data[i];
	else {
		cout<<"Array bound error!"<<endl;
		return T();				
	}			
}

template <class T>
void Array<T>::print() {
	cout<<"[";
	for(int i = 0; i < len; i++) {
		cout<<data[i];
		if(i + 1 < len) cout<<" ";				
	}				
	cout<<"]"<<endl;
}

#endif
