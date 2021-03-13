#include <stdlib.h>
#include <stdio.h>
#include <math.h> /* M_PI, sin(), cos(), isgreaterequal, isless */
#include <time.h>

#include "Robots.h" /* SUCCESS, FAILURE, Floor, RobotCollection, Robot, Position */

/******************** DEFINITIONS ********************/
// Module of the distance a robot can move in a single turn
#define ROBOT_DISPLACEMENT_MODULE       (1.0)

/* In case M_PI is not defined (copied from definition in glibc's math.h file)
 * It seems like some non-POSIX implementations could not have it defined.
 * See:
 * https://stackoverflow.com/questions/9912151/math-constant-pi-value-in-c#comment12648210_9912169
 */
#ifndef M_PI
#define M_PI            3.14159265358979323846 /* pi */
#endif

/******************** MACROS ********************/
 // Convert degree in radians
#define DEG2RAD(d)      ((d)*(M_PI/180))

/******************** PROTOTYPES ********************/
static double getRandomAngle(void);
static struct Position* newCoordinates(struct Position* current, double angle, struct Position* new, double module);
static long int ipow(unsigned int base, unsigned int exponent);

/******************** PUBLIC FUNCTIONS ********************/
void* generateRobots(struct Floor* floor_p, int ammount) {
    void* pointer;
    pointer = malloc(ammount * sizeof(struct Robot);
    if (pointer != NULL) {
        srand(time(0));
        floor_p->robots.robots_number = ammount; // SACAR
        floor_p->robots.robots->coordinates.x = (0 + rand() % ((floor_p->width) + 1));			//a random number is assigned between zero and the maximum width value
        floor_p->robots.robots->coordinates.y = (0 + rand() % ((floor_p->height) + 1));			//a random number is assigned between zero and the maximum height value
        floor_p->robots.robots->angle = getRandomAngle();
        return pointer;
    }
    else{
        floor_p->robots.robots_number = 0; // SACAR
        floor_p->robots.robots->coordinates.x = 0;			//a random number is assigned between zero and the maximum width value
        floor_p->robots.robots->coordinates.y = 0;			//a random number is assigned between zero and the maximum height value
        floor_p->robots.robots->angle = 0;
        return NULL;
    }
}

int cleanFloor(struct Floor* floor_p) {
    if (floor_p == NULL)
        return FAILURE;

    int i = 0;
    while (i < floor_p->robots.robots_number) {
        int x = floor(floor_p->robots.robots[i].coordinates.x);
        int y = floor(floor_p->robots.robots[i].coordinates.y);

        floor_p->clean[x][y] = TILE_CLEAN;
        i++;
    }
    return SUCCESS;
}

int isFloorClean(struct Floor* floor) {
    if (floor == NULL)
        return FAILURE;

    int tile_status = TILE_CLEAN;
    int i = 0;
    while (i < floor->clean_size && tile_status != TILE_DIRTY) {
        tile_status = floor->clean[i];
        i++;
    }
    return tile_status;
}

void * moveRobots(struct Floor* floor) {
    if (floor == NULL)
        return NULL;

    struct RobotCollection* collection = &(floor->robots);
    struct Robot* current_robot = NULL;
    struct Position new_position = { 0.0 };

    int i = 0;
    while (i < collection->robots_number) {
        current_robot = &collection->robots[i];

        // Compare with borders using angle
        // Get new angle when collision is inminent
        if (newCoordinates(&current_robot->coordinates,
            current_robot->angle,
            &new_position,
            ROBOT_DISPLACEMENT_MODULE) == NULL) {
            return NULL;
        }

        // Does the robot move out of the board?
        if (isless(new_position.x, 0.0)
            || isless(new_position.y, 0.0)
            || isgreaterequal(new_position.x, (double)floor->width)
            || isgreaterequal(new_position.y, (double)floor->height)) {
            // YES, it moves out

            // Give it a new angle!
            current_robot->angle = getRandomAngle();
            // And do NOT increment variable i, so its new possible position 
            // gets calculated again
        }
        else {
            // NO, it doesn't move out

            // Store the new position and go on with another robot
            current_robot->coordinates.x = new_position.x;
            current_robot->coordinates.y = new_position.y;

            i++;
        }
    }
    return floor;
}

/******************** STATIC FUNCTIONS ********************/

/*
 * Generate a random angle in range 0.00 <= angle < 360.00.
 * 
 * Decimal precision is defined in MINIMUM_PRECISION (see Backend.h)
 * and the returned angle is in degrees.
 *
 * Assumes that srand() has been already initialized with a seed.
 * 
 * Arguments:
 *  None
 * 
 * Returns:
 *  Number in range [0.0;360.0) with at most MINIMUM_PRECISION decimals.
 */
static double getRandomAngle(void) {
    // Generate two random numbers:
    // .- Integer part ( 0 <= integer < 360)
    // .- Decimal part ( 0 <= decimal < 100)
    // This will let us create the double with MINIMUM_PRECISION decimal digits.

    int integer = rand() % 360;
    // Uses custom pow function which only works with integers.
    // (math lib's pow() uses doubles... seems overkill)
    // Just in case, we take the absolute value of MINIMUM_PRECISION.
    int decimal = rand() % ipow(10, abs(MINIMUM_PRECISION)); 

    // Integer part + decimal part 
    return (double)(integer + ((double)decimal / ipow(10, abs(MINIMUM_PRECISION))));
}

/*
 * Calculate where a robot should move based on its _current_ position and _angle_.
 * 
 * Assumes coordinate system with (0,0) in the top left position and
 *  (width, height) in the bottom right position.
 *  (x is (+) to the west; y is (+) to the south)
 * Assumes angle in degrees.
 *
 * All comparisons are made using comparison macros from math lib
 *  (isgreater, isless, etc).
 *
 * Arguments:
 *  current: Current robot's position.
 *  angle: Robot's movement angle. Must be in degrees and in range [0;360.0)
 *  new: Where the robot will land after moving.
 *  module: How much the robot should move (vector's module).
 * 
 * Returns:
 *  On success: the same pointer given in new argument.
 *  On failure: returns NULL
 */
static struct Position*
    newCoordinates(struct Position* current, double angle,
        struct Position* new, double module) {

    if (current == NULL)
        return NULL;

    if (new == NULL)
        return NULL;

    /* Angles greater or equal than 360.00 are considered invalid */
    if (isgreaterequal(angle, 360.0)) {
        printf("Invalid angle recieved.\n");
        printf("I got %.2f which is >= 360.00\n", angle);
        return NULL;
    }
    else if (isless(angle, 0.0)) {
        printf("Invalid angle recieved.\n");
        printf("I got %.2f which is < 0.00\n", angle);
        return NULL;
    }

    new->x = current->x;
    new->y = current->y;

    // 0 <= angle < 90
    if (isless(angle, 90.0)) {
        new->x += module * sin(DEG2RAD(angle));
        new->y -= module * cos(DEG2RAD(angle));
    }
    // 90 <= angle < 180
    else if (isless(angle, 180.0)) {
        new->x += module * sin(DEG2RAD(angle - 90));
        new->y += module * cos(DEG2RAD(angle - 90));
    }
    // 180 <= angle < 270
    else if (isless(angle, 270.0)) {
        new->x -= module * sin(DEG2RAD(angle - 180));
        new->y += module * cos(DEG2RAD(angle - 180));
    }
    // 270 <= angle < 360
    else if (isless(angle, 360.0)) {
        new->x -= module * sin(DEG2RAD(angle - 270));
        new->y -= module * cos(DEG2RAD(angle - 270));
    }

    return new;
}

/*
 * Raise base to exponent using only integers.
 * 
 * As math library does not provide a pow() function to work with pure 
 * integers, I've made a simple one based on _Exponentiation by squaring_ 
 * method.
 * See more at: https://en.wikipedia.org/wiki/Exponentiation_by_squaring
 * 
 * Arguments:
 *  base: Power base.
 *  exponent: Power exponent.
 * 
 * Returns:
 *  Success: base^exponent
 *  Failure: -1 (i.e. when trying to perform 0^0)
 */
static long int ipow(unsigned int base, unsigned int exponent) {
    if (base == 0 && exponent == 0) {
        return -1;
    }

    int result = 1;
    while (1) {
        if (exponent & 1) {
            result *= base;
        }
        exponent >>= 1;
        if (!exponent) {
            break;
        }
        base *= base;
    }

    return result;
}