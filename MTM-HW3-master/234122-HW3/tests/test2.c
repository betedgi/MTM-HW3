#include "../mtmflix.h"
#include "test_utilities.h"
#include <stdio.h>

bool mtmFlixRemoveUserTest(){
	MtmFlix mf = mtmFlixCreate();
	ASSERT_TEST(mf);

	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixRemoveUser(NULL, "Somename"));
	ASSERT_TEST(MTMFLIX_NULL_ARGUMENT == mtmFlixRemoveUser(mf, NULL));

	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "Saifun", 19));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "Niko", 26));

	ASSERT_TEST(MTMFLIX_USER_DOES_NOT_EXIST == mtmFlixRemoveUser(mf, "niko"));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixRemoveUser(mf, "Niko"));
	ASSERT_TEST(MTMFLIX_USER_DOES_NOT_EXIST == mtmFlixRemoveUser(mf, "Niko"));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixAddUser(mf, "Niko", 26));
	ASSERT_TEST(MTMFLIX_USERNAME_ALREADY_USED == mtmFlixAddUser(mf, "Niko", 22));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixRemoveUser(mf, "Niko"));
	ASSERT_TEST(MTMFLIX_USER_DOES_NOT_EXIST == mtmFlixRemoveUser(mf, "Niko"));
	ASSERT_TEST(MTMFLIX_SUCCESS == mtmFlixRemoveUser(mf, "Saifun"));

	mtmFlixDestroy(mf);
	return true;
}

int main() {
	RUN_TEST(mtmFlixRemoveUserTest);
	return 0;
}