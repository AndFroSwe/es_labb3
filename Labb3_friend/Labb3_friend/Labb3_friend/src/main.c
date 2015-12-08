/********************************************************
Name : main.c

CAN example

**********************************************************/

#include <asf.h>
#include "can.h"

#define CAN_1000kbps 1
#define CAN_500kbps 5
#define CAN_250kbps 7
#define CAN_125kbps 10

UINT32 Ident;
UINT8 msg[8], mSize;

int main(void) {
	//spidatareadpointer=&spidataread;
	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);
	
	// Configures the MCP2515 SPI communication.
	config_SPI_SPARE();

	// Enables receive interrupts.
	Disable_global_interrupt();
	INTC_init_interrupts();
	Enable_global_interrupt();
	
	// Delay to let the Oscillator get started
	delay_init( FOSC0 );
	
	// Initializes the display
	config_dpi204();
	dip204_init(100,1);
	dip204_clear_display();
	
	// Define dashboard parameters
	UINT16 airsupply = 0x18FEAE30;
	UINT16 tach = 0xCFE6CEE;
	UINT16 time = 0x18FEE6EE;
	UINT16 eec = 0xCF00400;
	UINT16 enginetemp = 0x18FEEE00;0, 
	UINT16 ambient = 0x18FEF527;
	
	// Set filter and mask
	UINT16 Mask = 0xFFFFFFF; 
	UINT16 flt = time;
	UINT16 Flt[] = {flt,flt,flt,flt,flt,flt};
	InitializeCANExtended(0, CAN_250kbps, Mask, Flt);
	
	dip204_set_cursor_position(1,1);
	dip204_printf_string("ES1 CAN EXAMPLE");
	dip204_hide_cursor();

	while(1){
		//Clear memory contents
		ClearMessages(msg);
		//Read any message available
		if(CANRxReady(0)){
			if( CANGetMsg(0, &Ident, msg, &mSize )) // Gets message and returns //TRUE if message received.
			{	
				// Evk1100PrintDisplay prints 4 message values, the Identifier and the data size on the display
				dip204_clear_display();
				Evk1100PrintDisp(&Ident, msg, &mSize );	
				delay_ms(1000);
				//dip204_clear_display();
			}
		}
		// Send messages if possible
		if(CANTxReady(0))
		{
			msg[0]=0;
			msg[1]=0;
			msg[2]=0;
			msg[3]=0;
			msg[4]=0;
			msg[5]=0;
			msg[6]=0;
			msg[7]=100;
			// Channel, Identifier (max 0x1fffffff (29 bits)), Message, Number of bytes, R //or 0 (Remote frame or no remote frame).
			//CANSendMsg( 0, tach , msg, 8, 0 );
			delay_ms(100);
		}
	}
	return 0;
}