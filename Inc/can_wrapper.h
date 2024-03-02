/**
 * @file can_wrapper.h
 * CAN wrapper for simplified initialisation, message reception, and message
 * transmission.
 *
 * @author Logan Furedi <logan.furedi@umsats.ca>
 *
 * @date February 27, 2024
 */

#ifndef CAN_WRAPPER_MODULE_INC_CAN_WRAPPER_H_
#define CAN_WRAPPER_MODULE_INC_CAN_WRAPPER_H_

#include <can_message.h>
#include <stdbool.h>
#include <stm32l4xx.h>
#include <stm32l4xx_hal_can.h>
#include <stm32l4xx_hal_tim.h>
#include <stm32l4xx_hal_def.h>
#include <sys/_stdint.h>

typedef enum
{
	CAN_WRAPPER_HAL_OK = HAL_OK,
	CAN_WRAPPER_HAL_ERROR,
	CAN_WRAPPER_HAL_BUSY,
	CAN_WRAPPER_HAL_TIMEOUT,
	CAN_WRAPPER_INVALID_ARGS,
	CAN_WRAPPER_NOT_INITIALISED,
	CAN_WRAPPER_FAILED_TO_CONFIG_FILTER,
	CAN_WRAPPER_FAILED_TO_START_CAN,
	CAN_WRAPPER_FAILED_TO_ENABLE_INTERRUPT,
	CAN_WRAPPER_FAILED_TO_START_TIMER,
} CANWrapper_StatusTypeDef;

typedef enum
{
	CAN_WRAPPER_TIMEOUT = 0,
	CAN_WRAPPER_CAN_TIMEOUT,
} CANWrapper_SendError;

typedef void (*CANMessageCallback)(CANMessage, CANMessageInfo);
typedef void (*CANSendFailureCallback)(CANWrapper_SendError, CANMessage);

typedef struct
{
	NodeID node_id;           // your subsystem's unique ID in the CAN network.

	CAN_HandleTypeDef *hcan;  // pointer to the CAN peripheral handle.
	TIM_HandleTypeDef *htim;  // pointer to the timer handle.

	CANMessageCallback message_callback; // called when a new message is polled.
	CANSendFailureCallback send_failure_callback; // called when a message fails to send.
} CANWrapper_InitTypeDef;

/**
 * @brief				Performs necessary setup for normal functioning.
 *
 * @param init_struct   Configuration for initialisation.
 */
CANWrapper_StatusTypeDef CANWrapper_Init(CANWrapper_InitTypeDef init_struct);

/**
 * @brief               Polls the CAN queue for incoming messages.
 *
 * This is the point where callback functions will be triggered.
 */
CANWrapper_StatusTypeDef CANWrapper_Poll_Messages();

/**
 * @brief               Sends a message over CAN.
 *
 * @param message       See CANMessage definition.
 * @param recipient     ID of the intended recipient.
 */
CANWrapper_StatusTypeDef CANWrapper_Send(CANMessage message, NodeID recipient);

#endif /* CAN_WRAPPER_MODULE_INC_CAN_WRAPPER_H_ */
