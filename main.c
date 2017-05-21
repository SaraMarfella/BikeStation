/* GENERAL INFORMATION
 / Project created by Sara Marfella
 / IST188316
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

//ALL THE STRUCT

// define a type of date
typedef struct{
    int month;
    int day;
    int year;
    int hour;
    int minute;
}Date;

// Linked List of Travelers
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

//define a type for short name of station ( one characters and 5 numbers)
typedef struct{
    char first_part;
    int second_part;
}ShortName;

// Linked List of Stations
typedef struct Station{
    int id;
    ShortName name;
    char full_name[MAX_SIZE];
    char municipal[MAX_SIZE];
    long int latitude;
    long int longitude;
    bool status; // existing or removed
    struct Station *next;
}Station;

// DECLARATION OF FUNCTIONS
void readTripsData();
void mainMenu();
void selectData();
void printTripsList(Trip*, int);
Trip* selectTripsByTime(Trip*, int, int);

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
    mainMenu(tripsHead);

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


/* Command Line Interface */
void mainMenu(Trip * sourceList){
    int command;
    printf("Press 1: Select the data\n");
    printf("Press 2: Print List of trips\n");
    printf("Press 3: Print List of stations\n");
    printf("Press 4: Print List of routes\n");
    printf("Press 5: Print List of statistics\n");
    printf("Any other key: Quit\n");
    scanf("%d", &command);
    switch (command) {
        case 1: selectData(sourceList); break;
        case 2:
            printf("How many trips do you want to print? (0..32000, enter 0 for all)\n");
            scanf("%d", &command);
            printTripsList(sourceList, command);
            mainMenu(sourceList);
            break;
        case 3: break;
        case 4: break;
        case 5: break;
        default: return;
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


void selectData(Trip * filteredTrips){

    int hour_start, hour_end, day, duration, command;

    printf("Select the mode of your search\n");
    printf("Press 1: Period of time (hour start, hour end)\n");
    printf("Press 2: Day of week\n");
    printf("Press 3: Max duration of trip (in seconds)\n");
    printf("Press 4: Return to Main Menu\n");
    printf("Any other key: Quit\n");
    scanf("%d", &command);
    switch (command) {
        case 1:
            printf("Insert start time of trip (hour between 0..24):\n");
            scanf("%d", &hour_start); // devo cercare il dato all'interno del file
            printf("Insert end time of trip (hour between 0..24):\n");
            scanf("%d", &hour_end); // e poi stampare i viaggi se ce ne sono
            filteredTrips = selectTripsByTime(tripsHead, hour_start, hour_end);
            printf("Done.\n");
            mainMenu(filteredTrips);
            break;
        case 2: break;
        case 3: break;
        case 4:
            if (filteredTrips != NULL) mainMenu(filteredTrips);
            else mainMenu(tripsHead);
            break;
        default: return;
    }
}
