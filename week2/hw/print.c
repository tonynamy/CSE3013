#include "header.h"

void print(int* a) {

	// 배열의 각 원소에 대하여
	for(int i=0;i<10;i++) {
		
		// 공백 한 칸을 띄고 출력
		printf("%d ", a[i]);

	}

	// 줄바꿈
	printf("\n");

}
