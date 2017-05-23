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
    int max_bikesIn;
    int min_bikesIn;
    int max_bikesOut;
    int min_bikesOut;
    float avg_bikesIn;
    float avg_bikesOut;
    struct Station *next;
}Station;


// DECLARATION OF FUNCTIONS
void mainMenu(Trip*, Station*);

// command line interface
void selectData();
void printTripsList(Trip*, int);
void printStationsList(Station*, int);

// data management
Trip* selectTripsByTime(Trip*, int, int);
Trip* selectTripsByDuration(Trip*, int);
Trip* selectTripsByDay(Trip*, int);
Trip* selectTripsByIdStation(Trip*, int);
void selectStation (Trip*, int);

// file reader
void readTripsData();
void readStationData();

// helpers
Trip* copyTripToList(Trip*, Trip*);
Station * countBikes(Trip*, Station*);
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
    // stationsHead = countBikes(tripsHead, stationsHead);
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


Station * countBikes(Trip *headTrips, Station *headStations) {
    
    //debug code
    //int tempCounter = 0;
    
    // cycle through stations
    struct Station *auxStations = stationsHead;
    while (auxStations != NULL) {
        //debug code
        //tempCounter++;
        //printf("\n%d - Counting bikes for Station ID: %d Name: %s\n", tempCounter, auxStations->id, auxStations->full_name);
        
        // initialize station counters
        int max_bikesIn = -1;
        int min_bikesIn = -1;
        int max_bikesOut = -1;
        int min_bikesOut = -1;
        int total_bikesIn = 0;
        int total_bikesOut = 0;
        
        float avg_bikesIn = 0;
        float avg_bikesOut = 0;
        // initialize hourly counters
        int counter_bikesIn = 0;
        int counter_bikesOut = 0;
        // initialize hours counters
        int hour_start=0;
        int hour_end=1;
        
        //first line checker
        bool shouldInitMin = true;
        
        // for each station, get all trips in or out of the station
        struct Trip * tripsByStation = selectTripsByIdStation(headTrips, auxStations->id);
        
        // cycle through trips per hour
        while(hour_start < 24){
            
            //printf(" - Processing trips between: %d and %d\n", hour_start, hour_end);
            
            // handle midnight
            if (hour_end == 24) hour_end = 0;
            
            // get all trips within the hour
            Trip * filteredList = selectTripsByTime(tripsByStation, hour_start, hour_end);
            while (filteredList != NULL) {
                
                //printf("   - Trip id: %ld\n", filteredList->id);
                
                // for each trip in or out of the station, count bikes in and out
                if (filteredList->id_start_station == auxStations->id) {
                    counter_bikesOut++;
                    total_bikesOut++;
                }
                if (filteredList->id_final_station == auxStations->id) {
                    counter_bikesIn++;
                    total_bikesIn++;
                }
                
                filteredList = filteredList->next;
            }
            
            // check max
            if (max_bikesIn < counter_bikesIn) max_bikesIn = counter_bikesIn;
            if (max_bikesOut < counter_bikesOut) max_bikesOut = counter_bikesOut;
            
            // initialize min counters
            if (shouldInitMin) {
                min_bikesIn = max_bikesIn;
                min_bikesOut = max_bikesOut;
                shouldInitMin = false;
            }
            
            // check min
            if (min_bikesIn > counter_bikesIn) min_bikesIn = counter_bikesIn;
            if (min_bikesOut > counter_bikesOut) min_bikesOut = counter_bikesOut;
            
            
            //debug code
            //printf("Max In: %d | Min In: %d | TOT IN: %d | Max Out: %d | Min Out: %d | TOT OUT: %d\n",
            //      max_bikesIn, min_bikesIn, total_bikesIn, max_bikesOut, min_bikesOut, total_bikesOut);
            
            // increase hours counters
            hour_start++;
            hour_end++;
            // reset houlry coutners
            counter_bikesIn = 0;
            counter_bikesOut = 0;
        }
        //printf("Total IN: %d | Total OUT: %d\n", counter_bikesIn, counter_bikesOut);
        // calc average
        avg_bikesIn = (float)total_bikesIn/24;
        avg_bikesOut = (float)total_bikesOut/24;
        
        //debug code
        //printf("Avg IN: %f | Avg OUT: %f\n", avg_bikesIn, avg_bikesOut);
        
        // reset hour counters
        hour_start = 0;
        hour_end = 1;
        
        // save counters data to Stations list
        auxStations->max_bikesIn    = max_bikesIn;
        auxStations->max_bikesOut   = max_bikesOut;
        auxStations->min_bikesIn    = min_bikesIn;
        auxStations->min_bikesOut   = min_bikesOut;
        auxStations->avg_bikesIn    = avg_bikesIn;
        auxStations->avg_bikesOut   = avg_bikesOut;
        
        auxStations = auxStations->next;
    }
    return stationsHead;
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
            //printf("How many stations do you want to print? (0..32000, enter 0 for all)\n");
            //scanf("%d", &command);
            countBikes(tripsList, stationsList);
            printStationsList(stationsList, 0);
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
        bool shouldSave = false;
        
        
        // if hour start < hour end, then check if trip is between time span
        // if user enters trips from 8 to 9, we take all trips between 8:00 and 8:59
        if (hour_start < hour_end) {
            if (  (aux->start.hour >= hour_start) && (aux->start.hour < hour_end) &&
                (aux->end.hour >= hour_start) && (aux->end.hour < hour_end) )  {
                shouldSave = true;
            }
        }
        // handle scenario: hour start > hour end
        // if user enters trips from 18 to 17, we take all trips between 18:00 and 23:59, and all trips between 0:00 and 17:59
        else if (hour_start > hour_end) {
            if ( ( (aux->start.hour >= hour_start) && (aux->end.hour <= 23) ) || ( (aux->start.hour >= 0) && (aux->end.hour < hour_end)) ){
                // get all trips between hour_start and 23:59.
                // get all trips between 0 and hour_end.
                shouldSave = true;
            }
        }
        // handle scenario: hour start = hour end
        // if user enters trips from 16 to 16, we take all trips between 16:00 and 16:59
        else {
            if ( (aux->start.hour == hour_start) && (aux->end.hour == hour_end) ) {
                shouldSave = true;
            }
        }
        // save item if it should
        if (shouldSave) {
            filteredTripsHead = copyTripToList(filteredTripsHead, aux);
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
            
            filteredTripsHead = copyTripToList(filteredTripsHead, aux);
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
            
            filteredTripsHead = copyTripToList(filteredTripsHead, aux);
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
            filteredTripsHead = copyTripToList(filteredTripsHead, aux);
        }
        aux = aux->next;
    }
    return filteredTripsHead;
}

Trip* copyTripToList(Trip * filteredTripsHead, Trip * aux) {
    
    Trip* trip = (Trip*)malloc(sizeof(Trip));
    
    trip->id = aux->id;
    strcpy(trip->bike, aux->bike);
    trip->duration = aux->duration;
    trip->end = aux->end;
    trip->start = aux->start;
    trip->gender = aux->gender;
    trip->id_final_station = aux->id_final_station;
    trip->id_start_station = aux->id_start_station;
    trip->type = aux->type;
    trip->year_birthday = aux->year_birthday;
    
    trip->next = filteredTripsHead;
    filteredTripsHead = trip;
    
    return filteredTripsHead;
}

// prints trips list to screen
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

// prints stations list to screen
void printStationsList(Station *head, int limit) {
    struct Station *aux = head;
    int lineCounter = 0;
    printf(" ID | Name   | Latitude   | Longitude  | MaxIn | MinIn | Avg In | MaxOut | MinOut | Avg Out\n");
    
    while (aux != NULL) {
        //printf("%d\n", lineCounter);
        
        printf(" %02d | ", aux->id);
        printf("%s | ", aux->name);
        printf(" %f | ", aux->latitude);
        printf("%f | ", aux->longitude);
        printf("  %03d | ", aux->max_bikesIn);
        printf("   %02d | ", aux->min_bikesIn);
        printf(" %05.2f | ", aux->avg_bikesIn);
        printf("   %03d | ", aux->max_bikesOut);
        printf("    %02d | ", aux->min_bikesOut);
        printf(" %05.2f \n", aux->avg_bikesOut);
        
        aux = aux->next;
        lineCounter++;
        if ((limit != 0) && (lineCounter >= limit)) {
            return;
        }
    }
}

