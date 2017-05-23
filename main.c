/* GENERAL INFORMATION
 / Project created by Sara Marfella IST188316
 / on May 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

// CONSTANT
#define MAX_SIZE 100 // max size of a string
#define REGISTERED 1 // the registered member
#define CASUAL 0 // the casual user
#define MALE 1 // the male gender
#define FEMALE 0 // the female gender
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
    struct Station *next;
}Station;

// DECLARATION OF FUNCTIONS
void readTripsData();
void readStationData();
void mainMenu(Trip*, Station*);
void selectData();
void printTripsList(Trip*, int);
Trip* selectTripsByTime(Trip*, int, int);
Trip* selectTripsByDuration(Trip*, int);
Trip* selectTripsByDay(Trip*, int);
Trip* selectTripsByIdStation(Trip*, int);
void selectStation (Trip*, int);
int calculateWeekDateFromDate(int, int, int);


// GLOBAL VARIABLES
struct Trip * tripsHead = NULL;
struct Station * stationsHead = NULL;

/*function main
 @param mode(-g or -t) and file (file travelers and file stations)
 */
int main (int argc, char *argv[]){

    //bool shouldQuit = false;

    // check for correct input
    /*if (argc != 3) {
     printf("Error - required arguments missing");
     }
     if ((!argv[1] == '-t')||(!argv[1] == '-g')) {
     printf("Error - only text mode or graphic mode is available, please start with -t or -g");
     //call of function that read the files
     readFiles();
     }*/
    readTripsData();
    readStationData();
    mainMenu(tripsHead, stationsHead);

    //struct Trip * filteredTrips = selectTripsByTime(tripsHead, 8, 9);

    //printTripsList(filteredTrips, 0);
    return 0;
}

// Functions that open and read the files into the linked lists

//void readFiles(char *argv[]){
//  readTripsData(argv[qualcosa]);
//}

void readTripsData(/*char *filename*/){
    char line[1024];
    char *token;
    char *separators = ", /:";
    int lineNumber = 0;
    int fieldCounter = 0;

    //open file for reading
    FILE *fileOne = fopen( "hubway_trips_v2.csv", "r" );
    if ( fileOne == 0 ) printf( "Error - Could not open file\n" );
    else {
        while(fgets(line, sizeof line, fileOne) != NULL){   // read each line

            lineNumber++;                                   // keep line count for convenience
            token = strtok(line, separators);               // Split the line into parts
            fieldCounter = 0;                               // make sure field counter is 0
            Trip* trip = (Trip*)malloc(sizeof(Trip));       // Allocation of memory

            while (token != NULL) {                         // cycle through fields
                // printf("Field: %d\n", fieldCounter);
                switch (fieldCounter) {
                    case 0:  trip->id = atoi(token);                break;
                    case 1:  trip->duration = atoi(token);          break;
                    case 2:  trip->start.month = atoi(token);       break;
                    case 3:  trip->start.day = atoi(token);         break;
                    case 4:  trip->start.year = atoi(token);        break;
                    case 5:  trip->start.hour = atoi(token);        break;
                    case 6:  trip->start.minute = atoi(token);      break;
                    case 7:  /*"seconds field" to be ignored*/      break;
                    case 8:  trip->id_start_station = atoi(token);  break;
                    case 9:  trip->end.month = atoi(token);         break;
                    case 10: trip->end.day = atoi(token);           break;
                    case 11: trip->end.year = atoi(token);          break;
                    case 12: trip->end.hour = atoi(token);          break;
                    case 13: trip->end.minute = atoi(token);        break;
                    case 14: /*"seconds field" to be ignored*/      break;
                    case 15: trip->id_final_station = atoi(token);  break;
                    case 16:
                        // handle missing bike id on line 609
                        if (strlen(token)-1 > 7) {
                            if (strcmp(token, "Registered") !=0) {
                                trip->type = REGISTERED;
                            } else {
                                trip->type = CASUAL;
                            }
                            // add one to field counter so we skip this field
                            fieldCounter++;
                            break;
                        }
                        strcpy(trip->bike, token);
                        break;
                    case 17:
                        if (strcmp(token, "Registered") !=0) {
                            trip->type = REGISTERED;
                        } else {
                            trip->type = CASUAL;
                        }
                        break;
                    case 18:
                        if (trip->type == REGISTERED) {
                            trip->year_birthday = atoi(token);
                        }
                        break;
                    case 19:
                        if (trip->type == REGISTERED) {
                            if (strcmp(token, "Male") !=0) {
                                trip->gender = MALE;
                            } else {
                                trip->gender = FEMALE;
                            }
                        }
                        fieldCounter = -1; // Reset fields counter for new line
                        break;
                    default: break;
                }
                // printf ("%s\n",token);
                fieldCounter++;
                token = strtok (NULL, separators);
            }
            // add new trip to linked list
            trip->next = tripsHead;
            tripsHead = trip;
        }
        fclose(fileOne);
    }
}

void readStationData(/*char *filename*/){
    char line[1024];
    char *token;
    char *separators = ",";
    int lineNumber = 0;
    int fieldCounter = 0;

    //open file for reading
    FILE *fileTwo = fopen( "hubway_stations.csv", "r" );
    if ( fileTwo == 0 ) printf( "Error - Could not open file\n" );
    else {
        while(fgets(line, sizeof line, fileTwo) != NULL){
            lineNumber++;                                           // keep line count for convenience
            token = strtok(line, separators);                       // Split the line into parts
            fieldCounter = 0;                                       // make sure field counter is 0
            Station* station = (Station*)malloc(sizeof(Station));   // Allocation of memory
            if (lineNumber != 1) {                                  // Skip first line with headers
                while (token != NULL) {                             // cycle through fields
                    // printf("Field: %d\n", fieldCounter);
                    switch (fieldCounter) {
                        case 0:  station->id = atoi(token);             break;
                        case 1:  strcpy(station->name, token);          break;
                        case 2:  strcpy(station->full_name, token);     break;
                        case 3:  strcpy(station->municipal, token);     break;
                        case 4:  station->latitude = atof(token);       break;
                        case 5:  station->longitude = atof(token);      break;
                        case 6:  station->status = atoi(token);         break;
                        default: break;
                    }
                    // printf ("%s\n",token);
                    fieldCounter++;
                    token = strtok (NULL, separators);
                }
                // add new trip to linked list
                station->next = stationsHead;
                stationsHead = station;
            }
        }
        fclose(fileTwo);
    }
}

void printTripsList(Trip *head, int limit) {

    struct Trip *aux = head;
    int lineCounter = 0;
    while (aux != NULL) {
        printf("* * * * * * * * %d\n", lineCounter);
        printf("id: %ld\n", aux->id);
        printf("Duration: %d\n", aux->duration);
        printf("Start Date: %d/%d/%d %d:%d\n", aux->start.month, aux->start.day, aux->start.year, aux->start.hour, aux->start.minute);
        printf("Start Station ID: %d\n", aux->id_start_station);
        printf("End Date: %d/%d/%d %d:%d\n", aux->end.month, aux->end.day, aux->end.year, aux->end.hour, aux->end.minute);
        printf("Final Station ID: %d\n", aux->id_final_station);
        printf("Bike ID: %s\n", aux->bike);
        printf("User Type: %d\n", aux->type);
        printf("Year of Birth: %d\n", aux->year_birthday);
        printf("Gender: %d\n", aux->gender);
        aux = aux->next;
        lineCounter++;
        if ((limit != 0) && (lineCounter >= limit)) {
            return;
        }
    }
}

void printStationsList(Station *head, int limit) {
    struct Station *aux = head;
    int lineCounter = 0;
    while (aux != NULL) {
        printf("* * * * * * * * %d\n", lineCounter);
        printf("id: %d\n", aux->id);
        printf("name : %s\n", aux->name);
        printf("latitude: %f\n", aux->latitude);
        printf("longitude: %f\n", aux->longitude);

        aux = aux->next;
        lineCounter++;
        if ((limit != 0) && (lineCounter >= limit)) {
            return;
        }
    }
}

void printBike(Trip *head, Station *head) {

    struct Trip *aux = head;
    int cunterBike=0;
    int hour_start=0;
    int hour_end=1;

    while(hour_start != 23){
        selectTripsByTime(tripsHead, hour_start, hour_end);

        strcpy(trip->bike, aux->bike);
        counterBike++;
        printf("numero di bici:%d", couterBike);

        printTripsList(tripsHead, 10);


        //printf("Bike ID: %s\n", aux->bike);
        hour_start++;
        hour_end++;
    }
    printf("bikeCounter:%d\n", bikeCounter);

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
        case 1: selectData(tripsList); break;
        case 2:
            printf("How many trips do you want to print? (0..32000, enter 0 for all)\n");
            scanf("%d", &command);
            printTripsList(tripsList, command);
            mainMenu(tripsList, stationsList);
            break;
        case 3:
            printf("How many stations do you want to print? (0..32000, enter 0 for all)\n");
            scanf("%d", &command);
            printStationsList(stationsList, command);
            printBike(tripsList, stationsList);
            mainMenu(tripsList, stationsList);
            break;
        case 4:
            selectStation(tripsList, 0);
            printf("how many station do yo want to print?\n");
            scanf("%d", &command);
            printTripsList(tripsList, command);
            mainMenu(tripsList, stationsList);
            break;
        case 5: break;
        default: return;
    }
}

void selectStation(Trip * filteredTrips, int id){
    printf("Insert the id of the station:\n");
    scanf("%d", &id);
    filteredTrips = selectTripsByIdStation(tripsHead, id);
    mainMenu(filteredTrips, stationsHead);
}

void selectData(Trip * filteredTrips){

    int hour_start, hour_end, duration, command;

    printf("\n * Select the mode of your search * \n\n");
    printf("  [ 1 ]   Period of time (hour start, hour end)\n\n");
    printf("  [ 2 ]   Day of week\n\n");
    printf("  [ 3 ]   Max duration of trip (in seconds)\n\n");
    printf("Any other key: Return to Main Menu\n");
    scanf("%d", &command);
    switch (command) {
        case 1:
            printf("Insert start time of trip (hour between 0..24):\n");
            scanf("%d", &hour_start);
            printf("Insert end time of trip (hour between 0..24):\n");
            scanf("%d", &hour_end);
            filteredTrips = selectTripsByTime(filteredTrips, hour_start, hour_end);
            mainMenu(filteredTrips, stationsHead);
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
            mainMenu(filteredTrips, stationsHead);
            break;
        case 3:
            printf("Insert the max duration of trip (in seconds):\n");
            scanf("%d", &duration);
            filteredTrips = selectTripsByDuration(filteredTrips, duration);
            mainMenu(filteredTrips, stationsHead);
            break;
        default:
            if (filteredTrips != NULL) mainMenu(filteredTrips, stationsHead);
            else mainMenu(tripsHead, stationsHead);
            break;
    }
}

Trip* selectTripsByTime(Trip * sourceListHead, int hour_start, int hour_end) {
    struct Trip *aux = sourceListHead;
    struct Trip *filteredTripsHead = NULL;
    while (aux != NULL) {

        // Only save the item to the list if the start hour and end hour are within the parameters
        // If the end hour is the same as the end parameter, we only want trips that ended in
        // the first minute of the hour, otherwise we consider the trips outside the desired set
        // so if user enters trips from 8 to 9, we take all trips between 8:00 and 9:00

        if ( ( (aux->start.hour >= hour_start) && (aux->start.hour < hour_end) &&
              (aux->end.hour >= hour_start) && (aux->end.hour < hour_end) ) ||
            ( (aux->start.hour >= hour_start) && (aux->end.hour == hour_end) && (aux->end.minute == 0) ) ) {

            Trip* trip = (Trip*)malloc(sizeof(Trip));

            trip->id = aux->id;
            strcpy(trip->bike, aux->bike);
            trip->duration = aux->duration;
            trip->end = aux->end;
            trip->start = aux->start;
            trip->gender = aux->gender;
            trip->id_final_station = aux->id_final_station;
            trip->id_start_station = aux->id_final_station;
            trip->type = aux->type;
            trip->year_birthday = aux->year_birthday;

            // add new trip to filtered linked list
            trip->next = filteredTripsHead;
            filteredTripsHead = trip;
        }
        aux = aux->next;
    }
    return filteredTripsHead;
}

Trip* selectTripsByDuration(Trip * sourceListHead, int duration) {
    struct Trip *aux = sourceListHead;
    struct Trip *filteredTripsHead = NULL;
    while (aux != NULL) {
        if (aux->duration <= duration) {

            Trip* trip = (Trip*)malloc(sizeof(Trip));

            trip->id = aux->id;
            strcpy(trip->bike, aux->bike);
            trip->duration = aux->duration;
            trip->end = aux->end;
            trip->start = aux->start;
            trip->gender = aux->gender;
            trip->id_final_station = aux->id_final_station;
            trip->id_start_station = aux->id_final_station;
            trip->type = aux->type;
            trip->year_birthday = aux->year_birthday;

            trip->next = filteredTripsHead;
            filteredTripsHead = trip;
        }
        aux = aux->next;
    }
    return filteredTripsHead;
}

// Calculate the day of the week this current trip was in
// Source: https://stackoverflow.com/questions/6054016/c-program-to-find-day-of-week-given-date
int calculateWeekDateFromDate(int y, int m, int d) {
    int weekday  = (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;
    return weekday;
}

Trip* selectTripsByDay(Trip * sourceListHead, int selectedDay){

    struct Trip *aux = sourceListHead;
    struct Trip *filteredTripsHead = NULL;

    if (selectedDay == 7) {
        selectedDay = 0;
    }

    while (aux != NULL) {

        // Check if day of the current trip is == to the selected day
        if ((calculateWeekDateFromDate(aux->start.year, aux->start.month, aux->start.day) == selectedDay) ||
            (calculateWeekDateFromDate(aux->end.year, aux->end.month, aux->end.day) == selectedDay)){

            Trip* trip = (Trip*)malloc(sizeof(Trip));

            trip->id = aux->id;
            strcpy(trip->bike, aux->bike);
            trip->duration = aux->duration;
            trip->end = aux->end;
            trip->start = aux->start;
            trip->gender = aux->gender;
            trip->id_final_station = aux->id_final_station;
            trip->id_start_station = aux->id_final_station;
            trip->type = aux->type;
            trip->year_birthday = aux->year_birthday;

            trip->next = filteredTripsHead;
            filteredTripsHead = trip;
        }
        aux = aux->next;
    }
    return filteredTripsHead;
}

Trip* selectTripsByIdStation(Trip * sourceListHead, int id) {
    struct Trip *aux = sourceListHead;
    struct Trip *filteredTripsHead = NULL;
    while (aux != NULL) {
        if ((aux->id_final_station == id)||(aux->id_start_station == id)) {

            Trip* trip = (Trip*)malloc(sizeof(Trip));

            trip->id_final_station = aux->id_final_station;
            trip->id_start_station = aux->id_final_station;

            trip->next = filteredTripsHead;
            filteredTripsHead = trip;
        }
        aux = aux->next;
    }
    return filteredTripsHead;
}
