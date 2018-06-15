#include "../mtmflix.h"
#include "test_utilities.h"
#include <stdio.h>

bool mtmFlixRemoveSeriesTest(){
	MtmFlix mf = mtmFlixCreate();
	ASSERT_TEST(mf);

	int ages[2] = {10, 30};

	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixRemoveSeries(NULL, "Saifun"));
	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixRemoveSeries(mf, NULL));

	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddSeries(mf, "Saifun", 20, ROMANCE, ages, 40));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddSeries(mf, "saifun", 20, DRAMA, ages, 40));

	ASSERT_TEST(MTMFLIX_SERIES_ALREADY_EXISTS == mtmFlixAddSeries(mf, "Saifun", 10, MYSTERY, ages, 10));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixRemoveSeries(mf, "Saifun"));
	ASSERT_TEST(MTMFLIX_SERIES_DOES_NOT_EXIST == mtmFlixRemoveSeries(mf, "Saifun"));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddSeries(mf, "Saifun", 21, ROMANCE, ages, 30));

	mtmFlixDestroy(mf);
	return true;
}

int main() {
	RUN_TEST(mtmFlixRemoveSeriesTest);
	return 0;
}