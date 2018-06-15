#include <malloc.h>
#include <assert.h>
#include "map_mtm.h"
#include <stdio.h>

typedef struct node_t{
    MapKeyElement key;
    MapDataElement data;
    struct node_t* next;
}*Node;

struct Map_t{
        Node iterator;
        Node info;

        copyMapDataElements copyDataFuction;
        copyMapKeyElements copyKeyFuction;
        freeMapDataElements freeDataFunction;
        freeMapKeyElements  freeKeyFuction;
        compareMapKeyElements compareKeysFuction;

};

static Node createNode(MapKeyElement key ,MapDataElement data) {
    Node node=malloc(sizeof(*node));
    node->next=NULL;
    node->data=data;
    node->key=key;
    return node;
}

static void destroyNode(Map map,Node node) {
    if(!node || !map) return;
    if(node->key) map->freeKeyFuction(node->key);
    if(node->data) map->freeDataFunction(node->data);
    free(node);
}

Map mapCreate(copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements) {
    if(!copyDataElement || !copyKeyElement || !compareKeyElements || !freeDataElement || !freeKeyElement) return NULL;
    Map map=(Map)malloc(sizeof(*map));
    if(!map) return NULL;
    map->iterator=NULL;
    map->info=NULL;
    map->compareKeysFuction=compareKeyElements;
    map->copyDataFuction=copyDataElement;
    map->copyKeyFuction=copyKeyElement;
    map->freeDataFunction=freeDataElement;
    map->freeKeyFuction=freeKeyElement;
    return map;
}

void mapDestroy(Map map){
    if(!map) return;
    Node temp=map->info;
    while(temp) {
        Node toDelete=temp;
        temp=temp->next;
        destroyNode(map,toDelete);
    }
        free(map);


}


Map mapCopy(Map map){
    if(!map) return NULL;
    Map new_map=malloc(sizeof(*new_map));
    if(!new_map) {
        mapDestroy(new_map);
        return NULL;
    }
    Node* previous_temp=NULL;
    Node temp=map->info;
    Node* new_temp=&new_map->info;
    new_map->freeKeyFuction=map->freeKeyFuction;
    new_map->freeDataFunction=map->freeDataFunction;
    new_map->compareKeysFuction=map->compareKeysFuction;
    new_map->copyDataFuction=map->copyDataFuction;
    new_map->copyKeyFuction=map->copyKeyFuction;
    new_map->iterator=map->iterator;
    while(temp)
    {
        *new_temp=malloc(sizeof(**new_temp));
        if(!(*new_temp)) {
            mapDestroy(new_map);
            return NULL;
        }

        (*new_temp)->key=map->copyKeyFuction(temp->key);
        if(!(*new_temp)->key) {
            mapDestroy(new_map);
            return NULL;
        }
        (*new_temp)->data=map->copyDataFuction(temp->data);
        if(!(*new_temp)->data) {
            mapDestroy(new_map);
            return NULL;
        }
        previous_temp=&(*new_temp);
        new_temp=&(*new_temp)->next;
        temp=temp->next;
    }
    (*previous_temp)->next=NULL;
    return new_map;
}

int mapGetSize(Map map){
    if(!map) return -1;
    int size=0;
    Node temp=map->info;
    while(temp){
        size++;
        temp=temp->next;
    }
    return size;
}

bool mapContains(Map map, MapKeyElement element){
    if(!map || !element) return NULL;
    Node temp=map->info;
    while(temp) {
        if(map->compareKeysFuction(temp->key,element)==0) return true;
        if(map->compareKeysFuction(temp->key,element)>0) return false;
        temp=temp->next;
    }
    return false;
}
void print(Map map){
    Node temp=map->info;
    while(temp){
        printf(" key: %d , data: %d ",*(int*)temp->key,*(int*)temp->data);
        temp=temp->next;
    }
}
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    if(!map || !keyElement || !dataElement) return MAP_NULL_ARGUMENT;
    Node node=map->info;

    if(!node){
        Node temp=createNode(map->copyKeyFuction(keyElement),map->copyDataFuction(dataElement));
        if(!temp || !temp->key || !temp->data){
            destroyNode(map,node);
            return MAP_OUT_OF_MEMORY;
        }
        map->info=temp;
        map->iterator=0;
        return MAP_SUCCESS;
    }

    if(mapContains(map,keyElement)){
        while(map->compareKeysFuction(node->key,keyElement)!=0){
            node=node->next;
        }
        node->data=map->copyDataFuction(dataElement);
        if(!node->data){
            return MAP_OUT_OF_MEMORY;
        }
        map->iterator=0;
        return MAP_SUCCESS;
    }
    else {
        if(map->compareKeysFuction(node->key, keyElement) > 0){
            Node temp=createNode(map->copyKeyFuction(keyElement),map->copyDataFuction(dataElement));
            if(!temp || !temp->key || !temp->data){
                destroyNode(map,temp);
                return MAP_OUT_OF_MEMORY;
            }
            temp->next=node;
            map->info=temp;
            map->iterator=NULL;
            return MAP_SUCCESS;
        }
        while (node->next && map->compareKeysFuction(node->next->key, keyElement) < 0) {
            node = node->next;
        }
        Node temp=createNode(map->copyKeyFuction(keyElement),map->copyDataFuction(dataElement));
        if(!temp || !temp->key || !temp->data){
            destroyNode(map,temp);
            return MAP_OUT_OF_MEMORY;
        }
        if(node->next) temp->next=node->next;
        node->next=temp;
        map->iterator=NULL;
        return MAP_SUCCESS;
    }
}

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    if(!map || !keyElement ) return NULL;
    Node node=map->info;
    if(mapContains(map,keyElement)){
        while(!map->compareKeysFuction(node->key,keyElement)){
            node=node->next;
        }
        return map->copyDataFuction(node->data);
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement){
    if(!map || !keyElement ) return MAP_NULL_ARGUMENT;
    Node node=map->info;
    Node* temp=NULL;
    Node toDelete=NULL;
    if(mapContains(map,keyElement)){
        if(mapGetSize(map)==1){
            destroyNode(map,node);
            map->info=NULL;
            map->iterator=NULL;
            return MAP_SUCCESS;
        }
        if(map->compareKeysFuction(node->key,keyElement)){
            toDelete=node;
            map->info=node->next;
            destroyNode(map,toDelete);
            map->iterator=NULL;
            return MAP_SUCCESS;
        }
        while(node->next && !map->compareKeysFuction(node->next->key,keyElement)){
            node=node->next;
        }
        if(!node->next) {
            temp=&node;
            destroyNode(map,node);
            *temp=NULL;
            map->iterator=NULL;
            return MAP_SUCCESS;
        }
        toDelete=node->next;
        if(node->next->next) {
            node->next = node->next->next;
        }
        else {
            node->next = NULL;
        }
        destroyNode(map,toDelete);
        map->iterator=NULL;
        return MAP_SUCCESS;
    }

    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map){
    if(!map || !map->info) return NULL;
    map->iterator=map->info;
    return map->info->key;
}


MapKeyElement mapGetNext(Map map){
    if(!map || !map->iterator || !map->iterator->next) return NULL;
    map->iterator=map->iterator->next;
    return map->iterator->key;
}

MapResult mapClear(Map map){
    if(!map) return MAP_NULL_ARGUMENT;
    Node node=map->info;
    Node toDelete=NULL;
    while(node){
        toDelete=node;
        node=node->next;
        destroyNode(map,toDelete);
    }
    map->info=NULL;
    return MAP_SUCCESS;
}







