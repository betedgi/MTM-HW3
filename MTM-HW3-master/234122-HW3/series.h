//
// Created by Ilan Etedgi on 5/28/2018.
//

#ifndef WET_SERIES_H
#define WET_SERIES_H

#include "mtm_ex3.h"
#include "map.h"

typedef struct series_t*  Series ;
/**
 * a fuction which creates a parameter from type Series
 * @param name the name of the series
 * @param episodesNum the number of episodes the series has
 * @param genre the genre of the series
 * @param ages the scope of ages the series is for
 * @param episodeDuration the avarage duration of each episode in the series
 * @return a parameter from type Series which stores the values given
 */
Series createSeries(const char* name, int episodesNum,
                    Genre genre, int* ages, int episodeDuration);
/**
 * a fuction which deallocates the memory the series uses and removes the series
 * @param series the series which needs to be desroyred
 */
void destroySeries(void* series);
/**
 * a fuction which compares two seriers by values
 * @param series_1 a parameter from type series
 * @param series_2 a parameter from type series
 * @return a value which indicates which series is "greater" by their values
 */
int compareSeries( void* series_1, void* series_2);
/**
 * a function which copies a series by value
 * @param series a series which needs to be copied
 * @return a copy of the series given
 */
void* copySeries(void* series);
/**
 * a function which receives a series and return its name
 * @param series a parameter from type series
 * @return the given series's age
 */
char* seriesGetName(Series series);
/**
 * a function which receives a series and return its genre
 * @param series a parameter from type series
 * @return the given series's genre
 */
Genre seriesGetGenre(Series series);
/**
 * a function which receives a series and return its duration
 * @param series a parameter from type series
 * @return the given series's duration
 */
int seriesGetDuration(Series series);
/**
 * a function which receives a series and return its number of episodes
 * @param series a parameter from type series
 * @return the given series's number of episodes
 */
int seriesGetNumOfEpisodes(Series series);
/**
 * a function which receives a series and return its scope of ages
 * @param series a parameter from type series
 * @return the given series's number of scope of ages
 */
int* seriesGetAges(Series series);
/**
 * a fuction which makes a list of names from a list of series
 * @param list a list of series
 * @return a list of names by the given list of series
 */
List makeListNamesListSeries(List list);
/**
 * a fuction which makes a list of series from a map
 * @param map a map which stores series
 * @return a list of series by the map given
 */
List makeList(Map map);
/**
 * a fuction which sorts two connected arrays by specific value
 * @param arrRank an array of ranks for each series
 * @param arrSeriesRank an array of series
 * @param sizeArr the size of the arrays
 */
void max_sort(int* arrRank,Series arrSeriesRank[], int sizeArr);
/**
 * a fuction which return the index of the max value in an array
 * @param arrRank an array of ranks for each series
 * @param arrSeriesRank an array of series
 * @param sizeArr the size of the arrays
 * @return the index of the value which the greatest value
 */
int index_of_max(int* arrRank,Series arrSeriesRank[], int sizeArr);
/**
 * a function which swaps two series by address
 * @param series_1 a parameter from type series
 * @param series_2 a parameter from type series
 */
void swapSeries(Series* series_1, Series* series_2);
/**
 * a fuction which swaps two numbers by address
 * @param num_1 a number from type int
 * @param num_2 a number from type int
 */
void swapInt(int* num_1, int* num_2);
#endif //WET_SERIES_H
