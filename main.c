/* GENERAL INFORMATION
 / Project created by Sara Marfella IST188316
 / on May 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

// CONSTANT
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

// EVIL GLOBAL VARIABLES
int filter_hour_start = -1;
int filter_hour_end = -1;


// DECLARATION OF FUNCTIONS
void mainMenu(Trip*, Station*);

// command line interface
void selectData();
void printTripsList(Trip*, int);
void printStationsList(Station*, int, int);

// data management
Trip* selectTripsByTime(Trip*, int, int);
Trip* selectTripsByDuration(Trip*, int);
Trip* selectTripsByDay(Trip*, int);
Trip* selectTripsByIdStation(Trip*, int);
void selectStation (Station*, Trip*, int);

// file reader
Trip * readTripsData();
Station * readStationData();

// helpers
Trip* copyTripToList(Trip*, Trip*);
Station * countBikes(Trip*, Station*);
int calculateWeekDateFromDate(int, int, int);

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
    
    //Trip * filteredTrips = selectTripsByTime(tripsHead, 8, 22);
    //Trip * filteredTrips2 = selectTripsByIdStation(filteredTrips, 3);
    //printTripsList(filteredTrips2, 0);
    //stationsHead = countBikesSaraCiProva(tripsHead, stationsHead);
    //printTripsList(filteredTrips, 0 );
    
    Trip * allTrips = readTripsData();
    Station * allStations = readStationData();
    
    mainMenu(allTrips, allStations);
    
    //struct Trip * filteredTrips = selectTripsByTime(tripsHead, 8, 9);
    
    //printTripsList(filteredTrips, 0);
    return 0;
}

// Functions that open and read the files into the linked lists

//void readFiles(char *argv[]){
//  readTripsData(argv[qualcosa]);
//}

Trip * readTripsData(/*char *filename*/){
    char line[1024];
    char *token;
    char *separators = ", /:\n";
    int lineNumber = 0;
    int fieldCounter = 0;
    
    Trip * tripsHead = NULL;
    
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
                //printf("%d %s\n", fieldCounter, token);
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
                        if (strlen(token)-1 > 8) {
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
                        if (strcmp(token, "Registered") == 0) {
                            trip->type = REGISTERED;
                        } else {
                            trip->type = CASUAL;
                        }
                        break;
                    case 18:
                        trip->year_birthday = atoi(token);
                        break;
                    case 19:
                        if (token[0] == 'M') {
                            trip->gender = MALE;
                        } else if (token[0] == 'F') {
                            trip->gender = FEMALE;
                        } else {
                            trip->gender = 0;
                        }
                        //fieldCounter = -1; // Reset fields counter for new line
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
    return tripsHead;
}

Station * readStationData(/*char *filename*/){
    char line[1024];
    char *token;
    char *separators = ",";
    int lineNumber = 0;
    int fieldCounter = 0;
    
    Station * stationsHead = NULL;
    
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
    return stationsHead;
}

Station * countBikes(Trip *tripsList, Station *stationsList) {
    struct Station * auxStations = stationsList;
    
    
    while (auxStations != NULL) {
        
        Trip *trips = selectTripsByIdStation(tripsList, auxStations->id);
        
        // initialize counters and hours
        int tripsCount = 0;
        int inTotal = 0;
        int outTotal = 0;
        
        int counterIn[24] = {0};
        int counterOut[24] = {0};
        
        if (trips != NULL) {
            while (trips != NULL) {
                tripsCount++;
                if (trips->id_start_station == auxStations->id) {
                    counterOut[trips->start.hour]++;
                }
                if (trips->id_final_station == auxStations->id) {
                    counterIn[trips->end.hour]++;
                }
                
                trips = trips->next;
            }
        }
        int maxIn = counterIn[0];
        int minIn = counterIn[0];
        int maxOut = counterOut[0];
        int minOut = counterOut[0];
        
        int start = 0;
        int end = 24;
        if (filter_hour_start != -1 && filter_hour_end != -1) {
            start = filter_hour_start;
            end = filter_hour_end;
            
            maxIn = counterIn[filter_hour_start];
            minIn = counterIn[filter_hour_start];
            maxOut = counterOut[filter_hour_start];
            minOut = counterOut[filter_hour_start];
        }
        // find max and min within selected time range
        for (int i = start; i<end; i++){
            if (maxIn < counterIn[i]) maxIn = counterIn[i];
            if (maxOut < counterOut[i]) maxOut = counterOut[i];
            if (minIn > counterIn[i]) minIn = counterIn[i];
            if (minOut > counterOut[i]) minOut = counterOut[i];
            
            printf("CounterOut[%d] %d; minOut %d\n", i,counterOut[i],minOut);
            
            inTotal += counterIn[i];
            outTotal += counterOut[i];
        }
        
        // save counters data to Stations list
        auxStations->max_bikesIn    = maxIn;
        auxStations->max_bikesOut   = maxOut;
        auxStations->min_bikesIn    = minIn;
        auxStations->min_bikesOut   = minOut;
        //auxStations->avg_bikesIn    = avg_bikesIn;
        //auxStations->avg_bikesOut   = avg_bikesOut;
        
        
        printf("Station ID: %d Total Trips: %d / In %d  / Out %d\n", auxStations->id, tripsCount, inTotal, outTotal);
        auxStations = auxStations->next;
    }
    return stationsList;
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
            selectData(tripsList, stationsList);
            break;
        case 2:
            printf("How many trips do you want to print? (0..32000, enter 0 for all)\n");
            scanf("%d", &command);
            printTripsList(tripsList, command);
            mainMenu(tripsList, stationsList);
            break;
        case 3:
            //printf("How many stations do you want to print? (0..32000, enter 0 for all)\n");
            //scanf("%d", &command);
            printf("Show stations with no travels for selected data? (1 - yes / 2 - no)\n");
            scanf("%d", &command);
            Station * filteredStations = countBikes(tripsList, stationsList);
            printStationsList(filteredStations, 0, command);
            mainMenu(tripsList, stationsList);
            break;
        case 4:
            selectStation(stationsList, tripsList, 0);
            break;
        case 5: break;
        default: return;
    }
}

void selectStation(Station * allStations, Trip * filteredTrips, int id){ //case 4
    int command;
    printf("Insert the id of the station:\n");
    scanf("%d", &id);
    filteredTrips = selectTripsByIdStation(filteredTrips, id);
    printf("How many trips do yo want to print? (0 for all)\n");
    scanf("%d", &command);
    printTripsList(filteredTrips, command);
    Trip * allTrips = readTripsData();
    mainMenu(allTrips, allStations);
}

void selectData(Trip * filteredTrips, Station * allStations){
    
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
            Trip * allTrips = readTripsData();
            mainMenu(allTrips, allStations);
            break;
        case 5:
            break;
        default:
            if (filteredTrips != NULL) mainMenu(filteredTrips, allStations);
            else {
                Trip * allTrips = readTripsData();
                mainMenu(allTrips, allStations);
            }
            break;
    }
}

Trip* selectTripsByTime(Trip * sourceListHead, int hour_start, int hour_end) {
    Trip *aux = sourceListHead;
    Trip *filteredTripsHead = NULL;
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
    int counter=0;
    struct Trip *aux = sourceListHead;
    struct Trip *filteredTripsHead = NULL;
    while (aux != NULL) {
        
        if ((aux->id_final_station == id) || (aux->id_start_station == id)) {
            filteredTripsHead = copyTripToList(filteredTripsHead, aux);
            counter++;
        }
        
        aux = aux->next;
    }
    //printf("Total Trips Found: %d\n", counter);
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
    int lineCounter = 1;
    
    printf("##### | ID      | Dur    | Start: Date      | St | End: Date        | St | Bike ID | Type | Year | Gender\n");
    
    while (aux != NULL) {
        printf("%05d | ", lineCounter);
        printf(" %07ld | ", aux->id);
        printf("%06d | ", aux->duration);
        printf("%02d/%02d/%d %02d:%02d | ", aux->start.month, aux->start.day, aux->start.year, aux->start.hour, aux->start.minute);
        printf("%02d | ", aux->id_start_station);
        printf("%02d/%02d/%d %02d:%02d | ", aux->end.month, aux->end.day, aux->end.year, aux->end.hour, aux->end.minute);
        printf("%02d | ", aux->id_final_station);
        printf(" %s | ", aux->bike);
        if (aux->type == REGISTERED) {
            printf("Reg. | ");
        } else {
            printf("Cas. | ");
        }
        if (aux->year_birthday != 0) {
            printf("%04d |", aux->year_birthday);
        }
        if (aux->gender == MALE) {
            printf(" M");
        } else if (aux->gender == FEMALE) {
            printf(" F");
        }
        printf("\n");
        aux = aux->next;
        
        lineCounter++;
        
        if ((limit != 0) && (lineCounter >= limit)) {
            return;
        }
    }
}

// prints stations list to screen
void printStationsList(Station *head, int limit, int printWithNoTrips) {
    struct Station *aux = head;
    int lineCounter = 0;
    printf(" ID | Name   | Latitude   | Longitude  | MaxIn | MinIn | Avg In | MaxOut | MinOut | Avg Out\n");
    
    while (aux != NULL) {
        
        // only print if the station has some trips, or if it should print even with no trips
        if ((printWithNoTrips == 2 && aux->max_bikesIn != 0 && aux->max_bikesOut != 0) || printWithNoTrips == 1) {
            
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
        }
        
        aux = aux->next;
        lineCounter++;
        if ((limit != 0) && (lineCounter >= limit)) {
            return;
        }
    }
}
