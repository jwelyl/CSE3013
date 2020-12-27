#include "Header.h"

void Remove_Blanks_At_The_End( char *line ) {
	int i, k, newline_flag = 0;

	for ( k = 0; ; k++ ) {
		if ( line[k] == '\n' ) {
			newline_flag = 1;
			break;
		}
		else if ( line[k] == '\0' ) {
			break;
		}
	}
	for ( i = k; i >= 0; i-- ) {
		if ( line[i] != ' ' ) {
			break;
		}
	}
	if ( newline_flag == 1 ) {
		line[i] = '\n';
		line[i+1] = '\0';
	}
	else {
		line[i] = '\0';
	}
}

void Get_Blanks_Chars( char *line, int Start, int *N_Blanks, int *N_Chars ) {
	int i, blank_flag = 0;	//	변수 blank_flag 변수 초기화

	*N_Blanks = *N_Chars = 0;	//	전달받은 변수 N_Blanks, N_Chars 함수에서 0으로 초기화
	for ( i = Start; ; i++ ) {
		if ( (line[i] == '\n') || (line[i] == '\0') ) {
			break;
		}
		else if ( line[i] == ' ' ) {
			if ( blank_flag == 0 ) {
				++(*N_Blanks);
			}
			else {
				break;
			}
		}
		else {
			blank_flag = 1;
			++(*N_Chars);
		}
	}
}

