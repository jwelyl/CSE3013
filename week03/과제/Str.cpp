#include "Str.h"

Str::Str(int leng) {
	len = leng;				
	str = new char[len + 1];	//	'\0' 포함 len + 1 만큼 할당
}

Str::Str(const char* neyong) {
	len = strlen(neyong);
	str = new char[len + 1];
	strcpy(str, neyong);				
}

Str::~Str() {
	delete[] str;				
}

int Str::length() const {
	return len;				
}

char* Str::contents() const {
	return str;				
}

int Str::compare(class Str& a) {
	return strcmp(str, a.contents());				
}

int Str::compare(const char* a) {
	return strcmp(str, a);				
}

void Str::operator=(const char* a) {
	if(strlen(a) > len) {	//	대입할 문자열 a의 길이가 더 길 경우
		delete[] str;				//	str에 할당된 메모리를 해제하고
		str = new char[strlen(a) + 1];	//	a  길이만큼 재할당함
	}

	len = strlen(a);	//	str의 길이 업데이트
	strcpy(str, a);		//	a를 str에 복사
}

void Str::operator=(class Str& a) {	//	멤버 함수를 활용하여 문자열과 길이를 이용하여 위와 같은 방법을 적용
	if(a.length() > len) {
		delete[] str;
		str = new char[a.length() + 1];				
	}				

	len = a.length();
	strcpy(str, a.contents());
}
