#include "../mtmflix.h"
#include "test_utilities.h"
#include <stdio.h>

bool mtmFlixAddUserTest(){
	MtmFlix mf = mtmFlixCreate();
	ASSERT_TEST(mf);

	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixAddUser(NULL, "Saifun", 19));
	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixAddUser(mf, NULL, 19));

	ASSERT_TEST(MTMFLIX_ILLEGAL_USERNAME == mtmFlixAddUser(mf, "S_aifun", 19));
	ASSERT_TEST(MTMFLIX_ILLEGAL_USERNAME == mtmFlixAddUser(mf, "Sai=fun", 2));
	ASSERT_TEST(MTMFLIX_ILLEGAL_USERNAME == mtmFlixAddUser(mf, "Say fun", 7));
	ASSERT_TEST(MTMFLIX_ILLEGAL_USERNAME == mtmFlixAddUser(mf, "", 20));

	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "Sa1fun", 19));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "sa1fun", 19));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "N1ko", MTM_MAX_AGE));

	ASSERT_TEST(MTMFLIX_USERNAME_ALREADY_USED == mtmFlixAddUser(mf, "sa1fun", 39));
	ASSERT_TEST(MTMFLIX_ILLEGAL_USERNAME == mtmFlixAddUser(mf, "Sai=fun", 9));

	ASSERT_TEST(MTMFLIX_ILLEGAL_AGE == mtmFlixAddUser(mf, "Niko", MTM_MIN_AGE - 1));
	ASSERT_TEST(MTMFLIX_ILLEGAL_AGE == mtmFlixAddUser(mf, "Niko", MTM_MAX_AGE + 1));
	ASSERT_TEST(MTMFLIX_ILLEGAL_AGE == mtmFlixAddUser(mf, "Niko", -1));

	mtmFlixDestroy(mf);
	return true;
}

int main() {
	RUN_TEST(mtmFlixAddUserTest);
	return 0;
}