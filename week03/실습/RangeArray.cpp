#include "RangeArray.h"

RangeArray::RangeArray(int l, int h) : Array(h - l + 1) {
	//	Array 클래스의 생성자 호출
	low = l;		//	멤버 변수 초기화
	high = h;
}

RangeArray::~RangeArray() {	/* 따로 해 줄 작없 없음 */ }

int RangeArray::baseValue() const {	return low;	}
int RangeArray::endValue() const {	return high;	}

int& RangeArray::operator [](int i) {
	return Array::operator [](i - low); 				
}

int RangeArray::operator [](int i) const {
	return Array::operator [](i - low);				
}
