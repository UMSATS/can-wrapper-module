/**
 * @file can_wrapper.c
 * CAN wrapper for simplified message receipt & transmission.
 *
 * @author Logan Furedi <logan.furedi@umsats.ca>
 *
 * @date February 28, 2024
 */

#include <can_queue.h>
#include <can_wrapper.h>
#include <stddef.h>

#define ACK_MASK       0b00000000001
#define RECIPIENT_MASK 0b00000000110
#define SENDER_MASK    0b00000011000
#define PRIORITY_MASK  0b11111100000

static CANWrapper_InitTypeDef s_init_struct = {0};
static CANQueue s_msg_queue = {0};
static CANQueueItem s_queue_item = {0}; // the current message being processed.
static bool s_init = false;

CANWrapper_StatusTypeDef CANWrapper_Init(CANWrapper_InitTypeDef init_struct)
{
	if ( !(init_struct.node_id <= 3
		&& init_struct.message_callback != NULL
		&& init_struct.hcan != NULL
		&& init_struct.htim != NULL))
	{
		return CAN_WRAPPER_INVALID_ARGS;
	}

	const CAN_FilterTypeDef filter_config = {
			.FilterIdHigh         = 0x0000,
			.FilterIdLow          = 0x0000,
			.FilterMaskIdHigh     = 0x0000,
			.FilterMaskIdLow      = 0x0000,
			.FilterFIFOAssignment = CAN_FILTER_FIFO0,
			.FilterBank           = 0,
			.FilterMode           = CAN_FILTERMODE_IDMASK,
			.FilterScale          = CAN_FILTERSCALE_32BIT,
			.FilterActivation     = ENABLE,
			.SlaveStartFilterBank = 14,
	};

	if (HAL_CAN_ConfigFilter(init_struct.hcan, &filter_config) != HAL_OK)
	{
		return CAN_WRAPPER_FAILED_TO_CONFIG_FILTER;
	}

	if (HAL_CAN_Start(init_struct.hcan) != HAL_OK)
	{
		return CAN_WRAPPER_FAILED_TO_START_CAN;
	}

	// enable CAN interrupt.
	if (HAL_CAN_ActivateNotification(init_struct.hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		return CAN_WRAPPER_FAILED_TO_ENABLE_INTERRUPT;
	}

	if (HAL_TIM_Base_Start(init_struct.htim) != HAL_OK)
	{
		return CAN_WRAPPER_FAILED_TO_START_TIMER;
	}

	s_msg_queue = CANQueue_Create();

	s_init_struct = init_struct;
	s_queue_item = (CANQueueItem){0};

	s_init = true;
	return CAN_WRAPPER_HAL_OK;
}

CANWrapper_StatusTypeDef CANWrapper_Poll_Messages()
{
	if (!s_init) return CAN_WRAPPER_NOT_INITIALISED;

	while (CANQueue_Dequeue(&s_msg_queue, &s_queue_item))
	{
		if (s_queue_item.info.is_ack_flag)
		{
			// TODO: delete the entry for this message.
		}
		else
		{
			s_init_struct.message_callback(s_queue_item.msg, s_queue_item.info);
		}
	}

	// TODO: check for timeouts.

	return CAN_WRAPPER_HAL_OK;
}

CANWrapper_StatusTypeDef CANWrapper_Send(CANMessage message, NodeID recipient)
{
	if (!s_init) return CAN_WRAPPER_NOT_INITIALISED;

	uint32_t tx_mailbox; // transmit mailbox.
	CAN_TxHeaderTypeDef tx_header;

	// TX message parameters.
	uint16_t id = message.priority      << 5 & PRIORITY_MASK
	            | s_init_struct.node_id << 3 & SENDER_MASK
	            | recipient             << 1 & RECIPIENT_MASK;

	tx_header.StdId = id;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.DLC = CAN_MESSAGE_LENGTH;

	// wait to send CAN message.
	while (HAL_CAN_GetTxMailboxesFreeLevel(s_init_struct.hcan) == 0){}

	return HAL_CAN_AddTxMessage(s_init_struct.hcan, &tx_header, message.data, &tx_mailbox);
}

// called by HAL when a new CAN message is received and pending.
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if (hcan == s_init_struct.hcan)
	{
		HAL_StatusTypeDef status;

		CAN_RxHeaderTypeDef rx_header; // message header.
		CANQueueItem queue_item = {0};

		// get CAN message.
		status = HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, queue_item.msg.data);
		if (status != HAL_OK)
			return; // in theory this should never happen. :p

		queue_item.info.recipient = (RECIPIENT_MASK & rx_header.StdId) >> 1;

		if (queue_item.info.recipient == s_init_struct.node_id) // TODO: use CAN filtering instead.
		{
			queue_item.info.priority = rx_header.RTR == CAN_RTR_REMOTE ? 0x7F : rx_header.ExtId >> 24;
			queue_item.info.sender = (SENDER_MASK & rx_header.StdId) >> 3;

			// TODO: send ACK.

			CANQueue_Enqueue(&s_msg_queue, queue_item);
		}
	}
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) // TODO
{
	if (HAL_CAN_GetError(hcan) & HAL_CAN_ERROR_ACK)
	{
		// timed out.
	}

	if (HAL_CAN_GetError(hcan) & HAL_CAN_ERROR_EWG)
	{
		// error warning. (96 errors recorded from transmission or receipt)
	}

	if (HAL_CAN_GetError(hcan) & HAL_CAN_ERROR_EPV)
	{
		// entered error passive state. (more than 16 failed transmission attempts and/or 128 failed receipts)
	}

	if (HAL_CAN_GetError(hcan) & HAL_CAN_ERROR_BOF)
	{
		// entered bus-off state. (more than 32 failed transmission attempts)
	}
}
