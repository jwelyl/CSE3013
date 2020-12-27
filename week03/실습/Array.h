#ifndef __ARRAY__
#define __ARRAY__
#include <iostream>
#include <stdlib.h>
using namespace std;

class Array {
protected:
	int* data;
	int len;
public:
	Array(int size);	//	생성자 size가 0 이하 값이면 에러 메시지를 출력하고 종료,
										//	양수이면 new를 사용하여 배열 data를 할당. 
										//	len 값 초기화
	~Array();					//	소멸자 : 할당된 메모리 해제
	
	int length() const;				//	배열의 크기 리턴
	int& operator [](int i);	//	left value : 배열에 원소를 삽입
	int operator [](int i) const;		//	right value : 배열의 원소의 값을 반환

	void print();			//	배열의 모든 내용을 출력해주는 함수
};

#endif
