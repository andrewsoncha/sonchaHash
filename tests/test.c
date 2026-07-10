#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>
#include "../sonchaHash.h"

void test1(){
	StSHash *hashTable;
	int bucketN = 2048;
	hashTable = initStSHash(bucketN);
	assert(hashTable->elementN == 0); // Initialization Check

	// Testing Insertion of new elements
	insertStSHash(hashTable, "hello", "world");
	insertStSHash(hashTable, "Name", "Andrew");
	insertStSHash(hashTable, "whatIsThis", "sonchaHash");

	assert(strcmp(getStSHash(hashTable, "hello"), "world")==0);
	assert(strcmp(getStSHash(hashTable, "Name"), "Andrew")==0);
	assert(strcmp(getStSHash(hashTable, "whatIsThis"), "sonchaHash")==0);

	// Testing that keys not inserted are empty
	assert(getStSHash(hashTable, "nobody")==NULL); 

	// Testing Replacement of old key.
	insertStSHash(hashTable, "hello", "goodbye");
	printf("get(\"hello\"): %s\n", getStSHash(hashTable, "hello"));
	assert(strcmp(getStSHash(hashTable, "hello"), "goodbye")==0);

	// Testing Erasure of key and value.
	deleteStSHash(hashTable, "Name");
	printf("get(\"Name\"): %s\n", getStSHash(hashTable, "Name"));
	assert(getStSHash(hashTable, "Name")==NULL);

	freeStSHash(hashTable);
}

void test2(){ //Intentionally inserting more inputs than initial bucketN, causing hash collision
	const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
	StSHash *hashTable;
	int bucketN = 111;
	int testN = 200;
	char testKeys[200][6];
	char testVals[200][11];


	for(int i=0;i<200;i++){
		int currI = i;
		printf("i: %d   ", i);
		for(int j=0;j<5;j++){
			int alphaN = currI%26;
			testKeys[i][j] = alphabet[alphaN];
			currI /= 26;
			printf("%d ", alphaN);
		}
		printf("\n");
		testKeys[i][5] = 0;

		for(int j=0;j<10;j++){
			int alphaN = rand()%26;
			testVals[i][j] = alphabet[alphaN];
		}
		testVals[i][10] = 0;
	}

	printf("hashVal(%s, 31): %d\n", testKeys[0], hashStr(testKeys[0], bucketN, 31));
	printf("hashVal(%s, 33): %d\n", testKeys[0], hashStr(testKeys[0], bucketN, 33));
	printf("hashVal(%s, 37): %d\n", testKeys[0], hashStr(testKeys[0], bucketN, 37));
	printf("hashVal(%s, 41): %d\n", testKeys[0], hashStr(testKeys[0], bucketN, 41));
	printf("hashVal(%s, 31): %d\n", testKeys[25], hashStr(testKeys[25], bucketN, 31));
	printf("hashVal(%s, 33): %d\n", testKeys[25], hashStr(testKeys[25], bucketN, 33));
	printf("hashVal(%s, 37): %d\n", testKeys[25], hashStr(testKeys[25], bucketN, 37));
	printf("hashVal(%s, 41): %d\n", testKeys[25], hashStr(testKeys[25], bucketN, 41));

	hashTable = initStSHash(bucketN);
	for(int i=0;i<200;i++){
		printf("i: %d   inserting key: %s   val: %s\n", i, testKeys[i], testVals[i]);
		insertStSHash(hashTable, testKeys[i], testVals[i]);
	}

	for(int i=0;i<200;i++){
		printf("i: %d    key: %s  val: %s\n", i, testKeys[i], testVals[i]);
		char* gotVal = getStSHash(hashTable, testKeys[i]);
		printf("got val: %s\n", gotVal);
		assert(strcmp(gotVal, testVals[i])==0);
	}
	printf("test 2 all %d tests passed!\n", testN);

	freeStSHash(hashTable);
}

int main(int argc, char** argv){
	test1();
	test2();
}
