#ifndef MTMFLIX_H_
#define MTMFLIX_H_

#include <stdio.h>
#include "mtm_ex3.h"

/*typedef enum {
	MTMFLIX_SUCCESS,
	MTMFLIX_OUT_OF_MEMORY,
	MTMFLIX_NULL_ARGUMENT,
	MTMFLIX_SERIES_ALREADY_EXISTS,
	MTMFLIX_SERIES_DOES_NOT_EXIST,
	MTMFLIX_NO_SERIES,
	MTMFLIX_USER_ALREADY_EXISTS,
	MTMFLIX_USER_DOES_NOT_EXIST,
	MTMFLIX_NO_USERS,
	MTMFLIX_USER_ALREADY_FRIEND,
	MTMFLIX_USER_NOT_FRIEND,
	MTMFLIX_ILLEGAL_VALUE,
	MTMFLIX_ILLEGAL_EPISODES_NUM,
	MTMFLIX_ILLEGAL_EPISODES_DURATION,
	MTMFLIX_ILLEGAL_SERIES_NAME
} MtmFlixResult;*/


typedef struct mtmFlix_t* MtmFlix;

MtmFlix mtmFlixCreate();
void mtmFlixDestroy(MtmFlix mtmflix);

MtmFlixResult mtmFlixAddSeries(MtmFlix mtmflix, const char* name, int episodesNum, Genre genre, int* ages, int episodesDuration);
MtmFlixResult mtmFlixRemoveSeries(MtmFlix mtmflix, const char* name);
MtmFlixResult mtmFlixSeriesJoin(MtmFlix mtmflix, const char* username, const char* seriesName);
MtmFlixResult mtmFlixSeriesLeave(MtmFlix mtmflix, const char* username, const char* seriesName);

MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix, const char* username, int age);
MtmFlixResult mtmFlixRemoveUser(MtmFlix mtmflix, const char* username);
MtmFlixResult mtmFlixAddFriend(MtmFlix mtmflix, const char* username1, const char* username2);
MtmFlixResult mtmFlixRemoveFriend(MtmFlix mtmflix, const char* username1, const char* username2);
MtmFlixResult mtmFlixGetRecommendations(MtmFlix mtmflix, const char* username, int count, FILE* outputStream);

MtmFlixResult mtmFlixReportSeries(MtmFlix mtmflix, int seriesNum, FILE* outputStream);
MtmFlixResult mtmFlixReportUsers(MtmFlix mtmflix, FILE* outputStream);

#endif /* MTMFLIX_H_ */
