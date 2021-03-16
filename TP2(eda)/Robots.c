#include <stdlib.h>
#include <stdio.h>
#include <math.h> /* M_PI, sin(), cos(), isgreaterequal, isless */

#include "Robots.h" /* SUCCESS, FAILURE, Floor, RobotCollection, Robot, Position */

/******************** DEFINITIONS ********************/
// Module of the distance a robot can move in a single turn
#define ROBOT_DISPLACEMENT_MODULE       (1.0)

// Minimum decimal precision taken into account for floats and double
#define MINIMUM_PRECISION	    (3)

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
static coords_t* newCoordinates(coords_t* current, double angle, coords_t* new, double module);
static long int ipow(unsigned int base, unsigned int exponent);
static void robotsArr(struct RobotCollection* b1, coords_t max);

/******************** PUBLIC FUNCTIONS ********************/
void*
generateRobots(struct RobotCollection* b1, int ammount, coords_t maximum) {
    if (maximum.x == 0 || maximum.y == 0)
        return NULL;

    // B1 Droids... those weren't very smart, were them?
    b1->robots = malloc(ammount * sizeof(struct Robot));
    if (b1->robots != NULL) {
        b1->robots_number = ammount;            

        robotsArr(b1, maximum);  //create a defined number of robots, giving them random position and direction

        return b1->robots;
    }
    else{
        return NULL;
    }
}

void
destroyRobots(struct RobotCollection* b1) {
    if(b1->robots != NULL) free(b1->robots);
    b1->robots_number = 0;
}

void*
moveRobots(struct RobotCollection* b1, coords_t maximum_coordinates){
    if (b1 == NULL)
        return NULL;

    if (maximum_coordinates.x == 0 || maximum_coordinates.y == 0)
        return NULL;

    struct Robot* current_robot = NULL;
    coords_t new_position = {0};

    for (int i = 0; i < b1->robots_number; i++) {
        current_robot = &b1->robots[i];

        // Compare with borders using angle
        // Get new angle when collision is inminent
        if (newCoordinates(&(current_robot->coordinates), current_robot->angle,
                           &new_position, ROBOT_DISPLACEMENT_MODULE) == NULL) {
            return NULL;
        }

        // Does the robot move out of the board?
        if (isless(new_position.x, 0.0)
            || isless(new_position.y, 0.0)
            || isgreaterequal(new_position.x, maximum_coordinates.x)
            || isgreaterequal(new_position.y, maximum_coordinates.y)) {
            // YES, it moves out

            // Give it a new angle!
            current_robot->angle = getRandomAngle();
        }
        else {
            // NO, it doesn't move out

            // Store the new position and go on
            current_robot->coordinates.x = new_position.x;
            current_robot->coordinates.y = new_position.y;

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
static double
getRandomAngle(void) {
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
    return (double)(integer + (((double)decimal) / ipow(10, abs(MINIMUM_PRECISION))));
}

/*
 * Calculate where a robot should move based on its _current_ position and _angle_.
 * 
 * Assumes coordinate system with (0,0) in the top left position and
 *  (width, height) in the bottom right position.
 *  (x is (+) to the est; y is (+) to the south)
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
static coords_t *
newCoordinates(coords_t * current, double angle, coords_t * new, double module) {

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

    /*
     * Taking into account that 0° is pointing to the North, that x increases 
     * to the east and y to the south, a graphic to understand which signs to apply
     * on the arithmetics could be this one:
     * 
     * Let 'a' be an angle between the direction vector and the North (0°).
     * Let 'H' be the module of the direction vector.
     * Let 'R' be a robor in some coordinatas (x,y) of the board.
     * 
     *  -----------------------------------------> x
     *  |                         /\
     *  |                    ^    /
     *  |                    |_a_/    X increases:   H*sin(a) (X is positive to the east)
     *  |                    |  /     Y increases: - H*cos(a) (Y is negative to the north)
     *  |             <----- R ----->
     *  |                    |
     *  |                    |
     *  |                   \ /
     * \ /
     *  y
     * 
     * The same idea can be extrapolated to the other quadrants.
     * 
     */
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
 * method (also known as _binary exponentiation_).
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
static long int
ipow(unsigned int base, unsigned int exponent) {
    if (base == 0 && exponent == 0) {
        return -1;
    }

    if (base == 1) {
        return 1;
    }

    if (base == 0) {
        return 0;
    }

    int result = 1;
    while (1) {
        // Is exponent odd?
        if (exponent & 1) {
            result *= base;
        }

        // Shift exponent to the left (same as divide by 2)
        exponent >>= 1;
        if (!exponent) {
            break;
        }

        base *= base;
    }

    return result;
}

/*
 * Generates an array of struct robots with a quantity passed by the user.
 * 
 * Each robot has a random position and direction (angle). 
 * 
 * In the case of the angle, it is passed by getRandomAngle
 * 
 * Arguments:
 *  b1: Robots to initialize in a random position.
 *  max: Maximum coordinates any robot can take.
 *  
 * Returns: 
 *  Nothing
 * 
*/
static void
robotsArr(struct RobotCollection* b1, coords_t max) {
    for (int limit = 0; limit < b1->robots_number; limit++) {
        b1->robots[limit].coordinates.x = (0 + rand() % ((int) max.x));            //a random number is assigned between zero and the maximum width value
        b1->robots[limit].coordinates.y = (0 + rand() % ((int) max.y));           //a random number is assigned between zero and the maximum height value
        b1->robots[limit].angle = getRandomAngle();
    }
}
