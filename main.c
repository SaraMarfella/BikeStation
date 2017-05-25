/*  GENERAL INFORMATION
 
 Project created by Sara Marfella IST188316 on May 16, 2017
 
 Implemented:
 1. Criação das estruturas de dados necessárias para representar as estações e as viagens.
 2. Leitura parâmetros do main e dos ficheiros necessários ao funcionamento do programa.
 3. Construção dos menus e introdução dos dados.
 4. Construção das listas de viagens e de estações, inserindo os elementos de uma forma ordenada.
 5. Implementação do modo de listagem de viagens.
 6. Implementação da seleção de dados, removendo elementos da lista de viagens.
 7. Implementação do modo de listagem das estações.
 8. Implementação do modo de listagem de rotas.
 
 Not Implemented:
 9. Implementação do modo gráfico.
 10. Implementação da análise estatística de dados.
 11. Eventuais melhorias ao modo gráfico da aplicação, recorrendo a um mapa real.
 
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
void clearInputBuffer();

/* Main Function
 * \param mode and file (file travelers and file stations)
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

/* mainMenu: prints the main menu and handles the main menu options
 * \param tripList      the header of the trip list (can be the filtered list)
 * \param stationsList  the header of the stations list
 */
void mainMenu(Trip * tripsList, Station * stationsList){
    int command;
    printf("\n * M A I N   M E N U * \n\n");
    printf("  [ 1 ]   Select the data\n\n");
    printf("  [ 2 ]   Print List of trips\n\n");
    printf("  [ 3 ]   Print List of stations\n\n");
    printf("  [ 4 ]   Print List of routes\n\n");
    printf("  [ 5 ]   Print List of statistics\n\n");
    printf("  [ 6 ]   Quit\n\n");
    scanf("%d", &command);
    clearInputBuffer();
    switch (command) {
        case 1:
            selectDataMenu(tripsList, stationsList);
            break;
        case 2:
            command = -1;
            while ((command < 0) || (command > 32000)) {
             printf("How many trips do you want to print? (0..32000, enter 0 for all)\n");
             scanf("%d", &command);
             clearInputBuffer();
            }
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
        case 6:
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Error: invalid command...\n");
            mainMenu(tripsList, stationsList);
            break;
    }
}

/* List of Stations Menu: creates and prints the list of stations with max/min/avg
 * \param tripList      the header of the trip list (usually the filtered list)
 * \param stationsList  the header of the stations list
 */
void listOfStationsMenu(Trip * tripsList, Station * stationsList) {
    Station * filteredStations = countBikes(tripsList, stationsList, filter_hour_start, filter_hour_end);
    
    // option to ask the user: limit (0 for all), should print stations with no trips (YES, NO)
    printStationsList(filteredStations, 0, YES);
    mainMenu(tripsList, stationsList);
}

/* SelectDataMenu: prints and manages the menu to set search criteria
 * \param filteredTrips     the header of the trip list (can be the filtered list)
 * \param allStations       the header of all stations list
 */
void selectDataMenu(Trip * filteredTrips, Station * allStations){
    
    int duration = -1, command;
    
    printf("\n * Select the mode of your search * \n\n");
    printf("  [ 1 ]   Period of time (hour start, hour end)\n\n");
    printf("  [ 2 ]   Day of week\n\n");
    printf("  [ 3 ]   Max duration of trip (in seconds)\n\n");
    printf("  [ 4 ]   New Search (reset list)\n\n");
    printf("  [ 5 ]   Return to Main Menu\n\n");
    scanf("%d", &command);
    clearInputBuffer();
    switch (command) {
        case 1:
            command = -1;
            
            while ((command < 0) || (command > 23)) {
                printf("Insert start time of trip (hour between 0..23):\n");
                scanf("%d", &command);
                clearInputBuffer();
            }
            // temporary place to store start time cannot overwrite the global now
            // because we use it to understand if user has used the filter before, later in the code
            int start = command;
            
            command = -1;
            while ((command < 0) || (command > 23)) {
                printf("Insert end time of trip (hour between 0..23):\n");
                scanf("%d", &command);
                clearInputBuffer();
            }
            filter_hour_end = command;
            
            // if user had filtered the list before, reset the list to avoid problems
            if (filter_hour_start != -1) {
                // now we can save the start time
                filter_hour_start = start;
                Trip * allTrips = readTripsData(trips_file);
                printf("Note: a time filter was already applied.\nThe trip list was reset to use the new parameters.\n");
                filteredTrips = selectTripsByTime(allTrips, filter_hour_start, filter_hour_end);
            } else {
                filter_hour_start = start;
                filteredTrips = selectTripsByTime(filteredTrips, filter_hour_start, filter_hour_end);
            }
            mainMenu(filteredTrips, allStations);
            break;
        case 2:
            command = 0;
            while ((command < 1) || (command > 7)) {
                printf("\nPlease insert the day of trip:\n\n");
                printf("  [ 1 ]   Monday\n");
                printf("  [ 2 ]   Tuesday\n");
                printf("  [ 3 ]   Wednesday\n");
                printf("  [ 4 ]   Thursday\n");
                printf("  [ 5 ]   Friday\n");
                printf("  [ 6 ]   Saturday\n");
                printf("  [ 7 ]   Sunday\n");
                scanf("%d", &command);
                clearInputBuffer();
            }
            filteredTrips = selectTripsByDay(filteredTrips, command);
            mainMenu(filteredTrips, allStations);
            break;
        case 3:
            while ((duration < 0) || (duration > 32767)) {
                printf("Insert the max duration of trip (in seconds):\n");
                scanf("%d", &duration);
                clearInputBuffer();
            }
            filteredTrips = selectTripsByDuration(filteredTrips, duration);
            mainMenu(filteredTrips, allStations);
            break;
        case 4:
            filter_hour_end = -1;
            filter_hour_start = -1;
            Trip * allTrips = readTripsData(trips_file);
            mainMenu(allTrips, allStations);
            break;
        case 5:
            if (filteredTrips != NULL) mainMenu(filteredTrips, allStations);
            else {
                Trip * allTrips = readTripsData(trips_file);
                mainMenu(allTrips, allStations);
            }
            break;
        default:
            printf("Error: invalid command...\n");
            if (filteredTrips != NULL) selectDataMenu(filteredTrips, allStations);
            else {
                Trip * allTrips = readTripsData(trips_file);
                selectDataMenu(allTrips, allStations);
            }
            break;
    }
}

/* selectStationMenu:
 * \param allStations      the header of all stations list
 * \param filteredTrips    the header of the trips list (can be filtered)
 * \param id               the ID of the station
 */
void selectStationMenu(Station * allStations, Trip * filteredTrips, int id) {
    int command = -1;
    char stationName[7] = "";
    
    // validate id of station
    while (strcmp(stationName, "") == 0) {
        printf("Insert the id of the station:\n");
        scanf("%d", &id);
        clearInputBuffer();
        strcpy(stationName, getStationNameById(id, allStations));
    }
    filteredTrips = selectTripsByIdStation(filteredTrips, id);
    
    command = -1;
    while ((command <0) || (command > 32767)) {
        printf("How many routes do yo want to print? (0 for all)\n");
        scanf("%d", &command);
        clearInputBuffer();
    }
    printRoutesList(filteredTrips, allStations, id, command);
    Trip * allTrips = readTripsData(trips_file);
    mainMenu(allTrips, allStations);
}

/* statsMenu: placeholder for the stats section 
 */
void statsMenu() {
    printf("Sorry, the statistics are not yet implemented\n");
}

/* clearInputBuffer: handles input of multiple characters
 * resets the input buffer to avoid double-commands
 * Source: https://stackoverflow.com/questions/3969871/using-getchar-on-c-gets-the-enter-after-input
 */
void clearInputBuffer() // works only if the input buffer is not empty
{
    char c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
    return;
}
