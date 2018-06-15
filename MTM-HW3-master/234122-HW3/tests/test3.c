#include "../mtmflix.h"
#include "test_utilities.h"
#include <stdio.h>

bool mtmFlixAddSeriesTest(){
	MtmFlix mf = mtmFlixCreate();
	ASSERT_TEST(mf);

	int ages[2] = {10, 30};

	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixAddSeries(NULL, "Saifun", 20, ROMANCE, ages, 40));
	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixAddSeries(mf, NULL, 20, ROMANCE, ages, 40));

	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddSeries(mf, "Niko", 20, HORROR, NULL, 40));

	ASSERT_TEST(MTMFLIX_ILLEGAL_SERIES_NAME == mtmFlixAddSeries(mf, "Niko_", 20, HORROR, ages, 40));
	ASSERT_TEST(MTMFLIX_ILLEGAL_SERIES_NAME == mtmFlixAddSeries(mf, "Ni ko", 20, HORROR, ages, 40));

	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddSeries(mf, "Saifun", 20, ROMANCE, ages, 40));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddSeries(mf, "saifun", 20, DRAMA, ages, 40));

	ASSERT_TEST(MTMFLIX_SERIES_ALREADY_EXISTS == mtmFlixAddSeries(mf, "Saifun", 10, MYSTERY, ages, 10));

	ASSERT_TEST(MTMFLIX_ILLEGAL_EPISODES_NUM == mtmFlixAddSeries(mf, "Virgore", 0, ROMANCE, ages, 40));
	ASSERT_TEST(MTMFLIX_ILLEGAL_EPISODES_NUM == mtmFlixAddSeries(mf, "Virgore", -1, ROMANCE, ages, 40));

	ASSERT_TEST(MTMFLIX_ILLEGAL_EPISODES_DURATION == mtmFlixAddSeries(mf, "Virgore", 20, ROMANCE, ages, 0));
	ASSERT_TEST(MTMFLIX_ILLEGAL_EPISODES_DURATION == mtmFlixAddSeries(mf, "Virgore", 20, ROMANCE, ages, -1));

	mtmFlixDestroy(mf);
	return true;
}

int main() {
	RUN_TEST(mtmFlixAddSeriesTest);
	return 0;
}