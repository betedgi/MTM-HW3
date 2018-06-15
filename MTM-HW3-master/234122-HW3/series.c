#include "list.h"
#include "map.h"
#include "mtm_ex3.h"
#include "series.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define MIN_AGE_INDEX 0
#define MAX_AGE_INDEX 1
struct series_t{
    char* name;
    int episodesNum;
    Genre genre;
    int* ages;
    int episodeDuration;
};
Series createSeries(const char* name, int episodesNum,
                           Genre genre, int* ages, int episodeDuration) {
    if(!name || !episodesNum ||!episodeDuration) return NULL;
    int* ages_t=malloc(sizeof(int)*2);
    if(!ages_t) return NULL;
    if( !ages || *ages<MTM_MIN_AGE)
        *ages_t = MTM_MIN_AGE;
    else *ages_t = *ages;
    if((ages+1) || *(ages+1)>MTM_MAX_AGE)
        *(ages_t+1) = MTM_MAX_AGE;
    else *(ages_t+1)=*(ages+1);
    Series series=malloc(sizeof(*series));
    if(!series) return NULL;
    series->name=malloc(sizeof(char)*(strlen(name)+1));
    if(!series->name) {
        free(ages);
        free(series);
        return NULL;
    }
    strcpy(series->name,name);
    series->episodeDuration=episodeDuration;
    series->episodesNum=episodesNum;
    series->genre=genre;
    series->ages=ages_t;
    return series;
}

void destroySeries(void* series){
    if(!series) return;
    Series temp_series=(Series)series;
    free(temp_series->name);
    free(temp_series->ages);
    free(temp_series);
}

void* copySeries(void* series) {
    Series temp_series=(Series)series;
    Series new_series=createSeries(temp_series->name,temp_series->episodesNum
            ,temp_series->genre,temp_series->ages,temp_series->episodeDuration);
    if(!new_series) return NULL;
    return new_series;
}

Genre seriesGetGenre(Series series){
    return series->genre;
}

char* seriesGetName(Series series){
    return series->name;
}

int seriesGetDuration(Series series){
    return series->episodeDuration;
}

int seriesGetNumOfEpisodes(Series series){
    return series->episodesNum;
}

int* seriesGetAges(Series series){
    return series->ages;
}

void swapSeries(Series* series_1, Series* series_2){
    Series temp=*series_2;
    *series_2=*series_1;
    *series_1=temp;
}


int compareSeries(void* series_1,void* series_2){
    const char* genres[] = { "SCIENCE_FICTION", "DRAMA", "COMEDY", "CRIME",
                             "MYSTERY", "DOCUMENTARY", "ROMANCE", "HORROR"};
    Series series_temp1=(Series)series_1;
    if (!series_temp1) {
        return -1;
    }
    Series series_temp2=(Series)series_2;
    if (!series_temp2) {
        return 1;
    }
    int cmpGenre=strcmp(genres[seriesGetGenre(series_temp1)]
            ,genres[seriesGetGenre(series_temp2)]);
    if(cmpGenre!=0) return cmpGenre;
    return strcmp(seriesGetName(series_temp1),seriesGetName(series_temp2));
}

List makeListNamesListSeries(List list){
    if(!list || !listGetSize(list)) return NULL;
    List list_temp=listCreate(stringCpy,free);
    LIST_FOREACH(Series,iterator,list)
    {

        listInsertFirst(list_temp,iterator->name);
    }
    return list_temp;
}


List makeList(Map map){
    if(!map || !mapGetSize(map)) return NULL;
    List list=listCreate(copySeries,destroySeries);
    MAP_FOREACH(char*,iterator,map){
        listInsertFirst(list,mapGet(map,iterator));
    }
    return list;
}

void swapInt(int* num_1, int* num_2){
    int temp=*num_2;
    *num_2=*num_1;
    *num_1=temp;
}

void max_sort(int* arrRank,Series arrSeriesRank[], int sizeArr)
{
    for ( int i=0; i <sizeArr; i++ ) {
        int i_max = index_of_max(arrRank + i, arrSeriesRank + i,
                                 sizeArr - i);
        swapInt(&arrRank[i_max + i], &arrRank[i]);
        swapSeries(&arrSeriesRank[i_max + i], &arrSeriesRank[i]);
    }
}

int index_of_max(int* arrRank,Series arrSeriesRank[], int sizeArr)
{
    int i, i_max = 0;
    for ( i = 0; i < sizeArr; i++ ){
        if ((arrRank[i_max] < arrRank[i]) ||
            (arrRank[i_max] == arrRank[i] && arrRank[i]!=-1 &&
             strcmp(seriesGetName(arrSeriesRank[i])
                     ,seriesGetName(arrSeriesRank[i_max]))<=0)){
            i_max = i;
        }
    }

    return i_max;
}
