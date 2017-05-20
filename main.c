/* GENERAL INFORMATION
/ Project created by Sara Marfella
/ IST188316
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// CONSTANT
#define MAX_SIZE 100 // max size of a string

// DECLARATION OF FUNCTIONS

//ALL THE STRUCT

// define a type of date
typedef struct{
    int hours;
    int minutes;
}Date;

// define a type of bike id
typedef struct{
    char first_part;
    int second_part;
}IdBike;

// structure of Travelers
struct Traveler{
   long int id_traveler;
    int duration_trip;
    Date start_trip;
    int id_start_station;
    Date end_trip;
    int id_final_station;
    IdBike the_bike;
    char user[MAX_SIZE]; // the user can be casual or members
    int year_birthday; // only in case of members
    char gender[MAX_SIZE]; // female or man only in case of members
};

//define a type for short name of station ( one characters and 5 numbers)
typedef struct{
    char fist_part;
    int second_part[5];
}ShortName;

// structure of station
struct Station{
    int id_station;
    ShortName name_station;
    char full_name_station[MAX_SIZE];
    char municipal[MAX_SIZE];
    long int latitude;
    long int longitude;
    char state[MAX_SIZE]; // existing or removed
};



struct Traveler *oneTraveler;
struct Station *oneStation;


/*function main
@param mode(-g or -t) and file (file travelers and file stations)
*/
int main (int argc, char *argv[]){

SDL_Event event;

    // check for correct input
    /*if (argc != 3) {
        printf("Error - required arguments missing");
    }
     if ((!argv[1] == '-t')||(!argv[1] == '-g')) {
        printf("Error - only text mode or graphic mode is available, please start with -t or -g");
    //call of function that read the files
    readFiles();
     }*/

    Menu();

    while(event.type == SDL_KEYDOWN){
        switch ( event.key.keysym.sym ){

            case SDLK_1:
                Menu_1();
                break;

            case SDLK_2:
                break;

            case SDLK_3:
                break;

            case SDLK_4:
                break;

            case SDLK_5:
                break;
        }
    }

    return 0;
}

// function that open and read the file
void readFiles(char *argv[]){
    char line[1024];
    char *token;
    const char space = " ";
    int i=0;

    //Allocation of memory
    struct Traveler *oneTraveler= (struct Traveler*)malloc(sizeof(struct Traveler));
    struct Station *oneStation= (struct Station*)malloc(sizeof(struct Station));

    //first file
    FILE *fileOne = fopen( argv[2], "r" );
    if ( fileOne == 0 ){
        printf( "Error - Could not open file\n" );
    }
    else {
        while(fgets(line, sizeof line, fileOne) != NULL){
            token = strtok(line, space);
            if(i == 0){
               printf("%s\t",token);
                token = strtok(NULL,space);
            } else {
                printf("%d\n",atoi(token));
            }
        }
        fclose(fileOne);
    }

    //second file
    FILE *fileTwo = fopen( argv[3], "r" );
     if ( fileTwo == 0 ){
        printf( "Error - Could not open file\n" );
    }
    else {
        while(fgets(line, sizeof line, fileTwo) != NULL){
            token = strtok(line, space);
            if(i == 0){
               printf("%s\t",token);
                token = strtok(NULL,space);
            } else {
                printf("%d\n",atoi(token));
            }
        }
        fclose(fileTwo);
    }

}

void Menu(){
    printf("\nPress 1: Select the data");
    printf("\nPress 2: List of travelers");
    printf("\nPress 3: List of stations");
    printf("\nPress 4: List of routes");
    printf("\nPress 5: List of statistics");
}

void Menu_1(fileOne){

SDL_Event event;

    printf("Select the mode of your search");
    printf("\nPress 1: Period of time");
    printf("\nPress 2: Day of week");
    printf("\nPress 3: Duration");

    while(event.type == SDL_KEYDOWN){
        switch ( event.key.keysym.sym ){

            case SDLK_1:
                printf("insert hour's start of trip:");
                scanf("%d", hour_start); // devo cercare il dato all'interno del file
                printf("insert hour's end of trip:");
                scanf("%d", hour_end); // e poi stampare i viaggi se ce ne sono
                break;

            case SDLK_2:
                printf("insert the day of the week:");
                scanf("%d", day); // questo però non c'è nel file
                break;

            case SDLK_3:
                printf("insert the max duration of the trip");
                scanf("%d", duration); //duration_trip in the file
                break;



}
