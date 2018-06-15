#ifndef MTM_EX3_H_
#define MTM_EX3_H_

#include <stdio.h> // For FILE*
// You should avoid including unnecessary header files
#include <stdbool.h>
#include "list.h"


#define MTM_MAX_AGE 127
#define MTM_MIN_AGE 5

/**
* This type defines all return values for the system.
* These codes should be used for calling mtmPrintErrorMessage to report
* error messages.
*/
typedef enum {
	MTMFLIX_SUCCESS,
	MTMFLIX_OUT_OF_MEMORY, // You should exit program after this error
	MTMFLIX_CANNOT_OPEN_FILE, // You should exit program after this error
	MTMFLIX_NULL_ARGUMENT,
	MTMFLIX_ILLEGAL_USERNAME,
	MTMFLIX_USERNAME_ALREADY_USED,
	MTMFLIX_ILLEGAL_AGE,
	MTMFLIX_USER_DOES_NOT_EXIST,
	MTMFLIX_ILLEGAL_SERIES_NAME,
	MTMFLIX_SERIES_ALREADY_EXISTS,
	MTMFLIX_ILLEGAL_EPISODES_NUM,
	MTMFLIX_ILLEGAL_EPISODES_DURATION,
	MTMFLIX_SERIES_DOES_NOT_EXIST,
	MTMFLIX_NO_SERIES,
	MTMFLIX_ILLEGAL_NUMBER,
	MTMFLIX_NO_USERS,
	MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE
} MtmFlixResult;


typedef enum {
        SCIENCE_FICTION,
        DRAMA,
        COMEDY,
        CRIME,
        MYSTERY,
        DOCUMENTARY,
        ROMANCE,
        HORROR
} Genre;

/**
* mtmPrintErrorMessage - prints an error message into the given output channel
* Use the standard error channel to print error messages.
*
* @param errorChannel - File descriptor for the error channel
* @param code - Error code to print message for.
*/
void mtmPrintErrorMessage(FILE* errorChannel, MtmFlixResult code);

/**
* Returns the string which describes the series.
* This string should be printed without any alteration.
* @param seriesName - The name of the series.
* @param seriesGenre - The the genre of the series.
* @returns NULL in case of any error creating the string (you should notify on
* 			an error using MTM_OUT_OF_MEMORY
*/
const char* mtmPrintSeries(const char* seriesName, const char* seriesGenre);

/**
* Returns the string which describes the user.
* This string should be printed without any alteration.
* @param userName - The name of the user.
* @param age - The age of the user.
* @param friendsNames - a list of strings containing the names of the friends of the user
* @param seriesNames - a list of strings containing the names of the series that the user likes
* @returns NULL in case of any error creating the string (you should notify on
* 			an error using MTM_OUT_OF_MEMORY
*/
const char* mtmPrintUser(const char* userName, int age, List friendsNames, List seriesNames);

#endif /* MTM_EX3_H_ */
