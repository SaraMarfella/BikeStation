/* GENERAL INFORMATION
 / Project created by Sara Marfella IST188316
 / on May 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "dataManager.h"
#include "print.h"

// EVIL GLOBAL VARIABLES
int filter_hour_start = -1;
int filter_hour_end = -1;

char trips_file[MAX_SIZE];
char stations_file[MAX_SIZE];

// DECLARATION OF FUNCTIONS

// command line interface
void mainMenu(Trip*, Station*);
void listOfStationsMenu(Trip *, Station *);
void selectDataMenu();
void selectStationMenu(Station*, Trip*, int);
void statsMenu();

/*function main
 @param mode(-g or -t) and file (file travelers and file stations)
 */
int main (int argc, char *argv[]){
    
    // check for correct input
    if (argc != 4) {
        printf("Error - required arguments missing.\nPlease start with -t trips-file-name stations-file-name\n");
        return 0;
    }
    if (strcmp(argv[1], "-t") != 0) {
        printf("Sorry - only text mode is available, please start with -t\n");
        return 0;
    }
    
    strcpy(trips_file, argv[2]);
    strcpy(stations_file, argv[3]);
    
    Trip * allTrips = readTripsData(trips_file);
    Station * allStations = readStationData(stations_file);
    
    mainMenu(allTrips, allStations);
    
    return 0;
}

/* Command Line Interface */
void mainMenu(Trip * tripsList, Station * stationsList){
    int command;
    printf("\n * M A I N   M E N U * \n\n");
    printf("  [ 1 ]   Select the data\n\n");
    printf("  [ 2 ]   Print List of trips\n\n");
    printf("  [ 3 ]   Print List of stations\n\n");
    printf("  [ 4 ]   Print List of routes\n\n");
    printf("  [ 5 ]   Print List of statistics\n\n");
    printf("Any other key: Quit\n");
    scanf("%d", &command);
    switch (command) {
        case 1:
            selectDataMenu(tripsList, stationsList);
            break;
        case 2:
            printf("How many trips do you want to print? (0..32000, enter 0 for all)\n");
            scanf("%d", &command);
            printTripsList(tripsList, command);
            mainMenu(tripsList, stationsList);
            break;
        case 3:
            listOfStationsMenu(tripsList, stationsList);
            break;
        case 4:
            selectStationMenu(stationsList, tripsList, 0);
            break;
        case 5:
            statsMenu();
            mainMenu(tripsList, stationsList);
            break;
        default: return;
    }
}

void listOfStationsMenu(Trip * tripsList, Station * stationsList) {
    Station * filteredStations = countBikes(tripsList, stationsList, filter_hour_start, filter_hour_end);
    
    // option to ask the user: limit (0 for all), should print stations with no trips (1 yes, 2 no)
    printStationsList(filteredStations, 0, 1);
    mainMenu(tripsList, stationsList);
}

void selectDataMenu(Trip * filteredTrips, Station * allStations){
    
    int duration, command;
    
    printf("\n * Select the mode of your search * \n\n");
    printf("  [ 1 ]   Period of time (hour start, hour end)\n\n");
    printf("  [ 2 ]   Day of week\n\n");
    printf("  [ 3 ]   Max duration of trip (in seconds)\n\n");
    printf("  [ 4 ]   New Search (reset list)\n\n");
    printf("Any other key: Return to Main Menu\n");
    scanf("%d", &command);
    switch (command) {
        case 1:
            printf("Insert start time of trip (hour between 0..23):\n");
            scanf("%d", &filter_hour_start);
            printf("Insert end time of trip (hour between 0..23):\n");
            scanf("%d", &filter_hour_end);
            filteredTrips = selectTripsByTime(filteredTrips, filter_hour_start, filter_hour_end);
            mainMenu(filteredTrips, allStations);
            break;
        case 2:
            printf("Insert the day of trip:\n\n");
            printf("  [ 1 ]   Monday\n");
            printf("  [ 2 ]   Tuesday\n");
            printf("  [ 3 ]   Wednesday\n");
            printf("  [ 4 ]   Thursday\n");
            printf("  [ 5 ]   Friday\n");
            printf("  [ 6 ]   Saturday\n");
            printf("  [ 7 ]   Sunday\n");
            scanf("%d", &command);
            filteredTrips = selectTripsByDay(filteredTrips, command);
            mainMenu(filteredTrips, allStations);
            break;
        case 3:
            printf("Insert the max duration of trip (in seconds):\n");
            scanf("%d", &duration);
            filteredTrips = selectTripsByDuration(filteredTrips, duration);
            mainMenu(filteredTrips, allStations);
            break;
        case 4:
            filter_hour_end = -1;
            filter_hour_start = -1;
            Trip * allTrips = readTripsData(trips_file);
            mainMenu(allTrips, allStations);
            break;
        default:
            if (filteredTrips != NULL) mainMenu(filteredTrips, allStations);
            else {
                Trip * allTrips = readTripsData(trips_file);
                mainMenu(allTrips, allStations);
            }
            break;
    }
}

void selectStationMenu(Station * allStations, Trip * filteredTrips, int id) {
    int command;
    printf("Insert the id of the station:\n");
    scanf("%d", &id);
    filteredTrips = selectTripsByIdStation(filteredTrips, id);
    printf("How many routes do yo want to print? (0 for all)\n");
    scanf("%d", &command);
    printRoutesList(filteredTrips, allStations, id, command);
    Trip * allTrips = readTripsData(trips_file);
    mainMenu(allTrips, allStations);
}

void statsMenu() {
    printf("Sorry, the statistics are not yet implemented\n");
}
