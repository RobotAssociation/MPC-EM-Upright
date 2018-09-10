#include "DirectionControl.h"


float DirectionControlOutNew ,DirectionControlOutOld ;//= 0= 0
unsigned char DirectionControlPeriod;
float DirectionControlOut = 0.0;

void DirectionControl()
{
		
}



void DirectionControlOutput()
{
	float fValue;
	fValue = DirectionControlOutNew - DirectionControlOutOld;
	DirectionControlOut = fValue * DirectionControlPeriod /20 + DirectionControlOutOld;	
	 
}


