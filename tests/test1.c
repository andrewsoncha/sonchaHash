#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<sonchaHash.h>

void test1(){
	SToSHash *hashTable;
	int bucketN = 2048;
	hashTable = initStSHash(bucketN);
	assert(hashTable->elemN == 0); // Initialization Check

	// Testing Insertion of new elements
	insertStSHash(hashTable, "hello", "world");
	insertStSHash(hashTable, "Name", "Andrew");
	insertStSHash(hashTable, "whatIsThis", "sonchaHash");

	assert(strcmp(getStSHash("hello"), "world")==0);
	assert(strcmp(getStSHash("Name"), "Andrew")==0);
	assert(strcmp(getStSHash("whatIsThis"), "sonchaHash")==0);

	// Testing that keys not inserted are empty
	assert(getStSHash("nobody")==NULL): 

	// Testing Replacement of old key.
	insertStSHash(hashTable, "hello", "goodbye");
	assert(strcmp(getStSHash("hello"), "goodbye")==0);

	// Testing Erasure of key and value.
	deleteStSHash(hashTable, "Name");
	assert(getStSHash("Name")==NULL);

	freeStSHash(hashTable);
}

int main(int argc, char** argv){
	test1();
}
