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

#include <sys/_stdint.h>
#include <stdbool.h>
#include "can_command_list.h"

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

typedef struct
{
	uint8_t priority; // 0-63.
	NodeID sender;
	NodeID recipient;
	bool is_ack_flag;
} CANMessageInfo;

#endif /* CAN_WRAPPER_MODULE_INC_CAN_MESSAGE_H_ */
