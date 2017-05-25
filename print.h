
/* Project created by Sara Marfella IST188316 on May 16, 2017 */

#ifndef print_h
#define print_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "dataManager.h"

// Constants
#define YES 1   // used by printStationsList
#define NO 2    // used by printStationsList

// print functions
void printTripsList(Trip*, int);
void printStationsList(Station*, int, int);
void printRoutesList(Trip*, Station*, int, int);

#endif /* print_h */
