/*Colton Functions Start*/
void readJoysticks(int joyStickState[], int size)
{

	GPIO_PinState joy_a_status = HAL_GPIO_ReadPin(JOY_A_GPIO_Port, JOY_A_Pin);
	GPIO_PinState joy_b_status = HAL_GPIO_ReadPin(JOY_B_GPIO_Port, JOY_B_Pin);
	GPIO_PinState joy_c_status = HAL_GPIO_ReadPin(JOY_C_GPIO_Port, JOY_C_Pin);
	GPIO_PinState joy_d_status = HAL_GPIO_ReadPin(JOY_D_GPIO_Port, JOY_D_Pin);
	GPIO_PinState joy_ctr_status = HAL_GPIO_ReadPin(JOY_CTR_GPIO_Port, JOY_CTR_Pin);

	joyStickState[0] = joy_a_status;
	joyStickState[1] = joy_b_status;
	joyStickState[2] = joy_c_status;
	joyStickState[3] = joy_d_status;
	joyStickState[4] = joy_ctr_status;
}

void debounce(float y[],int size)
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
		readJoysticks(joyStateStatus,5);

		//Set joyStatePreviousStatus equal to what was last read
		for(int i = 0; i <= 4; i++){
			y[i] = a*y[i] + b*joyStateStatus[i];
		}
		HAL_Delay(1);
	}
}

void StateMachine()
{
    float joyStateStatus[5];
    debounce(joyStateStatus,5);
//	readJoysticks(joyStateStatus,5);
	static int currentState = ALL_LED_OFF;

	if(currentState == ALL_LED_OFF){
		if(joyStateStatus[JOY_LEFT] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn();
		}
		else if (joyStateStatus[JOY_RIGHT] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn();
		}
	}
	else if(currentState == LEFT_LED_ON){
		if(joyStateStatus[JOY_CENTRE] == 0){
			currentState = ALL_LED_OFF;
			setAllLedOff();
		}
		else if (joyStateStatus[JOY_RIGHT] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn();
		}
	}
	else if(currentState == RIGHT_LED_ON){
		if(joyStateStatus[JOY_CENTRE] == 0){
			currentState = ALL_LED_OFF;
			setAllLedOff();
		}
		else if (joyStateStatus[JOY_LEFT] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn();
		}
	}


}
void 	setAllLedOff()
{
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 0); //left
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, 0); //right
}
void 	setLeftLedOn()
{
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 1); //left
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, 0); //right
}
void	setRightLedOn()
{
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 0); //left
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, 1); //right
}
/*Colton Functions Stop*/