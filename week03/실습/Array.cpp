#include "Array.h"

Array::Array(int size) {
	if(size <= 0) {
		cout<<"Array size error!"<<endl;
		exit(-1);				
	}				
	len = size;
	data = new int[size];
}

Array::~Array() {
	delete[] data;				
}

int Array::length() const {
	return len;				
}

int& Array::operator [](int i) {
	static int tmp;
	//	배열의 인덱스가 범위 내에 있으면 해당 원소를 리턴한다.
	//	그렇지 않으면 에러 메시지를 출력하고 tmp를 리턴한다.
	if(0 <= i && i <= len - 1)
		return data[i];
	else {
		cout<<"Array bound error!"<<endl;
		return tmp;				
	}
}

int Array::operator [](int i) const {
	//	배열의 인덱스가 범위 내에 있으면 값을 리턴
	//	그렇지 ㅇ낳으면 에러 메시지를 출력하고 0을 리넡
	if(0 <= i && i <= len - 1)
		return data[i];
	else {
		cout<<"Array bound error!"<<endl;
		return 0;				
	}			
}

void Array::print() {
	cout<<"[";
	for(int i = 0; i < len; i++) { 
		cout<<data[i];
		if(i + 1 < len) cout<<" ";
	}
	cout<<"]"<<endl;				
}
