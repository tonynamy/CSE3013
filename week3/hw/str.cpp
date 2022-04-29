#include "Str.h"
#include <string.h>

Str::Str(int leng) {
	// 생성자로 입력 받은 길이를 클래스 변수에 할당함.
	len = leng;
	// str에 len+1 만큼의 크기를 할당(널문자 고려)
	str = new char[len+1];
}

Str::Str(char* neyong) {
	// 입력받은 char 형 변수의 길이를 계산하고 클래스 변수에 할당함.
	len = strlen(neyong);
	// str에 len+1 만큼의 크기를 할당(널문자 고려)
	str = new char[len+1];
	// strcpy 함수를 통하여 char 배열 복사
	strcpy(str, neyong);
}

Str::~Str() {
	// 할당된 변수를 메모리에서 해제 
	delete[] str;
}

int Str::length(void) {
	// len 클래스 변수 반환
	return len;
}

char* Str::contents(void) {
	// str 클래스 변수 반환
	return str;
}

int Str::compare(class Str& a) {
	// a의 str 클래스 변수와 현재 클래스의 str 클래스 변수와의 strcmp
	return strcmp(str, a.contents());
}

int Str::compare(char* a) {
	// a와 현재 클래스의 str 클래스 변수와의 strcmp
	return strcmp(str, a);
}

void Str::operator=(char* a) {
	// 입력받은 char 배열의 길이를 계산하고 클래스 변수에 저장
	len = strlen(a);
	// 원래의 str 변수 메모리 해제
	delete[] str;
	// str 변수에 새로 저장한 len+1의 공간 할당
	str = new char[len+1];
	// strcpy로 char 배열 복사
	strcpy(str, a);
}

void Str::operator=(class Str& a) {
	// 입력받은 클래스의 len을 받아와 저장
	len = a.length();
	// 원래의 str 변수 메모리 해제
	delete[] str;
	// str 변수에 새로 저장한 len+1의 공간 할당
	str = new char[len+1];
	// strcpy로 char 배열 복사(a의 char 배열은 contents 메소드 호출하여 받아옴)
	strcpy(str, a.contents());
}	
