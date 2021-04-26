void WriteLED(int LED_Colour, int LED_Status)
{
	int gpioPin;
	int ledNowStatus;

	switch (LED_Colour) {
		case GREEN:
			gpioPin = GPIO_PIN_12;
			break;
		case ORANGE:
			gpioPin = GPIO_PIN_13;
			break;
		case RED:
			gpioPin = GPIO_PIN_14;
			break;
		case BLUE:
			gpioPin = GPIO_PIN_15;
			break;
	}

	//Current Status
	if((GPIOD->ODR & gpioPin) == gpioPin){
		ledNowStatus = LED_ON;
	}
	else{
		ledNowStatus = LED_OFF;
	}

	if(ledNowStatus != LED_Status){
		GPIOD->ODR = GPIOD->ODR ^ gpioPin; //Toggle LED
	}
}
void StateMachine2()
{
    float joyStateStatus[5];
    debounce(joyStateStatus,5);
//	readJoysticks(joyStateStatus,5);
	static int currentState = ALL_LED_OFF;

	if(currentState == ALL_LED_OFF){
		if(joyStateStatus[JOY_LEFT] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn2();
		}
		else if (joyStateStatus[JOY_RIGHT] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn2();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn2();
		}
	}
	else if(currentState == LEFT_LED_ON){
		if(joyStateStatus[JOY_CENTRE] == 0){
			currentState = ALL_LED_OFF;
			setAllLedOff2();
		}
		else if (joyStateStatus[JOY_RIGHT] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn2();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn2();
		}
	}
	else if(currentState == RIGHT_LED_ON){
		if(joyStateStatus[JOY_CENTRE] == 0){
			currentState = ALL_LED_OFF;
			setAllLedOff2();
		}
		else if (joyStateStatus[JOY_LEFT] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn2();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn2();
		}
	}


}
void 	setAllLedOff2()
{
	WriteLED(GREEN, LED_OFF); //left
	WriteLED(RED, LED_OFF); //right
}
void 	setLeftLedOn2()
{
	WriteLED(GREEN, LED_ON); //left
	WriteLED(RED, LED_OFF); //right
}
void	setRightLedOn2()
{
	WriteLED(GREEN, LED_OFF); //left
	WriteLED(RED, LED_ON); //right
}
void debounce2(float y[],int size)
{
	int joyStateStatus[size];
	float a = 0.5;
	float b = 1 - a;

	//zero the incoming array
	for(int i = 0; i < size; i++){
		y[i] = 0;
	}

	//pseudocode mentioned in assignment
	for(int i = 0; i < 10; i++)
	{
		//read the current state of the joySticks
		readJoysticks2(joyStateStatus,5);

		//Set joyStatePreviousStatus equal to what was last read
		for(int i = 0; i <= 4; i++){
			y[i] = a*y[i] + b*joyStateStatus[i];
		}
		HAL_Delay(1);
	}
}