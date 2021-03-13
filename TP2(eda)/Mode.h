#ifndef MODE_H
#define MODE_H	1

/*************PROTOTYPE****************/

/*
*function that executes mode one,
*which consists of showing in real time how the floor is being cleaned;
*graphing the clean and dirty tiles, to the robots and their directions.
* 
*Argument:
*floor :floor to be completed
* robots _number: number of robots
* width and height :floor dimensions
* 
* Returns
*Success: SUCCESS definition
*Failure: FAILURE definition
*/

int runModeOne(struct Floor*,int robots_number, int width, int height);


#endif