
#ifndef dataManager_h
#define dataManager_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

// CONSTANTS
#define MAX_SIZE 100 // max size of a string
#define REGISTERED 1 // the registered member
#define CASUAL 0 // the casual user
#define MALE 1 // the male gender
#define FEMALE 2 // the female gender
#define EXISTING 1
#define REMOVED 0

//ALL THE STRUCT

// define a type of date
typedef struct{
    int month;
    int day;
    int year;
    int hour;
    int minute;
}Date;

// Linked List of Trips
typedef struct Trip{
    long int id;
    int duration; // in seconds
    Date start;
    int id_start_station;
    Date end;
    int id_final_station;
    char bike[7];
    int type; // the user can be casual or members
    int year_birthday; // only in case of members
    int gender; // female or man only in case of members
    struct Trip *next;
}Trip;

// Linked List of Stations
typedef struct Station{
    int id;
    char name[7];
    char full_name[MAX_SIZE];
    char municipal[MAX_SIZE];
    double latitude;
    double longitude;
    int status; // existing or removed
    int max_bikesIn;
    int min_bikesIn;
    int max_bikesOut;
    int min_bikesOut;
    float avg_bikesIn;
    float avg_bikesOut;
    struct Station *next;
}Station;

// LinkedList of Routes
typedef struct Route{
    int total;
    int id_start_station;
    char name_start_station[7];
    char id_final_station;
    char name_final_station[7];
    struct Route *next;
}Route;

// FUNCTIONS

// file readers
Station * readStationData(char*);
Trip * readTripsData(char*);

// list creators
Route * createRoutesList(Trip*, Station*, int);
Station * countBikes(Trip*, Station*, int, int);

// list filters
Trip* selectTripsByTime(Trip*, int, int);
Trip* selectTripsByDuration(Trip*, int);
Trip* selectTripsByDay(Trip*, int);
Trip* selectTripsByIdStation(Trip*, int);

// helpers
Trip* copyTripToList(Trip*, Trip*);
int calculateWeekDateFromDate(int, int, int);
void sortedInsert(Route**, Route*);
char * getStationNameById(int, Station*);

#endif /* dataManager_h */
