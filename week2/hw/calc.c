#include "header.h"

// 1~n까지의 각 숫자의 개수를 계산하는 함수
void calc(int *a, int n) {

	// 1~n까지의 각 숫자에 대하여
	for(int i=1;i<=n;i++)
		// 각 자리의 숫자의 개수를 배열에 추가
		calc_digit(a, i);

}

// 숫자 n의 각 자리의 숫자의 개수를 계산하여 배열에 추가하는 함수
void calc_digit(int *a, int n) {

	// 10 이상이라면
	if(n/10 > 0) {
		// 1의 자리를 제거한 뒤 재귀호출
		calc_digit(a, n/10);
	}

	// 10으로 나눈 나머지가 1의 자리의 숫자이므로 이에 해당하는 원소를 1늘림
	a[n%10]++;

}
