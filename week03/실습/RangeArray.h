#include "Array.h"

class RangeArray:public Array {
protected:
	int low;		//	배열의 첫 번째 index
	int high;		//	배열의 마지막 index
public:
	RangeArray(int l, int h);
	~RangeArray();
	
	int baseValue() const;	//	배열의 첫 번째 index 값 리턴
	int endValue() const;		//	배열의 마지막 index 값 리턴

	int& operator [](int i);	//	Array 클래스의 다중 정의된 연산자를 호출
	int operator [](int i) const;
};
