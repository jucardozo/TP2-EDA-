#ifndef _MODE_H
#define _MODE_H	1

#include "Backend.h" /* Floor and statusCallback */

/*************PROTOTYPE****************/

/*
*function that executes mode one,
*which consists of showing in real time how the floor is being cleaned;
*graphing the clean and dirty tiles, to the robots and their directions.
* 
*Argument:
* robots_number: number of robots.
* width and height :floor dimensions.
* publishStatus: Frontend callback.
* front_data: Pointer to pass to the frontend callback.
* 
* Returns
*Success: SUCCESS definition
*Failure: FAILURE definition
*/

int runModeOne(int robots_number, int width, int height, statusCallback publishStatus, void * front_data);

/*
 * Simulate the time needed to clean a floor of size width*height.
 * 
 * Arguments:
 *  width: Floor's width.
 *  height: Floor's height.
 *  publishStatus: Frontend callback.
 *  front_data: Pointer to pass to the frontend callback.
 */
int runModeTwo(int width, int height, statusCallback publishStatus, void * front_data);

#endif /* _MODE_H */