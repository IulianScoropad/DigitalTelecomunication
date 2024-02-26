#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "LPC177x_8x.h"
#include "system_LPC177x_8x.h"
#include <retarget.h>

#include <DRV\drv_sdram.h>
#include <DRV\drv_lcd.h>
#include <DRV\drv_uart.h>
#include <DRV\drv_touchscreen.h>
#include <DRV\drv_led.h>
#include <utils\timer_software.h>
#include <utils\timer_software_init.h>
#include "parser.h"


const char at_command_simple[]= "AT\r\n";
const char at_command_csq[] = "AT+CSQ\r\n";
const char at_command_gmi[] = "AT+GMI\r\n";
const char at_command_gsn[] = "AT+GSN\r\n";
const char at_command_gmr[] = "AT+GMR\r\n";
const char at_command_creg[] = "AT+CREG?\r\n";
const char at_command_cops[] = "AT+COPS?\r\n";


timer_software_handler_t my_timer_handler; 
timer_software_handler_t my_handler; 
AT_COMMAND_DATA var;

uint8_t ExtractData(AT_COMMAND_DATA* data, uint32_t line, uint32_t column, int l)
{
	uint8_t final_data;
	uint32_t linie;
	uint32_t coloana;
			final_data=0;
			for(coloana=0; coloana < AT_COMMAND_MAX_LINE_SIZE; coloana++)
         if (var.data[l][coloana] != '\0' && var.data[l][coloana]>'0' && var.data[l][coloana]<'9')
           { 	
							final_data = final_data*10 + (var.data[l][coloana]-'0'); 
           }
         

	return final_data;
}

void ExtractDataNew(AT_COMMAND_DATA* data, uint32_t line, uint32_t column, char *result, int l)
{
	
	uint8_t final_data;
	
	uint32_t linie;
	uint32_t coloana;
	uint8_t index=0;

			for(coloana=0; coloana < column; coloana++){
         if (var.data[l][coloana] != '\0')
           { 	
							result[index]= var.data[l][coloana];	
						 index++;
				
					 }  
			
   				 
  }
	result[index]='\0';	

}

uint32_t ConvertAsuToDbmw(uint32_t value)
{
	return 2 * value - 113 ;
}

STATE_MACHINE_RETURN_VALUE CommandResponseValid()
{
	
	if(var.ok ==1){
	return STATE_MACHINE_READY_OK;
	}
	else return STATE_MACHINE_READY_WITH_ERROR;
}

void timer_callback_1(timer_software_handler_t h)
{
}

void testLCD()
{
	uint32_t i,j;
	LCD_PIXEL foreground = {0, 255, 0, 0};
	LCD_PIXEL background = {0, 0, 0, 0};
	
	
	for (i = 0; i < LCD_HEIGHT; i++)
	{
		for (j = 0; j < LCD_WIDTH / 3; j++)
		{
			DRV_LCD_PutPixel(i, j, 255, 0, 0);
		}
		for (j = LCD_WIDTH / 3; j < 2 * (LCD_WIDTH / 3); j++)
		{
			DRV_LCD_PutPixel(i, j, 230, 220, 0);
		}
		for (j = 2 * (LCD_WIDTH / 3); j < LCD_WIDTH; j++)
		{
			DRV_LCD_PutPixel(i, j, 0, 0, 255);
		}
	}

	DRV_LCD_Puts("Hello", 20, 30, foreground, background, TRUE);
	DRV_LCD_Puts("Hello", 20, 60, foreground, background, FALSE);	
}

void TouchScreenCallBack(TouchResult* touchData)
{
	printf("touched X=%3d Y=%3d\n", touchData->X, touchData->Y);		
	
}

void BoardInit()
{
	
	timer_software_handler_t handler;
	
	TIMER_SOFTWARE_init_system();
	
	
	
	DRV_SDRAM_Init();
	
	initRetargetDebugSystem();
	DRV_LCD_Init();
	DRV_LCD_ClrScr();
	DRV_LCD_PowerOn();	
	
	DRV_TOUCHSCREEN_Init();
	DRV_TOUCHSCREEN_SetTouchCallback(TouchScreenCallBack);
	DRV_LED_Init();
	printf ("Hello\n");	
	
	handler = TIMER_SOFTWARE_request_timer();
	TIMER_SOFTWARE_configure_timer(handler, MODE_1, 100, 1);
	TIMER_SOFTWARE_set_callback(handler, timer_callback_1);
	TIMER_SOFTWARE_start_timer(handler);
	
	
	
	
}

void GetCommandResponse(const char *command)
{
	 uint8_t ch;
	 BOOLEAN ready = FALSE;
	 TIMER_SOFTWARE_reset_timer(my_handler);
	 TIMER_SOFTWARE_start_timer(my_handler);
		 while ((!TIMER_SOFTWARE_interrupt_pending(my_handler)) && (ready == FALSE))
		 {
				while (DRV_UART_BytesAvailable(UART_3) > 0)
			 {
				 DRV_UART_ReadByte(UART_3, &ch);
				 if (at_command_parse(ch) != STATE_MACHINE_NOT_READY)
					{
						ready = TRUE;
						
						CommandResponseValid();
						
				 }
			 }
		 }
}

void SendCommand(const char *command)
{
 DRV_UART_FlushRX(UART_3);
 DRV_UART_FlushTX(UART_3);
 DRV_UART_Write(UART_3, (unsigned char*)command, strlen(command));
}

void ExecuteCommand(const char *command)
{
 SendCommand(command);
 GetCommandResponse(command);
} 

int main(void)
{

	uint32_t rssi_value_asu;
	uint32_t rssi_value_dbmw;
	char data[50];

	
	
	BoardInit();
	testLCD();
	
	my_timer_handler = TIMER_SOFTWARE_request_timer();
	TIMER_SOFTWARE_configure_timer(my_timer_handler, MODE_1, 5000, 1);
	TIMER_SOFTWARE_start_timer(my_timer_handler);
	
	
	
	
	
	DRV_UART_Configure(UART_3, UART_CHARACTER_LENGTH_8, 115200, UART_PARITY_NO_PARITY, 1, TRUE, 3);
	DRV_UART_Configure(UART_0, UART_CHARACTER_LENGTH_8, 115200, UART_PARITY_NO_PARITY, 1, TRUE, 3);
	
	DRV_UART_Write(UART_3, (unsigned char*)at_command_simple, strlen(at_command_simple));
  DRV_UART_Write(UART_3, (unsigned char*)at_command_csq, strlen(at_command_csq));
  DRV_UART_Write(UART_3, (unsigned char*)at_command_gmi, strlen(at_command_gmi));
  DRV_UART_Write(UART_3, (unsigned char*)at_command_gsn, strlen(at_command_gsn));
  DRV_UART_Write(UART_3, (unsigned char*)at_command_gmr, strlen(at_command_gmr));
  DRV_UART_Write(UART_3, (unsigned char*)at_command_creg, strlen(at_command_creg));
  DRV_UART_Write(UART_3, (unsigned char*)at_command_cops, strlen(at_command_cops));
	TIMER_SOFTWARE_Wait(3000);
	
	ExecuteCommand(at_command_csq);
   ExecuteCommand(at_command_gmi);
	ExecuteCommand(at_command_gsn);
	ExecuteCommand(at_command_gmr);
	ExecuteCommand(at_command_creg);
	ExecuteCommand(at_command_cops);
  TIMER_SOFTWARE_Wait(3000);
 
	
	while(1)
	{
		if (TIMER_SOFTWARE_interrupt_pending(my_timer_handler))
		{

					if (CommandResponseValid())
					{ 
						rssi_value_asu = ExtractData(&var, var.line_count, var.col_count,0);
						rssi_value_dbmw = ConvertAsuToDbmw(rssi_value_asu);
						printf("GSM Modem signal %d ASU -> %d dBmW \n",rssi_value_asu , rssi_value_dbmw);
					}
					TIMER_SOFTWARE_clear_interrupt(my_timer_handler);

					if (CommandResponseValid())
					{ 
						ExtractDataNew(&var, var.line_count, var.col_count, data,1);
						printf("Modem manufacturer: %s\n", data);
						
					}
					TIMER_SOFTWARE_clear_interrupt(my_timer_handler);

					if (CommandResponseValid())
					{ 
						ExtractDataNew(&var, var.line_count, var.col_count, data,2);
						printf("Modem IMEI: %s\n", data);
						
					}
					TIMER_SOFTWARE_clear_interrupt(my_timer_handler);
	
			
				if (CommandResponseValid())
				{ 
					ExtractDataNew(&var, var.line_count, var.col_count, data,3);
					printf("Modem software version: %s\n", data+9);
			
					
				}				
				TIMER_SOFTWARE_clear_interrupt(my_timer_handler);

			
				if (CommandResponseValid())
				{ 
					ExtractDataNew(&var, var.line_count, var.col_count, data,4);
					if(data[8]=='0')
					printf("State of registration: Is not registered in the network and is not searching for a network\n");
					else if(data[8]=='1')
					printf("State of registration: Home Network\n");
					else if(data[8]=='2')
					printf("State of registration: Is not registered but it is currently searching for a network\n");
					else if(data[8]=='3')
					printf("State of registration: Was denied\n");
					else if(data[8]=='4')
					printf("State of registration: Unknown modem registration state\n");
					else if(data[8]=='5')
					printf("State of registration: Modem is registered to roaming network \n");
				}	
						
				TIMER_SOFTWARE_clear_interrupt(my_timer_handler);
	
			
				if (CommandResponseValid())
				{ 
					ExtractDataNew(&var, var.line_count, var.col_count, data,5);
					printf("Operator name: %s\n", data+6);
								
				}			
				TIMER_SOFTWARE_clear_interrupt(my_timer_handler);
			
			}

}
	
	/*while(1)
	{
		DRV_LED_Toggle(LED_4);
		
	}*/
	/*
	while(1)
	{
		DRV_UART_SendByte(UART_3, 'A');
	//	TIMER_SOFTWARE_Wait(1000);
	}
	*/
	/*
	while(1)
	{
		if (DRV_UART_ReadByte(UART_3, &ch) == OK)
		{
			DRV_UART_SendByte(UART_3, ch);
		}		
	}
*/
	/*while(1)
	{
		printf("thrfd\n ");
		if (DRV_UART_ReadByte(UART_0, &ch) == OK)
		{
			DRV_UART_SendByte(UART_3, ch);
		}
		if (DRV_UART_ReadByte(UART_3, &ch) == OK)
		{
			DRV_UART_SendByte(UART_0, ch);
		}
		if (DRV_UART_ReadByte(UART_2, &ch) == OK)
		{
			DRV_UART_SendByte(UART_0, ch);
		}
	}
	
	while(1)
	{
		DRV_UART_Process();
		DRV_TOUCHSCREEN_Process();
	}
	*/
	return 0; 
}
