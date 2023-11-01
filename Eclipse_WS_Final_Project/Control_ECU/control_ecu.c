/*
 ================================================================================================================================
 * File Name   : Control_ECU.c
 *
 * Module Name : Control_ECU
 * 
 * Description : Source File to The Control ECU
 *
 * Created On  : Oct 28, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */
#include "control_ech.h"
#include "gpio_interface.h"
#include "twi_interface.h"
#include "uart_interface.h"
#include "pwm_interface.h"
#include "pir.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "Internal_mapping_register_private.h"
#include "timer_interface.h"
/*******************************************************************************
 *                      Global Variables									   *
 *******************************************************************************/
static uint8 g_MC2_application_sequence=0; 					/*Controls HMI_ECU MC application Flow*/

static uint8 g_passwordArray1[5],g_passwordArray2[5]; 		/*array to Store password entered by the user*/

static volatile uint8 g_tick=0; 								/* For counting the number of Compare match for timer1*/

/*******************************************************************************
 *                      Main Function                         				   *
 *******************************************************************************/
int main(void)
{
	/*Note the Prescale of Pwm in this app it doesnt important choose what ever u want bec.
	  i will use Pwm in 100% duty and 0% duty so i have no restriction on the output of pwm
	  i have no certain requirment for the ouput signal frequency so in this app u can choose
	  a pwm prescale what ever u want :) */
	PWM_config Pwm_configuration={PWM0,NON_INVERTING,PWM_F_CPU_1024};

	/*Just intial values where in this project i will use timer1 in CTC mode with 1024 prescale and Inital value =0 but i
	  I Will change the compare value from time to time*/
	Timer_ConfigType Timer_configuration={0,0,Timer1,TIMER_F_CPU_1024,Compare_Mode};

	/*UART Configurations and Initialization*/
	UART_ConfigType Uart_configuration={CHARACTER_SIZE_8,EVEN_PARITY,STOP_1BIT,9600};

	/*Intialize TWI*/
	I2c_ConfigType	I2c_configuration= {0x01,0x02,TWI_Prescaler_1};
	TWI_init(&I2c_configuration);

	/*Enable Global Interrupt*/
	SREG_REG.Bits.I_Bit=1;

	UART_init(&Uart_configuration);

	/*Set timer1 to call back */
	Timer_setCallBack(timer1_SetRequiredCallBackFunction, Timer1);

	/*Initialize password*/
	BUZZER_init();

	/*Initialize motor*/
	DcMOTOR_init();

	while(1)
	{
		switch(g_MC2_application_sequence)
		{
		case RECEIVING_PASSWORDS_STAGE:

			/*Receive the first password enter by user*/
			receive_Password(g_passwordArray1);

			/*Receive the re-enterd password enter by user*/
			receive_Password(g_passwordArray2);

			/* Increment Stage for next loop (go to CHECK_PASSWORDS_MATCHING_STAGE )*/
			g_MC2_application_sequence=CHECK_PASSWORDS_MATCHING_STAGE;
			break;

		case CHECK_PASSWORDS_MATCHING_STAGE:
		{
			/*NOTE : i made this block scope bec. i want the local variables defined in this case just to be local for its case*/

			/*Check if the two passwords sent via HMI ECU are the same or not*/
			uint8 twoPassowrdState=check_TwoPasswords(g_passwordArray1,g_passwordArray2);

			if(twoPassowrdState == MATCHED) /*Two passwords are matched*/
			{
				/*Send to HMI_ECU that the two password are matched*/
				send_Byte(MATCHED);

				/* Increment Stage for next loop (go to SAVE_PASSWORD_STAGE )*/
				g_MC2_application_sequence=SAVE_PASSWORD_INTO_EEPROM_STAGE;
			}
			else if(twoPassowrdState == NOT_MATCHED)	/*Two passwords are NOT matched*/
			{
				/*Send to HMI_ECU that the two password are matched*/
				send_Byte(NOT_MATCHED);

				/*Go to RECEIVING_PASSWORDS_STAGE*/
				g_MC2_application_sequence=RECEIVING_PASSWORDS_STAGE;
			}
			else
			{
				/*Do Nothing*/
			}
			break;
		}
		case SAVE_PASSWORD_INTO_EEPROM_STAGE:

			/*Write the password in EEPROM at index start from 0x0000 to 0x0000+(PASWWORD_SIZE-1)*/
			EEPROM_writeArray(0X0000, g_passwordArray1, PASSWORD_SIZE);

			/* Increment Stage for next loop (go to MAIN_MENU_OPTION_STAGE )*/
			g_MC2_application_sequence=MAIN_MENU_OPTION_STAGE;

			break;

		case MAIN_MENU_OPTION_STAGE:
		{
			/*NOTE : i made this block scope bec. i want the local varriables defined in this case just to be local for its case*/
			uint8 twoPasswordState;

			/*Receive the password enter from user after choise an + or - option in main option menu*/
			receive_Password(g_passwordArray2);

			/*read the old password stored in the EEPROM*/
			EEPROM_readArray(0X0000, g_passwordArray1, PASSWORD_SIZE);

			/*Compare the oldest password stored in EEPROM with the Password enter via user*/
			twoPasswordState=check_TwoPasswords(g_passwordArray1, g_passwordArray2);

			/*if two password are matched then make for user what he/she choose in main menu option*/
			if(twoPasswordState==MATCHED)
			{
				uint8 recieveByte;

				/*Send Matched to HMI ECU*/
				send_Byte(MATCHED);

				recieveByte=recieve_Byte();

				if(recieveByte=='+')
				{
					/*send to HMI ECU 3 which is indication for HMI ECU to go to OPEN_DOOR_STAGE in HMI ECU for displaying the required messages */
					send_Byte(3);

					/* Increment Stage for next loop (go to OPEN_DOOR_STAGE to unlocking/locking the door )*/
					g_MC2_application_sequence=OPEN_DOOR_STAGE;
				}
				else if (recieveByte=='-')
				{
					/*send to HMI ECU 0 which is indication for HMI ECU to go to CREATE_PASSWORD_STAGE in HMI ECU */
					send_Byte(0);

					/* go to first  Stage RECEIVING_PASSWORDS_STAGE in CONTROL ECU to make a new password again*/
					g_MC2_application_sequence=RECEIVING_PASSWORDS_STAGE;
				}
				else
				{
					/*Do Nothing*/
				}
			}
			else if (twoPasswordState==NOT_MATCHED)
			{
				/*Var for counting number of times user entered wrong password*/
				uint8 count;

				/*Send to HMI_ECU that the Entered password and the saved one are not matched*/
				send_Byte(NOT_MATCHED);

				/*
				 * in case the password entered by user is matched with the oldest one save in EEPROM
				 * and number of times that user re-entered the password equal 2 times or less, the system will continue normally
				 * and execute the function which is responsible for the choice he chose (Open Door/Change password).

				 * but, if the number of timers that user re-enterd the password are more than 2 times we will
				 * Activate the Buzzer and block the system for 1 minute and After the 1 min ends return to case 3
				*/

				for(count=0;count<2;count++)
				{
					/*Receive the entered password by user*/
					receive_Password(g_passwordArray2);

					/*Read the password already stored in External EEPROM*/
					EEPROM_readArray(0X0000, g_passwordArray1, PASSWORD_SIZE);

					if(check_TwoPasswords(g_passwordArray1, g_passwordArray2)==MATCHED)
					{
						/*Send the HMI ECU that two password are matched*/
						send_Byte(MATCHED);

						/*go to MAIN_MENU_OPTION_STAGE where the user choose again what he want  */
						g_MC2_application_sequence=MAIN_MENU_OPTION_STAGE;
						break;
					}
					else
					{
						send_Byte(NOT_MATCHED);
					}
				}
				/*this mean that user enterd wrong password more than 2 times so we will
				 * Activate the Buzzer and block the system for 1 minute and After the 1 min ends return to case 3
				*/
				if(count==2)
				{

					/*Timer1 Configurations and Initialization
					  where F_CPU= 8 MHZ and Timer Prescale = 1024
					  so we need 468750 count from timer to time 1 min
					  so we will put in compare register of timer1 value = 46875
					  we will get an 10 interrupts for time 1 min
					*/
					Timer_configuration.timer_compare_MatchValue=46875;

					Timer_init(&Timer_configuration);

					/*display error message in HMI ECU and turn the buzzer one in Control ECU and wait for 1 min as g_Tick will be equal 10 after 1 min*/
					BUZZER_on();
					while(g_tick < 10)
					{
						/*when g_tick=10 this mean that 1 minute passes so exit from the while loop*/
						/*Note g_tick is increment via Timer1 ISR so u should use volatile keyword
						 *on definition the global variable g_tick to turn off any compile optimization on it
						 *bec. the compiler see that the g_tick is a variable that will not be incremented in your
						 *SW code but actually the compiler does't know that g_tick will be incremented via an interrupt
						 *in timer1 Hardware so Kindly define g_tick using keyword volatile */
					}

					/*stop timer1 via using deInit() Function*/
					Timer_DeInit(Timer1);

					g_tick=0;

					/*Turn off the buzzer after the 1 min ends*/
					BUZZER_off();
				}
					/* go to MAIN_MENU_OPTION_STAGE */
					g_MC2_application_sequence=MAIN_MENU_OPTION_STAGE;
			}
			else
			{
				/*Do Nothing*/
			}
		}
			break;
		case OPEN_DOOR_STAGE:
		{
			/*Timer1 Configurations and Initialization
			 where F_CPU= 8 MHZ and Timer Prescale = 1024
			 so we need 117188 count from timer to time 15 sec
			 so we will put in compare register of timer1 value = 58594
			 we will need 2 interrupts for time 15 sec
			 NOTE Timer Timer_configuration its prescale and ID inital value
			*/
			Timer_configuration.timer_compare_MatchValue=58594;

			/*Intialize PWM of timer0*/
			PWM_init(&Pwm_configuration);

			/*Initialize Timer 1*/
			Timer_init(&Timer_configuration);

			/*Turn on the motor clockwise */
			DCMOTOR_rotateWithControl(ROTATE_CLOCKWISE, 100, FIRST_DC_MOTOR, PWM0);

			/*Wait for 15 sec Rotating motor clockwise*/
			while(g_tick<2){};

			/*Stop Timer1*/
			Timer_DeInit(Timer1);

			g_tick=0;

			/*stop the dc motor */
			DCMOTOR_rotateWithControl(STOP_ROTATION, 0, FIRST_DC_MOTOR, PWM0);

			/*set Compare value = 23438 ,prescale ,timer id,mode as i initialized
			 we will need 1 interrupt within 3 sec*/
			Timer_configuration.timer_compare_MatchValue=23438;
			Timer_init(&Timer_configuration);

			/*wait for 3 sec and the motor is stop*/
			while(g_tick<1){};

			/*Keep door open while their is a motion detected from pir as the people still entering*/
			while(PIR_Motion()==MOTION_DECTECTED);

			/*When the PEOPLE enter send byte NO_MOTION_DECTECTED */
			send_Byte(NO_MOTION_DECTECTED);

			/*Stop Timer1*/
			Timer_DeInit(Timer1);

			g_tick=0;

			/*Turn on the motor clockwise */
			DCMOTOR_rotateWithControl(ROTATE_ANTI_CLOCKWISE, 100, FIRST_DC_MOTOR, PWM0);

			Timer_configuration.timer_compare_MatchValue=58594;
			/*Initialize Timer 1*/
			Timer_init(&Timer_configuration);

			/*Wait for 15 sec*/
			while(g_tick<2){};

			/*Stop Timer1*/
			Timer_DeInit(Timer1);

			g_tick=0;
			/*Turn off the motor*/
			DCMOTOR_rotateWithControl(STOP_ROTATION, 0, FIRST_DC_MOTOR, PWM0);

			/*go to MAIN_MENU_OPTION_STAGE in next loop*/
			g_MC2_application_sequence=MAIN_MENU_OPTION_STAGE;
			break;
		}	/*End of case OPEN_DOOR_STAGE*/

		}	/*End of switch case*/


	}/*End of while(1)*/

}/*End of Main Function*/

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Function Description :
 * used to increment a global variable to indicate number of interrupts happend for timer1
 */
void timer1_SetRequiredCallBackFunction(void)
{
	g_tick++;
}

/*
 * Function Description :
 * Receiving Password entered by the user from HMI_ECU and store it in array defined global
 */
void receive_Password(uint8* a_PasswordArray)
{
	uint8 i=0;
	while(UART_recieveByte()!=READY);
	UART_sendByte(READY);
	while(i<PASSWORD_SIZE)
	{
		a_PasswordArray[i]=UART_recieveByte();
		i++;

	}
	UART_sendByte(DONE);
}

/*
 * Function Description :
 * 1. Taking two passwords
 * 2. Check if both are the same
 * 3. return if they are matched or not
 */
uint8 check_TwoPasswords(uint8 *Password_array1,uint8 *Password_array2)
{
	uint8 i;
	for(i=0;i<PASSWORD_SIZE;i++)
	{
		if(Password_array1[i]!=Password_array2[i])
		{
			return NOT_MATCHED;
		}
	}
	return MATCHED;
}

/*
 * Function Description :
 * synchronising the UART in the Control_ECU and the UART in HMI_ECU
 * Receiving data from the UART in the HMI_ECU
 */
uint8 recieve_Byte(void)
{
	uint8 byte;
	while(UART_recieveByte() != READY){}
	UART_sendByte(READY);
	byte=UART_recieveByte();
	UART_sendByte(DONE);
	return byte;
}

/*
 * Description :
 * synchronising the UART in the Control_ECU and the UART in HMI_ECU
 * Sending data to the UART in the HMI_ECU
 */
void send_Byte(uint8 byte)
{
	UART_sendByte(READY);
	while(UART_recieveByte() != READY){}
	UART_sendByte(byte);
	while(UART_recieveByte() != DONE){}
}
