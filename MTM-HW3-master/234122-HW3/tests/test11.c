#include "test_utilities.h"
#include "../mtmflix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* 
* This is a simple test that checks only some of the functions.
* It is called "integration test" because it checks that all the functions work together as expected.
* You are Very Very welcome to add some lines to this integration test, and share it with others!
* You can find the diff output files in the folder named "diff"
*/

static bool test11(){
	MtmFlix m = mtmFlixCreate();
	int ageA[2] = {16, 77};
	int ageB[2] = {3, 180};
	
	ASSERT_TEST(mtmFlixAddSeries(m, "Sheldon!", 23, COMEDY, ageA, 25) == MTMFLIX_ILLEGAL_SERIES_NAME);
	ASSERT_TEST(mtmFlixAddSeries(m, "PartnersInCrime", 3, MYSTERY, ageB, 120) == MTMFLIX_SUCCESS);
	mtmFlixAddSeries(m, "TheFlash", 108, SCIENCE_FICTION, ageB, 30);
	mtmFlixAddSeries(m, "HarryPotter1", 1, DRAMA, ageB, 99);
	mtmFlixAddSeries(m, "HarryPotter2", 1, DRAMA, ageB, 99);
	mtmFlixAddSeries(m, "Secret", 9, MYSTERY, ageB, 98);
	mtmFlixAddSeries(m, "HorsesLoveStory", 10, ROMANCE, ageB, 22);
	mtmFlixAddSeries(m, "HarryPotter3", 1, DRAMA, ageB, 99);
	mtmFlixAddSeries(m, "TwoAndAHalfMen", 100, COMEDY, ageA, 22);
	 
	FILE* f1 = fopen("diff/out11", "w");
	assert(f1);
	
	mtmFlixReportSeries(m, 0, f1);
	ASSERT_TEST(mtmFlixRemoveSeries(m, "HarryPotter2") == MTMFLIX_SUCCESS);
	mtmFlixReportSeries(m, 1, f1);
	
	ASSERT_TEST(mtmFlixAddUser(m, "I am trying to get in!", 23) == MTMFLIX_ILLEGAL_USERNAME);
	ASSERT_TEST(mtmFlixAddUser(m, "Arnav14", 10) == MTMFLIX_SUCCESS);
	mtmFlixAddUser(m, "AmitLove", 24);
	ASSERT_TEST(mtmFlixAddUser(m, "SifSifN", 3) == MTMFLIX_ILLEGAL_AGE);
	ASSERT_TEST(mtmFlixSeriesJoin(m, "AmitLove", "PartnersInCrime") == MTMFLIX_SUCCESS);
	mtmFlixSeriesJoin(m, "AmitLove", "TwoAndAHalfMen");
	mtmFlixSeriesJoin(m, "Arnav14", "TheFlash");
	mtmFlixSeriesJoin(m, "Arnav14", "HarryPotter3");
	mtmFlixSeriesJoin(m, "Arnav14", "HarryPotter1");
	ASSERT_TEST(mtmFlixSeriesJoin(m, "Arnav14", "HarryPotter2") == MTMFLIX_SERIES_DOES_NOT_EXIST);
	ASSERT_TEST(mtmFlixSeriesJoin(m, "SifSifN", "PartnersInCrime") == MTMFLIX_USER_DOES_NOT_EXIST);

	mtmFlixReportUsers(m, f1);
	
	fclose(f1);
	mtmFlixDestroy(m);
    return true;
}

int main(){
	RUN_TEST(test11);
	return 0;
}
