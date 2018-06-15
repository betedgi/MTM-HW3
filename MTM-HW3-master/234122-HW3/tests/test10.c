#include "../mtmflix.h"
#include "test_utilities.h"
#include <stdio.h>

bool mtmFlixReportUsersTest(){
	MtmFlix mf = mtmFlixCreate();
	ASSERT_TEST(mf);

	ASSERT_TEST(MTMFLIX_NO_USERS == mtmFlixReportUsers(mf, stdout));


	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "Saifun", 19));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "Niko", 26));
	ASSERT_TEST(MTMFLIX_ILLEGAL_USERNAME == mtmFlixAddUser(mf, "Sai=fun", 2));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "Bae", 19));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "Ba", 36));

	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixReportUsers(mf, NULL));
	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixReportUsers(NULL, stdout));

	FILE* pFile = fopen ("diff/out10", "w");

	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixReportUsers(mf, pFile));

	fclose(pFile);

	mtmFlixDestroy(mf);
	return true;
}

int main() {
	RUN_TEST(mtmFlixReportUsersTest);
	return 0;
}