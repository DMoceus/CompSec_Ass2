#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main(){
	char* A = (char*)malloc(16);
	char* B = (char*)malloc(16);
	
	A = (char*)"12345678";
	strncpy(B,A+(2*sizeof(char)),5);
	//A[5] = '\0';
	cout << A << endl;
	cout << B << endl;
	return 1;
}
