#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define DEPTH_MAX 8

unsigned int hashStr(char* str, int divider, int primeN){
	unsigned int N = strlen(str);
	unsigned int val = 0;
	for(int i=0;i<N;i++){
		val += primeN * val + str[i];
		val %= divider;
	}
	return val;
}

// If the depth goes so deep that you need more primes, 
// you're doing something wrong.
int primeNums[DEPTH_MAX] = {31, 33, 37, 41, 43, 47, 53, 59}; 

typedef struct _SBucket{
	int type; // If the bucket holds one value (string), type == 0. 
		  // If the bucket holds multiple more buckets, type == 1
	int depth;
	char* key;
	char* value;
	int subBucketN;
	struct _SBucket* subBuckets;
} SBucket;

typedef struct _StSHash{
	int elementN;
	int bucketN;
	struct _SBucket* mainBucket;
} StSHash;

SBucket* initSBucket(){
	SBucket *newBucket;
	newBucket = malloc(sizeof(SBucket));
	newBucket->type = 0;
	newBucket->key = NULL;
	newBucket->value = NULL;
	return newBucket;
}
StSHash* initStSHash(int bucketN){
	StSHash* newHashTable;

	newHashTable = malloc(sizeof(StSHash));
	newHashTable->elementN = 0;
	newHashTable->mainBucket =  malloc(sizeof(SBucket));
	newHashTable->bucketN = bucketN;

	SBucket* mainBucket = newHashTable->mainBucket;
	mainBucket->type = 1;
	mainBucket->depth = 0;
	mainBucket->subBucketN = bucketN;
	mainBucket->subBuckets = malloc(sizeof(SBucket)*bucketN);
	for(int i=0;i<bucketN;i++){
		mainBucket->subBuckets[i].type = 0;
		mainBucket->subBuckets[i].key = NULL;
		mainBucket->subBuckets[i].value = NULL;
		mainBucket->subBuckets[i].depth = 1;
	}

	return newHashTable;
}

SBucket* findSBucket(SBucket* currBucket, char* key){
	unsigned int hashVal;
	if(currBucket->depth >= DEPTH_MAX){
		printf("ERROR! findSBucket. depth too deep!\n");
		return NULL;
	}
	SBucket* corrBucket; //Bucket corresponding to hash value
	hashVal = hashStr(key, currBucket->subBucketN, primeNums[0]);
	if(hashVal > currBucket->subBucketN){
		printf("ERROR! findSBucket. hashVal bigger than bucketN!\n");
	}

	corrBucket = &(currBucket->subBuckets[hashVal]);
	if(currBucket == NULL){
		printf("ERROR! findSBucket. Bucket Not Found (bucket corresponding to hasVal==NULL)!\n");
		return NULL;
	}
	if(corrBucket->type == 0){
		return corrBucket;
	}
	else{
		return findSBucket(corrBucket, key);
	}
}

void splitSBucket(SBucket* bucket, int bucketN){
	if(bucket->depth >= DEPTH_MAX-1){
		printf("ERROR! splitSBucket. Maximum Depth %d reached!\n", DEPTH_MAX);
		return;
	}

	char* oldKey = NULL;
	char* oldValue = NULL;
	bucket->type = 1;
	if(bucket->key != NULL){
		oldKey = bucket->key;
		oldValue = bucket->value;
		bucket->key = NULL;
		bucket->value = NULL;
	}
	bucket->subBucketN = bucketN;
	bucket->subBuckets = malloc(sizeof(SBucket)*bucketN);
	int newDepth = bucket->depth+1;
	for(int i=0;i<bucketN;i++){
		bucket->subBuckets[i].type = 0;
		bucket->subBuckets[i].depth = newDepth; 
		bucket->subBuckets[i].key = NULL;
		bucket->subBuckets[i].value = NULL;
	}

	if(oldKey != NULL){
		unsigned int hashVal = hashStr(oldKey, bucketN, primeNums[newDepth]);
		SBucket* oldKeyBucket = &bucket->subBuckets[bucketN];
		oldKeyBucket->key = oldKey;
		oldKeyBucket->value = oldValue;
	}
}

void insertStSBucket(SBucket* bucket, char* key, char* value, int splitBucketN){
	int keyLen = strlen(key);
	int valueLen = strlen(value);

	if(bucket->type == 0){
		if(bucket->key == NULL){
			bucket->key = malloc(keyLen);
			bucket->value = malloc(valueLen);
			strcpy(bucket->key, key);
			strcpy(bucket->value, value);
		}
		else{
			char* oldKey = bucket->key;
			char* oldValue = bucket->value;
			if(strcmp(oldKey, key)==0){ // Replace old value with new
				oldValue = realloc(oldValue, valueLen);
				strcpy(oldValue, value);
			}
			else{ // Hash Collision
			      splitSBucket(bucket, splitBucketN);
			      SBucket* foundSubBucket = findSBucket(bucket, key);
			      insertStSBucket(foundSubBucket, key, value, splitBucketN);
			}
		}
	}
	else if(bucket->type == 1){
		printf("ERROR! insertStSBucket. Given SBucket->type==1!\n");
		return;
	}
}

void insertStSHash(StSHash* ht, char* key, char* value){
	SBucket* insertBucket;
	insertBucket = findSBucket(ht->mainBucket, key);
	if(insertBucket == NULL){
		return;
	}
	insertStSBucket(insertBucket, key, value, ht->bucketN);
}

char* getStSHash(StSHash* ht, char* key){
	SBucket* foundBucket;
	foundBucket = findSBucket(ht->mainBucket, key);
	if(foundBucket == NULL){
		return NULL;
	}

	if(foundBucket->type == 0){
		if(foundBucket->key == NULL){ //key not found
			return NULL;
		}
		else{
			return foundBucket->value;
		}
	}
	else if(foundBucket->type == 1){
		printf("ERROR! getStSHash. found SBucket->type==1!\n");
		return NULL;
	}
	return NULL;
}

void freeSBucketInternal(SBucket* bucket){
	if(bucket->type == 0){
		if(bucket->key!=NULL){
			free(bucket->key);
		}
		if(bucket->value!=NULL){
			free(bucket->value);
		}
	}
	else{
		for(int i=0;i<bucket->subBucketN;i++){
			freeSBucketInternal(&(bucket->subBuckets[i]));
		}
		free(bucket->subBuckets);
	}
}
void freeStSHash(StSHash* ht){
	freeSBucketInternal(ht->mainBucket);
	free(ht->mainBucket);
	free(ht);
}

