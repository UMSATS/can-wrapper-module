/**
 * @file can_message.h
 * Structures for storing CAN message data.
 *
 * @author Logan Furedi <logan.furedi@umsats.ca>
 *
 * @date February 27, 2024
 */

#ifndef CAN_WRAPPER_MODULE_INC_CAN_MESSAGE_H_
#define CAN_WRAPPER_MODULE_INC_CAN_MESSAGE_H_

#include "can_command_list.h"
#include <sys/_stdint.h>
#include <stdbool.h>
#include <string.h>

#define CAN_MESSAGE_LENGTH 8

typedef enum
{
	NODE_CDH     = 0,
	NODE_POWER   = 1,
	NODE_ADCS    = 2,
	NODE_PAYLOAD = 3
} NodeID;

typedef union
{
	struct {
		uint8_t cmd;
		uint8_t body[CAN_MESSAGE_LENGTH-1]; // just the message body.
	};
	uint8_t data[CAN_MESSAGE_LENGTH]; // the entire message (command ID + body).
} CANMessage;

#define GET_ARG(msg, pos, var) \
	var = *((typeof(var)*)(msg.body + pos)) // this is only safe because TSAT MCU's are all the same endianness.

#endif /* CAN_WRAPPER_MODULE_INC_CAN_MESSAGE_H_ */
