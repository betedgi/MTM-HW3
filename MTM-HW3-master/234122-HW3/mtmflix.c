#include <malloc.h>
#include "map.h"
#include "set.h"
#include "mtmflix.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "user.h"
#include "series.h"
#include "utilities.h"
#define MIN_AGE 0
#define MAX_AGE 1

struct mtmFlix_t{
    Map users; //a map which stores the users by name
    Map series; //a map which stores the series by name
};

/**
 * a function which calculate the rank of a particular series
 * @param G the number of series which the user likes with the same genre
 * @param L the avarage duration of the user's favorite series
 * @param CUR the duration of the ranked series
 * @param F the number of friends who likes the series
 * @return the rank of the particular series
 */
static float rankEquation(float G, float L, float CUR, float F);
/**
 * a function which receives a series and finds and calculate the series rank
 * @param mtmflix an ADT which stores users and series
 * @param userName the name of the user who looks for recommended series
 * @param seriesName the name of a series to find the rank to
 * @return the rank of the series
 */
static int calculateRank(MtmFlix mtmflix,  char* userName, char* seriesName);
/**
 * a fuction which prints the dialog for the recommendations
 * @param mtmflix a parameter from type mtmflix
 * @param count the maximum number of series to print
 * @param ranks_array a sorted array of ranks which correlates to a series
 *                    array
 * @param series_rank_array a sorted array of series sorted by rank
 * @param temp_user the user to recommend to
 * @param outputStream the output stream to print to (file or terminal)
 */
static void printToFile(MtmFlix mtmflix, int count,int* ranks_array,Series*
                        series_rank_array,User temp_user, FILE*
                        outputStream);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MTMFLIX FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/*
 * according to saifun it is unnecessary to write
 * comments to function which were defined in the exercise
 * */

MtmFlix mtmFlixCreate(){
    MtmFlix mtmflix=malloc(sizeof(*mtmflix));
    if(!mtmflix) return NULL;
    mtmflix->users=mapCreate(copyUser,stringCpy, destroyUser, free,
                             stringCompar);
    if(!mtmflix->users) {
        free(mtmflix);
        return NULL;
    }
    mtmflix->series=mapCreate(copySeries,stringCpy, destroySeries, free,
                              stringCompar);
    if(!mtmflix->series) {
        mapDestroy(mtmflix->users);
        free(mtmflix);
        return NULL;
    }
    return mtmflix;
}

void mtmFlixDestroy(MtmFlix mtmflix){
    if(!mtmflix) return;
    mapDestroy(mtmflix->users);
    mapDestroy(mtmflix->series);
    free(mtmflix);
}

MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix, const char* username, int age){
    if(!mtmflix || !username || !age ) return MTMFLIX_NULL_ARGUMENT;
    if(strlen(username)<1 || !stringCheckIfValid(username)) {
        return MTMFLIX_ILLEGAL_USERNAME;
    }
    char* temp_name=stringCpyConst(username);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    if(mapContains(mtmflix->users,temp_name)) {
        free(temp_name);
        return MTMFLIX_USERNAME_ALREADY_USED;
    }
    free(temp_name);
    if(age>MTM_MAX_AGE || age<MTM_MIN_AGE) return MTMFLIX_ILLEGAL_AGE;
    User user=createUser(username,age);
    if(!user) return MTMFLIX_OUT_OF_MEMORY;
    MapResult status=mapPut(mtmflix->users,userGetName(user),user);
    if(status!=MAP_SUCCESS) {
        destroyUserList(user);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    destroyUserList(user);
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveUser(MtmFlix mtmflix, const char* name){
    if(!mtmflix || !name) return MTMFLIX_NULL_ARGUMENT;
    char* temp_name=stringCpyConst(name);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    for(int i=0;i<mapGetSize(mtmflix->users);i++){
        char* iterator=mapGetFirst(mtmflix->users);
        for(int j=0;j<i;j++) {
            iterator = mapGetNext(mtmflix->users);
        }
        mtmFlixRemoveFriend(mtmflix,iterator,name);
    }
    MapResult status=mapRemove(mtmflix->users,temp_name);
    free(temp_name);
    if(status==MAP_ITEM_DOES_NOT_EXIST) return MTMFLIX_USER_DOES_NOT_EXIST;
    if(status==MAP_NULL_ARGUMENT) return MTMFLIX_NULL_ARGUMENT;
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixAddSeries(MtmFlix mtmflix, const char* name
        , int episodesNum, Genre genre, int* ages, int episodesDuration) {
    if (!mtmflix || !name ) {
        return MTMFLIX_NULL_ARGUMENT;
    }
    if (strlen(name)<1 || !stringCheckIfValid(name)) {
        return MTMFLIX_ILLEGAL_SERIES_NAME;
    }
    char* temp_name=stringCpyConst(name);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    if(mapContains(mtmflix->series,temp_name)) {
        free(temp_name);
        return MTMFLIX_SERIES_ALREADY_EXISTS;
    }
    free(temp_name);
    if (episodesNum <= 0) return MTMFLIX_ILLEGAL_EPISODES_NUM;
    if (episodesDuration <= 0) return MTMFLIX_ILLEGAL_EPISODES_DURATION;
    Series series = createSeries(name, episodesNum, genre, ages,
                                 episodesDuration);
    if (!series) return MTMFLIX_OUT_OF_MEMORY;
    MapResult status=mapPut(mtmflix->series,seriesGetName(series),series);
    if(status!=MAP_SUCCESS) {
        destroySeries(series);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    destroySeries(series);
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveSeries(MtmFlix mtmflix, const char* name){
    if(!mtmflix || !name) return MTMFLIX_NULL_ARGUMENT;
    char* temp_name=stringCpyConst(name);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    MAP_FOREACH(char*,user_iterator,mtmflix->users){
        mtmFlixSeriesLeave(mtmflix,user_iterator,name);
    }
    MapResult status=mapRemove(mtmflix->series,temp_name);
    free(temp_name);
    if(status==MAP_ITEM_DOES_NOT_EXIST) return MTMFLIX_SERIES_DOES_NOT_EXIST;
    if(status==MAP_NULL_ARGUMENT) return MTMFLIX_NULL_ARGUMENT;
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixReportSeries(MtmFlix mtmflix,
                                  int seriesNum, FILE* outputStream){
    const char* genres[] = { "SCIENCE_FICTION", "DRAMA", "COMEDY", "CRIME",
                             "MYSTERY", "DOCUMENTARY", "ROMANCE", "HORROR"};
    if(!mtmflix || !outputStream) return MTMFLIX_NULL_ARGUMENT;
    if( mapGetSize(mtmflix->series)==0) return MTMFLIX_NO_SERIES;
    Genre temp_genre=COMEDY;
    int index_in_genres=0;
    List series_list=makeList(mtmflix->series);
    if(!series_list) return MTMFLIX_OUT_OF_MEMORY;
    listSort(series_list,compareSeries);
    LIST_FOREACH(Series,iterator_series,series_list){
        if(temp_genre!=seriesGetGenre(iterator_series)){
            index_in_genres=0;
            temp_genre=seriesGetGenre(iterator_series);
        }
        if(seriesNum==0 || index_in_genres<seriesNum){
            fputs(mtmPrintSeries(seriesGetName(iterator_series)
                    ,genres[seriesGetGenre(iterator_series)]),outputStream);
        }
        index_in_genres++;
    }
    listDestroy(series_list);
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixReportUsers(MtmFlix mtmflix, FILE* outputStream){
    if(!mtmflix || !outputStream) return MTMFLIX_NULL_ARGUMENT;
    if(!mtmflix->users||mapGetSize(mtmflix->users)==0) return MTMFLIX_NO_USERS;
    List users_names_list=makeListNamesMap(mtmflix->users);
    listSort(users_names_list,stringCompar);
    LIST_FOREACH(char*,iterator,users_names_list){
        User iterator_user=mapGet(mtmflix->users,iterator);
        List friends=makeListNamesListUser(userGetFriends(iterator_user));
        List favShows=makeListNamesListSeries(userGetFavSeries(iterator_user));
        fputs(mtmPrintUser(userGetName(iterator_user),userGetAge(iterator_user)
                ,friends,favShows),outputStream);
        listDestroy(friends);
        listDestroy(favShows);
    }
    listDestroy(users_names_list);
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixSeriesJoin(MtmFlix mtmflix
        ,const char* username, const char* seriesName){
    if (!mtmflix || !username || !seriesName) return MTMFLIX_NULL_ARGUMENT;
    char* temp_name=stringCpyConst(username);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    User temp_user=mapGet(mtmflix->users,temp_name);
    free(temp_name);
    if(!temp_user) return MTMFLIX_USER_DOES_NOT_EXIST;
    temp_name=stringCpyConst(seriesName);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    Series temp_series = mapGet(mtmflix->series,temp_name);
    free(temp_name);
    if(!temp_series) return MTMFLIX_SERIES_DOES_NOT_EXIST;
    if(userGetAge(temp_user)<seriesGetAges(temp_series)[MIN_AGE]
       || userGetAge(temp_user)>seriesGetAges(temp_series)[MAX_AGE]) {
        return MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE;
    }
    Series series_iterator = listGetFirst(userGetFavSeries(temp_user));
    if(!series_iterator) {
        listInsertFirst(userGetFavSeries(temp_user),temp_series);
        return MTMFLIX_SUCCESS;
    }
    while (compareSeries(series_iterator,temp_series)) {

        series_iterator = listGetNext(userGetFavSeries(temp_user));
        if (!series_iterator) {
            listInsertFirst(userGetFavSeries(temp_user),temp_series);
            return MTMFLIX_SUCCESS;
        }
    }
    return MTMFLIX_SUCCESS;

}

MtmFlixResult mtmFlixSeriesLeave(MtmFlix mtmflix, const char* username,
                                 const char* seriesName){
    if (!mtmflix || !username || !seriesName) return MTMFLIX_NULL_ARGUMENT;
    char* temp_name=stringCpyConst(username);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    User temp_user=mapGet(mtmflix->users,temp_name);
    free(temp_name);
    if(!temp_user) return MTMFLIX_USER_DOES_NOT_EXIST;
    temp_name=stringCpyConst(seriesName);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    Series temp_series = mapGet(mtmflix->series,temp_name);
    free(temp_name);
    if(!temp_series) return MTMFLIX_SERIES_DOES_NOT_EXIST;
    Series series_iterator = listGetFirst(userGetFavSeries(temp_user));
    if(!series_iterator) return MTMFLIX_SUCCESS;
    while (compareSeries(series_iterator,temp_series)) {
        series_iterator = listGetNext(userGetFavSeries(temp_user));
        if (!series_iterator) return MTMFLIX_SUCCESS;
    }
    listRemoveCurrent(userGetFavSeries(temp_user));
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixAddFriend(MtmFlix mtmflix,
                               const char* username1 ,const char* username2){
    if (!mtmflix || !username1 || !username2) return MTMFLIX_NULL_ARGUMENT;
    char* temp_name1=stringCpyConst(username1);
    if(!temp_name1) return MTMFLIX_OUT_OF_MEMORY;
    User temp_user1=mapGet(mtmflix->users,temp_name1);
    free(temp_name1);
    if(!temp_user1) return MTMFLIX_USER_DOES_NOT_EXIST;
    char* temp_name2=stringCpyConst(username2);
    if(!temp_name2) return MTMFLIX_OUT_OF_MEMORY;
    User temp_user2=mapGet(mtmflix->users,temp_name2);
    free(temp_name2);
    if(!temp_user2) return MTMFLIX_USER_DOES_NOT_EXIST;
    if(!strcmp(username1,username2)) return MTMFLIX_SUCCESS;
    User user_iterator = listGetFirst(userGetFriends(temp_user1));
    if (!user_iterator) {
        listInsertLast(userGetFriends(temp_user1),temp_user2);
        return MTMFLIX_SUCCESS;
    }
    while (compareUsers(user_iterator,temp_user2)) {
        user_iterator = listGetNext(userGetFriends(temp_user1));
        if (!user_iterator) {
            listInsertLast(userGetFriends(temp_user1),temp_user2);
            return MTMFLIX_SUCCESS;
        }
    }
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveFriend(MtmFlix mtmflix,
                                  const char* username1, const char* username2){
    if (!mtmflix || !username1 || !username2) return MTMFLIX_NULL_ARGUMENT;
    char* temp_name1=stringCpyConst(username1);
    if(!temp_name1) return MTMFLIX_OUT_OF_MEMORY;
    User temp_user1=mapGet(mtmflix->users,temp_name1);
    free(temp_name1);
    if(!temp_user1) return MTMFLIX_USER_DOES_NOT_EXIST;
    char* temp_name2=stringCpyConst(username2);
    if(!temp_name2) return MTMFLIX_OUT_OF_MEMORY;
    User temp_user2=mapGet(mtmflix->users,temp_name2);
    free(temp_name2);
    if(!temp_user2) return MTMFLIX_USER_DOES_NOT_EXIST;
    User user_iterator = listGetFirst(userGetFriends(temp_user1));
    if(!user_iterator) return MTMFLIX_SUCCESS;
    while (compareUsers(user_iterator,temp_user2)) {
        user_iterator = listGetNext(userGetFriends(temp_user1));
        if (!user_iterator) return MTMFLIX_SUCCESS;
    }
    listRemoveCurrent(userGetFriends(temp_user1));
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixGetRecommendations(MtmFlix mtmflix
        , const char* username, int count, FILE* outputStream){
    if(!mtmflix || !username || !outputStream ||!mtmflix->series) {
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!count) {
        count=mapGetSize(mtmflix->series);
    }
    if(strlen(username)<1 || !stringCheckIfValid(username)) {
        return MTMFLIX_ILLEGAL_USERNAME;
    }
    char* temp_name=stringCpyConst(username);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    User temp_user = mapGet(mtmflix->users,temp_name);
    free(temp_name);
    if(!temp_user) return MTMFLIX_USER_DOES_NOT_EXIST;
    if(count<0) return MTMFLIX_ILLEGAL_NUMBER;
    int series_rank_temp;
    int* ranks_array=malloc(sizeof(int)*mapGetSize(mtmflix->series));
    for (int i = 0; i < mapGetSize(mtmflix->series) ;i++){
        ranks_array[i]=-1;
    }
    Series* series_rank_array=malloc(sizeof(*series_rank_array)
                                     *mapGetSize(mtmflix->series));
    for (int i = 0; i < mapGetSize(mtmflix->series) ;i++){
        series_rank_array[i]=NULL;
    }
    Series series_iterator = mapGet(mtmflix->series,
                                    mapGetFirst(mtmflix->series));
    temp_name=stringCpyConst(username);
    if(!temp_name) return MTMFLIX_OUT_OF_MEMORY;
    for (int i = 0; i <mapGetSize(mtmflix->series); ++i) {
        if(seriesGetAges(series_iterator)[MIN_AGE]<=userGetAge(temp_user)
           && userGetAge(temp_user)<=seriesGetAges(series_iterator)[MAX_AGE]){
            series_rank_temp=calculateRank(mtmflix
                    ,temp_name,seriesGetName(series_iterator));
            if(series_rank_temp){
                ranks_array[i]=series_rank_temp;
                series_rank_array[i]=copySeries(series_iterator);
            }
        }
        char* series_name=mapGetNext(mtmflix->series);
        if(series_name) series_iterator = mapGet(mtmflix->series,series_name);
    }
    free(temp_name);
    max_sort(ranks_array,series_rank_array,mapGetSize(mtmflix->series));
    printToFile(mtmflix,count,ranks_array,series_rank_array,
                temp_user,outputStream);
    free(ranks_array);
    for (int i = 0; i < mapGetSize(mtmflix->series) ;i++) {
        if(series_rank_array[i]){
            destroySeries(series_rank_array[i]);
        }
    }
    free(series_rank_array);
    return MTMFLIX_SUCCESS;
}



//~~~~~~~~~~~~~~~~~~~~~~STATIC FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~//


static float rankEquation(float G, float L, float CUR, float F){
    float result = (G*F)/(1+absvalue(CUR-L));
    return result;
}

static int calculateRank(MtmFlix mtmflix,  char* userName, char* seriesName){
    char* genres[] = { "SCIENCE_FICTION", "DRAMA", "COMEDY", "CRIME",
                       "MYSTERY", "DOCUMENTARY", "ROMANCE", "HORROR"};
    Series series_temp = mapGet(mtmflix->series,seriesName);
    User temp_user = mapGet(mtmflix->users,userName);
    float countG=0;
    float countF=0;
    float averageL;
    float sumL=0;
    User friend_temp = listGetFirst(userGetFriends(temp_user));
    List friend_series=userGetFavSeries(friend_temp);
    Series series_iterator = listGetFirst(friend_series);
    for (int i = 0; i < listGetSize(userGetFriends(temp_user)); ++i) {
        if (series_iterator) {
            countF++;
            while (compareSeries(series_iterator,series_temp)) {
                series_iterator = listGetNext(friend_series);
                if (series_iterator == NULL) {
                    countF--;
                    break;
                }
            }
        }
        friend_temp = listGetNext(userGetFriends(temp_user));
        friend_series=userGetFavSeries(friend_temp);
        series_iterator = listGetFirst(friend_series);
    }
    series_iterator = listGetFirst(userGetFavSeries(temp_user));
    for (int i = 0; i < listGetSize(userGetFavSeries(temp_user)); ++i){
        sumL+=seriesGetDuration(series_iterator);
        if (!stringCompar(genres[seriesGetGenre(series_iterator)]
                              ,genres[seriesGetGenre(series_temp)])) countG++;
        series_iterator = listGetNext(userGetFavSeries(temp_user));;
    }

    averageL=sumL/listGetSize(userGetFavSeries(temp_user));
    return (int)rankEquation(countG,averageL,
                             (float)seriesGetDuration(series_temp),countF);
}

static void printToFile(MtmFlix mtmflix, int count,int* ranks_array,
                     Series* series_rank_array,User temp_user, FILE*
                                                           outputStream){
    const char* genres[] = { "SCIENCE_FICTION", "DRAMA", "COMEDY", "CRIME",
                             "MYSTERY", "DOCUMENTARY", "ROMANCE", "HORROR"};
    Series series_iterator;
    for(int i=0;i<mapGetSize(mtmflix->series);i++){
        if(ranks_array[i]==-1 || count<=0) break;
        series_iterator=listGetFirst(userGetFavSeries(temp_user));
        if (!series_iterator) {
            count--;
            fputs(mtmPrintSeries(seriesGetName(series_rank_array[i])
                   ,genres[seriesGetGenre(series_rank_array[i])]),outputStream);
            break;
        }
        while (compareSeries(series_iterator,series_rank_array[i])) {
            if (!series_iterator) {
                count--;
                fputs(mtmPrintSeries(seriesGetName(series_rank_array[i]),
                                  genres[seriesGetGenre(series_rank_array[i])]),
                                                                  outputStream);
                break;
            }
            series_iterator = listGetNext(userGetFavSeries(temp_user));
        }
    }
}


