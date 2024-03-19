# CAN Wrapper Module

This module simplifies CAN message handling on the TSAT satellite by providing an easy-to-use interface that abstracts our custom protocol.

## Importing

You will need CAN set up in your STM32 project to use this module.

To set up **CAN**, you will need to:

1. Enable a CAN peripheral in your `.ioc` file under `Pinout & Configuration > Connectivity`.
2. Configure your CAN peripheral as such:
   - In `NVIC Settings`, enable `RX0 interrupt`.
   - In `Parameter Settings`, set `Prescaler (for Time Quantum)` to `16`.
   - In `Parameter Settings`, set `Time Quanta in Bit Segment 1` to `5 Times`.
   - In `Parameter Settings`, set `Time Quanta in Bit Segment 2` to `4 Times`.

>Note: These Quanta settings are recommended for microcontrollers running at 80MHz to keep all subsystems synced. Should you decide to change your clock speed, you will need to modify these values accordingly.

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

This driver uses a flexible **callback** approach to handle events such as incoming messages and communication errors.

The  `CANWrapper_InitTypeDef` structure contains many settings that tune the module to your needs.

```c
CANWrapper_InitTypeDef wc_init = {
		.node_id = NODE_ADCS,    // your subsystem's unique ID in the CAN network.
		.notify_of_acks = false, // whether to notify you of incoming ACK's.

		.hcan = &hcan1,  // pointer to the CAN peripheral handle.
		.htim = &htim16, // pointer to the timer handle.

		.message_callback = &on_message_received, // called when a new message is polled.
		.error_callback = &on_error_occured;      // called when a communication error occurs.
};
```

To initialise CAN Wrapper, call `CANWrapper_Init` with your chosen settings (sometime after `MX_CAN#_Init`):

```c
CANWrapper_Init(wc_init);
```

## Receiving Messages

Here is starter template for a message handling function. Add your specific subsystem's functionality as needed. Note that this code also makes use of the error context utility to catch errors when they occur.

```c
#include "can_wrapper.h"
#include "util/error_context.h"
#include <stdbool.h>

void on_message_received(CANMessage msg, NodeID sender, bool is_ack)
{
	ErrorBuffer error_buffer; // stores command errors.
	ErrorContext_Push_Buffer(&error_buffer); // push to the top of the buffer stack.

	switch (msg.cmd)
	{
		case CMD_PLD_SET_WELL_LED: // example command.
		{
			// get the command arguments as defined in the command reference.
			uint8_t well_id, power;
			GET_ARG(msg, 0, well_id); // syntax: GET_ARG(msg, byte #, output var)
			GET_ARG(msg, 1, power);

			// perform instructed action.
			LEDs_Set_LED(well_id, power);
			break;
		}
		// ...
		default:
		{
			// unrecognized command.
			PUSH_ERROR(ERROR_UNKNOWN_COMMAND, msg.cmd);
			break;
		}
	}

	if (ErrorBuffer_Has_Error(&error_buffer))
	{
		CANMessage error_report;
		error_report.cmd = CMD_PROCESS_ERROR;
		SET_ARG(error_report, 0, error_buffer); // syntax: SET_ARG(msg, byte #, input var)

		CANWrapper_Transmit(sender, &error_report);
	}

	ErrorContext_Pop_Buffer();
}
```

> Note: As of March 17th, the "Error Context" utility has not yet been released due to my busy schedule. If you don't have it, you can either ignore the error reporting code completely or implement a simplified version of it yourself. I recommend waiting however, since it will be coming very soon!

## Handling Errors

Here is starter template for an error handling function.

```c
#include "can_wrapper.h"

void on_error_occured(CANWrapper_ErrorInfo error_info)
{
	switch (error_info.error)
	{
		case CAN_WRAPPER_ERROR_TIMEOUT:
		{
			// your transmission attempt timed out.
			// Here you can resolve the issue as appropriate.

			// You can re-send the message to the intended recipient like so.
			CANWrapper_Transmit(error.recipient, &error.msg);

			break;
		}
	}
}
```

> Warning: The error handling functionality is quite bare in this version. It only notifies of timeouts, but there a plenty of other things that can go wrong. Expect rapid changes and improvements in this area.

## Updating CAN Wrapper

The following steps will update your copy of the module to the most recent commit:
1. Ensure your project repository is up-to-date and clean (`git pull` to update, then `git status` to check if it's clean. commit and push if it's not).
2. `cd` into the root folder of the submodule (likely named `can-wrapper-module`).
3. Enter `git pull`.
4. `cd` back to the root folder of your project.
5. Enter:
```bash
git add .
git commit -m "Update CAN Wrapper Module"
git push
```

## Additional Examples

### Create & Send a CAN Message

```c
#include "can_wrapper.h"
#include <stdbool.h>

bool Report_PCB_Temp()
{
	uint16_t temp;
	bool success = TMP235_Read_Temp(&temp);
	if (success)
	{
		CANMessage my_msg;
		my_msg.cmd = CMD_CDH_PROCESS_PCB_TEMP;
		SET_ARG(my_msg, 0, temp);
		CANWrapper_Transmit(NODE_CDH, &my_msg);
	}

	return success;
}
```

### The `CANMessage` Data Members

The `CANMessage` type has three fields:

 - `.cmd`: the command ID
 - `.body`: the arguments of the command
 - `.data`: the entire message (including command ID & body)

The `data` feild occupies the same section in memory as the other two. Therefore, any change to `cmd` or `body` will reflect in `data`, and vice-versa. Think of `msg.data` as a shorthand for 	`(uint8_t*)msg`.

```c
CANMessage msg;

msg.cmd = CMD_CDH_PROCESS_PCB_TEMP;  // set command ID.
msg.data[0] = CMD_CDH_PROCESS_MCU_TEMP; // another way to set command ID. (less legible)

msg.body[0] = 'A'; // set first byte in message body. (ie. the byte after command ID)
msg.data[1] = 'A'; // equivalent.
SET_ARG(msg, 0, 'A'); // equivalent.

// SET_ARG allows you to assign larger types to the message body very easily.
// Warning: make sure your data is no more than 7 bytes! (56 bits)
uint32_t large_number = 4294967295;
SET_ARG(msg, 0, large_number)

// you can access arguments in a similar way
uint8_t single_byte;
uint32_t four_bytes;
GET_ARG(msg, 0, single_byte); // retrieves byte 0 in the message body.
GET_ARG(msg, 0, four_bytes);  // retrieves bytes 0-3 in the message body.
```

It's recommended to use the `SET_ARG` and `GET_ARG` macros whenever possible since that will potentially result in fewer breaking changes in the event of an update.

See `can_message.h` for the full structure definition.

## Tips 'n' Tricks

 - To quickly search for commands in the code editor, type the name of a prefix (e.g. one of `CMD_PLD`, `CMD_ACDS`, `CMD_PWR`, or `CMD_CDH`) and press `Ctrl + Space`. You'll then be greeted with a list of matching commands. (assuming you've included `can_wrapper.h`)
 - If you haven't already, check out the [Command Reference for TSAT-7](https://docs.google.com/spreadsheets/d/1o90k0nbOKp7gOJZohCGgkKvFZ3N4jlzd4gb5Lt9xrjw/edit#gid=1728821240) to read up on the expected format of each command. Ensuring the format you send is correct is important, as otherwise the data received may be incorrect or it might be cut off.

## More Reading

The below link is old, so I don't recommend reading it, but I will leave it here in case you want to read the documentation for the old version of this interface or you want to understand the high level concepts of how CAN works. Definitely not a required read to use this module.

<https://drive.google.com/file/d/1HHNWpN6vo-JKY5VvzY14uecxMsGIISU7/view?usp=share_link>
