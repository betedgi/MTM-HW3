/*
 * map_test_unit.c
 *
 *  Created on: 19 בדצמ× 2016
 *      Author: User
 */

#include <stdio.h>
#include <stdbool.h>
#include<string.h>
#include <stdlib.h>
#include "aux_macros.h"
#include "map_mtm.h"
#define N 1000

/** Function to be used for copying an int as a key to the map */
static MapKeyElement copyKeyInt(MapKeyElement n) {
	if (!n) {
		return NULL;
	}
	int* copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	*copy = *(int*)n;
	return copy;
}

/** Function to be used for copying a char as a data to the map */
static MapDataElement copyDataChar(MapDataElement n) {
	if (!n) {
		return NULL;
	}
	char* copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	*copy = *(char*)n;
	return (MapDataElement) copy;
}

/** Function to be used by the map for freeing elements */
static void freeInt(MapKeyElement n) {
	free(n);
}

/** Function to be used by the map for freeing elements */
static void freeChar(MapDataElement n) {
	free(n);
}

/** Function to be used by the map for comparing elements */
static int compareInts(MapKeyElement n1, MapKeyElement n2) {
	int* int_1 = (int*)n1;
	int* int_2 = (int*)n2;
	if(*int_1 == *int_2) return 0;
	else if (*int_1 > *int_2) return 1;
	else return -1;
}

static void (*test_void)() = NULL;
static int (*test_int)() = NULL;
static MapKeyElement (*test_mapKeyElement)() = NULL;
static MapDataElement (*test_mapDataElement)() = NULL;


static bool testmapCreate() {
	bool res = true;

	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
	TEST_DIFFERENT(res, map, NULL);

	Map map_n = mapCreate(test_mapKeyElement, copyDataChar, freeInt, freeChar, compareInts);
	TEST_EQUALS(res, map_n, NULL);

	 map_n = mapCreate(copyKeyInt, test_mapDataElement, freeInt, freeChar, compareInts);
		TEST_EQUALS(res, map_n, NULL);

	 map_n = mapCreate(copyKeyInt, copyDataChar, test_void, freeChar, compareInts);
		TEST_EQUALS(res, map_n, NULL);

	 map_n = mapCreate(copyKeyInt, copyDataChar, freeInt, test_void, compareInts);
		TEST_EQUALS(res, map_n, NULL);

	map_n = mapCreate(copyKeyInt, copyDataChar, freeInt, freeChar, test_int);
		TEST_EQUALS(res, map_n, NULL);

	mapDestroy(map);

	return res;

}

static bool testmapDestroy(){
	bool res = true;
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
	mapDestroy(map);
	TEST_EQUALS(res, map, NULL);
	return true;//Test with valgrind

}

static bool testmapCopy(){

	bool res = true;
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
	for (int i = 1; i < N; ++i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}
	Map map_n=NULL;
	Map map_copy = mapCopy(map_n);
	TEST_EQUALS(res,map_copy,NULL);

	map_copy = mapCopy(map);
	for (int i = 0; i < N; ++i) {
		res = mapContains(map_copy,&i);
	}

	mapDestroy(map);
	mapDestroy(map_copy);

	return true;
}



static bool testmapGetSize(){

	bool res = true;

	Map map_n = NULL;
	int size = mapGetSize(map_n);
	TEST_EQUALS(res,size,-1);
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
	for (int i = 1; i < N; ++i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}
	size = mapGetSize(map);
	TEST_EQUALS(res,size,N-1);

	mapDestroy(map);

	return res;
}


static bool testmapContains(){
	bool res=true;
	int i;
	Map map_n=NULL;
	TEST_EQUALS(res,mapContains(map_n,&i),false);
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
	for (int i = 1; i < N; ++i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
		TEST_EQUALS(res,mapContains(map,&i),true);
	}
	MapKeyElement ptr=NULL;
	TEST_EQUALS(res,mapContains(map,ptr),false);

	mapDestroy(map);

	return res;
}


static bool testmapPut(){//TODO check change val
	bool res=true;
	int x;
	Map map_n=NULL;
	MapKeyElement key = NULL;
	MapDataElement element = NULL;

	TEST_EQUALS(res,mapPut(map_n,&x,&x),MAP_NULL_ARGUMENT);
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	TEST_EQUALS(res,mapPut(map,key,&x),MAP_NULL_ARGUMENT);
	TEST_EQUALS(res,mapPut(map,&x,element),MAP_NULL_ARGUMENT);

	for (int i = 1; i < N; ++i) {
		char j = (char) i;
		++j;
		TEST_EQUALS(res,mapPut(map,&i,&j),MAP_SUCCESS);
	}

	for (int i = 600; i < N; ++i) {//changing values in key 600-999
		char j = (char) N-i;
		++j;
		TEST_EQUALS(res,mapPut(map,&i,&j),MAP_SUCCESS);
		char *val = (char*)mapGet(map,&i);
		TEST_EQUALS(res,j,*val);
	}
	mapDestroy(map);

	return res;
}



static bool testmapGet(){
	bool res = true;
	Map map_n = NULL;
	TEST_EQUALS(res,mapGet(map_n,&res),NULL);
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
	MapKeyElement key = NULL;
	MapDataElement element = NULL;

	TEST_EQUALS(res,mapGet(map,key),NULL);
	TEST_EQUALS(res,mapGet(map,element),NULL);

	for (int i = 1; i < N; ++i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}
	for (int i = 1; i < N; ++i) {
		char j = (char) i;
		++j;
		char *val=(char*)mapGet(map,&i);
		TEST_EQUALS(res,*val,j);
	}

	mapDestroy(map);

	return res;
}



static bool testmapRemove(){
	bool res = true;
	Map map_n = NULL;
	int var;
	TEST_EQUALS(res,mapGet(map_n,&res),NULL);
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
	MapKeyElement key = NULL;
	TEST_EQUALS(res,mapRemove(map_n,&var),MAP_NULL_ARGUMENT);
	TEST_EQUALS(res,mapRemove(map,key),MAP_NULL_ARGUMENT);

	for (int i = 1; i < N; ++i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}

	for (int i = 1; i < N; ++i) {
		ASSERT_TEST(mapRemove(map,&i) == MAP_SUCCESS);
		TEST_EQUALS(res,mapContains(map,&i),false);
	}
	TEST_EQUALS(res,mapGetFirst(map),NULL);
	TEST_EQUALS(res,mapGetSize(map),0);
	mapDestroy(map);
	return res;
}




static bool testmapGetFirst(){
	bool res = true;
	Map map_n = NULL;
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
	TEST_EQUALS(res,mapGetFirst(map_n),NULL);
	TEST_EQUALS(res,mapGetFirst(map),NULL);
	for (int i = 1; i < N; ++i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}
	for (int i = 1; i < N-1; ++i) {
		ASSERT_TEST(mapRemove(map,&i) == MAP_SUCCESS);
		int* val=mapGetFirst(map);
		TEST_EQUALS(res,*val,i+1);
	}
	mapDestroy(map);
	return res;
}

static bool testmapGetNext(){
	bool res = true;
	Map map_n = NULL;
	TEST_EQUALS(res,mapGetNext(map_n),NULL);
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
	TEST_EQUALS(res,mapGetNext(map),NULL);

	for (int i = 1; i < N; ++i) {
		char j = (char) i;
		++j;
		ASSERT_TEST(mapPut(map,&i,&j) == MAP_SUCCESS);
	}
	int *val = mapGetFirst(map);
	for (int i = 1; i < N-1; ++i) {

		val = (int*)mapGetNext(map);
		TEST_EQUALS(res,*val,i+1);
	}
	val=(int*)mapGetNext(map);
	TEST_EQUALS(res,val,NULL);
	mapDestroy(map);
	return res;
}

static bool testmapClear(){
		bool res=true;
		Map map_n=NULL;
		TEST_EQUALS(res,mapClear(map_n),MAP_NULL_ARGUMENT);
		Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
		mapClear(map);
		TEST_EQUALS(res,mapGetSize(map),0);
		for (int i = 1; i < N; ++i) {
			char j = (char) i;
			++j;
			mapPut(map,&i,&j);
		}
		TEST_EQUALS(res,mapClear(map),MAP_SUCCESS);
		for (int i = 1; i < N; ++i) {
			char j = (char) i;
			++j;
			ASSERT_TEST(mapContains(map,&i)==false);
		}
		mapDestroy(map);
		return res;
}


int main(){
	RUN_TEST(testmapCreate);
	RUN_TEST(testmapDestroy);
	RUN_TEST(testmapCopy);
	RUN_TEST(testmapGetSize);
	RUN_TEST(testmapContains);
	RUN_TEST(testmapPut);
	RUN_TEST(testmapGet);
	RUN_TEST(testmapRemove);
	RUN_TEST(testmapGetFirst);
	RUN_TEST(testmapGetNext);
	RUN_TEST(testmapClear);

	return 0;
}

