#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include "fscrypt.h"

using namespace std;

void* fs_encrypt(void* plaintext, int bufsize, char* keystr, int *resultlen){
	BF_KEY* key = (BF_KEY*)malloc(sizeof(BF_KEY));
	BF_set_key(key, BLOCKSIZE, (const unsigned char*)keystr);
	unsigned char* output = (unsigned char*)malloc((BLOCKSIZE+1)*sizeof(char));
	unsigned char iv[8] = "0000000";
	unsigned char input[8] = "1234567";
	//BF_cbc_encrypt((const unsigned char*)plaintext, output, strlen((const char*)plaintext)+1, key, iv, BF_ENCRYPT);
	BF_cbc_encrypt((const unsigned char*)input, output, BLOCKSIZE+1, key, iv, BF_ENCRYPT);
//	cout << hex << output << endl;
	*resultlen = strlen((const char*)output);
	return (void*)output;
}

void* fs_decrypt(void* ciphertext, int bufsize, char* keystr, int* resultlen){
	BF_KEY* key = (BF_KEY*)malloc(sizeof(BF_KEY));
	BF_set_key(key, BLOCKSIZE, (const unsigned char*)keystr);
	unsigned char* output = (unsigned char*)malloc((BLOCKSIZE+1)*sizeof(char));
	unsigned char iv[8] = "0000000";
	unsigned char input[8] = "1234567";
	//BF_cbc_encrypt((const unsigned char*)ciphertext, output, strlen((const char*)ciphertext)+1, key, iv, BF_DECRYPT);
	BF_cbc_encrypt((const unsigned char*)input, output, BLOCKSIZE+1, key, iv, BF_DECRYPT);
	return (void*)output;
}
/*
vector<unsigned char*> splitInput(unsigned char* inText){

}

unsigned char* assembleVector(vector<unsigned char*> inVec){

}*/
