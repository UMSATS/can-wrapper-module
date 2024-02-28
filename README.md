# CAN Wrapper

This module wraps a simpler interface around HAL's CAN interface for sending & receiving messages onboard the TSAT satellite.

## Importing

You will need CAN set up in order to use this module.

To set up **CAN**, you will need to:

1. Enable a CAN peripheral in your `.ioc` file under `Pinout & Configuration`.
2. Configure your CAN peripheral as such:
   - In `NVIC Settings`, enable `RX0 interrupt`.
   - In `Parameter Settings`, set `Prescaler (for Time Quantum)` to `16`.
   - In `Parameter Settings`, set `Time Quanta in Bit Segment 1` to `5 Times`.
   - In `Parameter Settings`, set `Time Quanta in Bit Segment 2` to `4 Times`.

>Note: These Quanta settings are recommended as of Feb 27, 2024 and are subject to change.

3. Save & regenerate code.
4. Ensure `MX_CAN#_Init();` has been generated in `main.c`

To import **CAN Wrapper** you will need to:

1. Open your terminal in the root folder of your STM32 project
2. Enter the following command:

```bash
git submodule add https://github.com/UMSATS/can-wrapper-module Drivers/can-wrapper-module
```

3. From STM32CubeIDE, right click on the `can-wrapper-driver/Inc` folder and click `Add/remove include path...`.
4. Leave all configurations selected and hit `OK`.
5. CAN Wrapper requires a dedicated timer peripheral to work. Enable `TIM16` in your `.ioc`.
6. In `Parameter Settings`, configure your timer as such:
   - Set `Prescalar` to `80 - 1`.
   - Set `Counter Period` to `5000 - 1`.
7. Save & regenerate code.

## Initialisation

This driver uses a flexible *callback* approach to events such as incoming messages and transmission failures.

All initialisation settings are to be stored in a `CANWrapper_InitTypeDef` struct.

E.g:

```c
CANWrapper_InitTypeDef wc_init = {
		.node_id = NODE_ADCS, // your subsystem's unique ID in the CAN network.
		
		.hcan = &hcan1,  // pointer to the CAN peripheral handle.
		.htim = &htim16, // pointer to the timer handle.
		
		.message_callback = &on_message_received, // to process incoming messages.
		.send_failure_callback = &on_send_failed // timeout during transmission.
};
```

Pass in your settings when calling `CANWrapper_Init` (sometime after `MX_CAN#_Init`):

```c
CANWrapper_Init(wc_init);
```

Here is an adaptable template for a message handler to start:

```c
#include "can_wrapper.h"
void on_message_received(CANMessage msg)
{
	CANMessageBody response_body = { .data = { msg.command_id } };

	// the reset command is a special case. (you must send a response FIRST)
	if (msg.command_id == CMD_RESET)
	{
		CANWrapper_Send_Response(true, response_body);
		Perform_Manual_Reset(); // implement this as per your subsystem.
	}

	HAL_StatusTypeDef status = HAL_OK; // use an appropriate enum in your case.
	uint8_t response_data_size = 0;

	switch (msg.command_id)
	{
		case CMD_LED_ON: // example command.
		{
			status = LEDs_Set_LED(msg.data[1], ON);
			response_body.data[1] = msg.data[1];
			response_data_size = 1;
			break;
		}
		// ...
		default:
		{
			status = HAL_ERROR;
			break;
		}
	}

	if (status != HAL_OK && response_data_size < CAN_MESSAGE_LENGTH-1)
	{
		response_body.data[response_data_size+1] = status; // append error code to NACK.
	}

	CANWrapper_Send_Response(status == HAL_OK, response_body); // return to sender.
}
```

## Updating

To update your copy of the module to the most recent commit, open your project in a terminal and enter:

```bash
git submodule update
```

## Additional Examples

### Create & Send a CAN Message

```c
#define CDH_ID               0x01
#define CMD_HELLO_MESSAGE    0xA3

void Say_Hello()
{
	CANMessage my_message = {
	    .recipient_id = CDH_ID,
	    .priority = 3,
	    .command_id = CMD_HELLO_MESSAGE,
	    .body = {
	        .data = {
	            'H', 'e', 'l', 'l', 'o', '\0'
	        }
	    }
	};
	
	CANWrapper_Send_Message(my_message);
}
```

### Accessing `CANMessage` Data Members

The `CANMessage` type is designed to be flexible in that you can use any of the fields:

 - `.command_id`
 - `.body`
 - `.data`

to access different parts of a CAN message.

```c
CANMessage msg;

msg.command_id = 0xB5;  // set command ID to 0xB5.
msg.data[0] = 0xB6;     // another way to set command ID (this time to 0xB6).

msg.body.data[0] = 'A'; // set first byte in message body. (ie. the byte after command ID)
msg.data[1] = 'A';      // equivalent.

CANMessageBody my_body = { .data = { 'H', 'i', '\0' } };
msg.body = my_body;     // replace the entire message body struct.
```

See `can_message.h` for the full structure definition.

## More Reading

The below link is old, so I don't recommend reading it, but I will leave it here in case you want to read the old documentation of this interface or you want to understand the high level concepts of how CAN works.

<https://drive.google.com/file/d/1HHNWpN6vo-JKY5VvzY14uecxMsGIISU7/view?usp=share_link>
