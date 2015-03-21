#include "fscrypt.h"
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void *fs_encrypt(void *plaintext, int bufsize, char *keystr, int *resultlen){
/*	void BF_set_key(BF_KEY *key, int len, const unsigned char *data);
 void BF_ecb_encrypt(const unsigned char *in, unsigned char *out, BF_KEY *key, int enc);
*/

	BF_KEY* key = (BF_KEY*)malloc(sizeof(BF_KEY));
	BF_set_key(key, sizeof(keystr), (unsigned char*)keystr);
	
	unsigned char* inHandler = (unsigned char*)malloc(strlen((const char*)plaintext)+1);
	cout << "Sizeof(plaintext): " << strlen((const char*)plaintext)+1/**sizeof(unsigned char)*/ << endl;
	inHandler = (unsigned char*)plaintext;
	inHandler[strlen((const char*)plaintext)] = '\0';
	
	if(inHandler[strlen((const char*)plaintext)] == '\0'){
		cout << "It Works!\n";
	}
	else{
		cout << "Re-evaluate your life choices...\n";
	}
	
	vector<unsigned char*> outputVector;
	unsigned char* outHandler = (unsigned char*)malloc(BLOCKSIZE+1);
	int isDone = 0;
	unsigned char* tempArray;
	int outLen = 0;
	unsigned char* iv = (unsigned char*)malloc(BLOCKSIZE+1);;
	int i,padder;
	unsigned char* xorHandler = (unsigned char*)malloc(BLOCKSIZE*sizeof(unsigned char));
	for(i=0; i<BLOCKSIZE; i++){
		iv[i] = '0';
	}
	for(i=0; i<BLOCKSIZE; i++){
		if(inHandler[i] == '\0'){
			isDone = 1;
			break;
		}
	}
	// Case where First Block is under BLOCKSIZE
		//padding
	//char temp = (char)(((int)'0')+4);
	//cout << "char of 4: " << temp << endl;
	if(1 == isDone){
		strncpy((char*)outHandler, (const char*)inHandler, BLOCKSIZE);
		padder = BLOCKSIZE-i-1;
		
		for(i=i+1; i<BLOCKSIZE; i++){
			outHandler[i] = (char)(((int)'0')+padder);
		}
		cout << "outHandler: " << outHandler << endl;
		BF_ecb_encrypt((const unsigned char*)outHandler, outHandler, key, BF_ENCRYPT);
		tempArray = (unsigned char*)malloc(BLOCKSIZE);
		for(i=0;i<BLOCKSIZE;i++){
			tempArray[i] = outHandler[i];
		}
		outputVector.push_back(tempArray);
		outLen = outLen + BLOCKSIZE;
		iv = outHandler;
	}
	//Other Cases
	else{
		//cout << "InputProcessed: " << strncpy((char*)outHandler, (const char*)inHandler,BLOCKSIZE) << endl;
		cout << "inHandler: " << inHandler << endl;
		BF_ecb_encrypt((const unsigned char*)inHandler,outHandler, key, BF_ENCRYPT);
		tempArray = (unsigned char*)malloc(BLOCKSIZE);
		for(i=0;i<BLOCKSIZE;i++){
			tempArray[i] = outHandler[i];
		}
		outputVector.push_back(tempArray);
		outLen = outLen + BLOCKSIZE;
		iv = outHandler;
	
		for(i=BLOCKSIZE; i<BLOCKSIZE*2; i++){
			if(inHandler[i] == '\0'){
				isDone = 1;
				break;
			}
		}
		if(1 == isDone){
			strncpy((char*)inHandler,(const char*)(inHandler+(BLOCKSIZE*sizeof(unsigned char))),strlen((const char*)plaintext));
		}
		while(0 == isDone){
			strncpy((char*)inHandler,(const char*)(inHandler+(BLOCKSIZE*sizeof(unsigned char))),strlen((const char*)plaintext));
			for(i=0; i<BLOCKSIZE; i++){
				if(inHandler[i] == '\0'){
					isDone = 1;
					break;
				}
			}
			if(1 == isDone){
				break;
			}
			//cout << "InputProcessed: " << strncpy(NULL, (const char*)inHandler, BLOCKSIZE) << endl;
			cout << "inHandler: " << inHandler << endl;
			for(i=0; i<BLOCKSIZE; i++){
				xorHandler[i] = inHandler[i]^iv[i];
			}
			BF_ecb_encrypt((const unsigned char*)xorHandler,outHandler,key,BF_ENCRYPT);
			iv = outHandler;
			tempArray = (unsigned char*)malloc(BLOCKSIZE);
			for(i=0;i<BLOCKSIZE;i++){
				tempArray[i] = outHandler[i];
			}
			outputVector.push_back(tempArray);
			outLen = outLen + BLOCKSIZE;

		}
		padder = BLOCKSIZE -i -1;
		for(i=i+1; i<BLOCKSIZE; i++){
			inHandler[i] = (char)(((int)'0')+padder);
		}
		for(i=0;i<BLOCKSIZE;i++){
			xorHandler[i] = inHandler[i]^iv[i];
		}
		//cout << "InputProcessed: " << strncpy((char*)outHandler, (const char*)outHandler, BLOCKSIZE) << endl;
		cout << "inHandler: " << inHandler << endl;
		BF_ecb_encrypt((const unsigned char*)xorHandler, outHandler, key, BF_ENCRYPT);
		tempArray = (unsigned char*)malloc(BLOCKSIZE);
		for(i=0;i<BLOCKSIZE;i++){
			tempArray[i] = outHandler[i];
		}
		outputVector.push_back(tempArray);
		outLen = outLen + BLOCKSIZE;

	}

	unsigned char* output = (unsigned char*)malloc(outLen*sizeof(unsigned char));

	for(i=0; i<outputVector.size(); i++){
		cout << "outputVector[" << i << "]: " << (char*)outputVector[i] << endl;
		strncpy((char*)(output+(BLOCKSIZE*i)),(const char*)outputVector[i],BLOCKSIZE);
	}
	*resultlen = outLen;
	return (void*)output;
	
}
void *fs_decrypt(void *ciphertext, int bufsize, char *keystr, int *resultlen){
	BF_KEY* key = (BF_KEY*)malloc(sizeof(BF_KEY));
	BF_set_key(key, sizeof(keystr), (unsigned char*)keystr);
	unsigned char* inHandler = (unsigned char*)malloc(strlen((const char*)ciphertext));
	inHandler = (unsigned char*)ciphertext;
	int numBlocks = strlen((const char*)ciphertext);
	cout << "numBlocks: " <<numBlocks << endl;
	vector<unsigned char*> outputVector;
	unsigned char* outHandler = (unsigned char*)malloc(BLOCKSIZE);
	int i,padder;
	unsigned char* xorHandler = (unsigned char*)malloc(BLOCKSIZE*sizeof(unsigned char));
	int isDone = 0;
	int outLen = 0;
	unsigned char* tempArray;
	unsigned char* iv = (unsigned char*)malloc(BLOCKSIZE);
	for(i=0;i<BLOCKSIZE;i++){
		iv[i] = '0';
	}
	
	//while(0 == isDone){
	while(numBlocks > 0){
		tempArray = (unsigned char*)malloc(BLOCKSIZE);
		BF_ecb_encrypt((const unsigned char*)inHandler, outHandler, key, BF_DECRYPT);
		
		for(i=0;i<BLOCKSIZE;i++){
			tempArray[i] = outHandler[i]^iv[i];
		}
		for(i=0;i<BLOCKSIZE;i++){
			if(tempArray[i] == '\0'){
				isDone = 1;
			}
		}
		outputVector.push_back(tempArray);
		outLen = outLen+BLOCKSIZE;
		for(i=0;i<BLOCKSIZE;i++){
			iv[i] = inHandler[i];
		}
		strncpy((char*)inHandler,(const char*)(inHandler+(BLOCKSIZE*sizeof(unsigned char))),strlen((const char*)ciphertext));
		numBlocks = numBlocks - 8;
	}
	
	unsigned char* output = (unsigned char*)malloc(outLen*sizeof(unsigned char));
	for(i=0;i<outputVector.size();i++){
		strncpy((char*)(output+(BLOCKSIZE*i)),(const char*)outputVector[i],BLOCKSIZE);
	}
	*resultlen = outLen;
	return (void*)output;
}
