/**
 * @file can_command_list.h
 * Configurations for all valid command ID's.
 *
 * @author Logan Furedi <logan.furedi@umsats.ca>
 *
 * @date March 3, 2024
 */

#ifndef CAN_WRAPPER_MODULE_INC_CAN_COMMAND_LIST_H_
#define CAN_WRAPPER_MODULE_INC_CAN_COMMAND_LIST_H_

#include <stdint.h>

typedef enum
{
	SENSOR_PCB_THERMOMETER = 0,
	SENSOR_MCU_THERMOMETER,
	SENSOR_PLD_WELL_THERMISTOR,
	SENSOR_PLD_WELL_PHOTOCELL,
} SensorID;

typedef enum
{
	////////////////////////////////////////////
	/// COMMON
	////////////////////////////////////////////
	CMD_PREPRARE_FOR_SHUTDOWN            = 0x00,
	CMD_RESET,
	CMD_GET_PCB_TEMP,
	CMD_GET_MCU_TEMP,

	////////////////////////////////////////////
	/// CDH
	////////////////////////////////////////////
	// Misc.
	CMD_CDH_HEARTBEAT                    = 0x20,
	CMD_CDH_PROCESS_ERROR,
	CMD_CDH_READY_FOR_SHUTDOWN,

	// Logging
	CMD_CDH_LOG_STARTUP                  = 0x30,
	CMD_CDH_LOG_PCB_TEMP,
	CMD_CDH_LOG_MCU_TEMP,
	CMD_CDH_LOG_CONVERTER_STATUS,
	CMD_CDH_LOG_WELL_LIGHT,
	CMD_CDH_LOG_WELL_TEMP,
	CMD_CDH_LOG_LED_TEST,

	// Tests
	CMD_CDH_TEST_FLASH                   = 0x40,
	CMD_CHD_TEST_MRAM,

	// Antenna
	CMD_CDH_ENABLE_ANTENNA_DEPLOYMENT,
	CMD_CDH_DEPLOY_ANTENNA,
	CMD_CDH_TRANSMIT_UHF_BEACON,

	// RTOS
	CMD_CDH_GET_NUM_TASKS,
	CMD_CDH_SAMPLE_TIME_TAGGED_TASK,

	// CLock
	CMD_CDH_SET_RTC,
	CMD_CDH_GET_RTC,

	CMD_CDH_SET_TELEMETRY_INTERVAL,

	////////////////////////////////////////////
	/// PAYLOAD
	////////////////////////////////////////////
	CMD_PLD_SET_WELL_LED                 = 0xA0,
	CMD_PLD_SET_WELL_HEATER,
	CMD_PLD_SET_WELL_TEMP,
	CMD_PLD_GET_WELL_TEMP,
	CMD_PLD_GET_WELL_LIGHT,
	CMD_PLD_SET_TELEMETRY_INTERVAL,
	CMD_PLD_TEST_LEDS,

	////////////////////////////////////////////
	/// ADCS
	////////////////////////////////////////////
	CMD_ADCS_SET_MAGNETORQUER_POWER       = 0xB0,
	CMD_ADCS_SET_MAGNETORQUER_DIRECTION,
	CMD_ADCS_SET_TELEMETRY_INTERVAL,

	////////////////////////////////////////////
	/// POWER
	////////////////////////////////////////////
	CMD_PWR_SET_SUBSYSTEM_POWER           = 0xC0,
	CMD_PWR_SET_BATTERY_HEATER,
	CMD_PWR_GET_CONVERTER_STATUS,
	CMD_PWR_SET_TELEMETRY_INTERVAL
} CmdID;

typedef struct
{
	uint8_t dlc;
	uint8_t priority;
} CmdConfig;

const CmdConfig cmd_configs[0xD0] = {
		///CMD                                 //DLC  //PRIORITY
		[CMD_PREPRARE_FOR_SHUTDOWN]            ={1,     0       },
		[CMD_RESET]                            ={1,     0       },
		[CMD_GET_PCB_TEMP]                     ={1,     0       },
		[CMD_GET_MCU_TEMP]                     ={1,     0       },

		[CMD_CDH_HEARTBEAT]                    ={1,     0       },
		[CMD_CDH_PROCESS_ERROR]                ={1,     0       },
		[CMD_CDH_READY_FOR_SHUTDOWN]           ={1,     0       },

		[CMD_CDH_LOG_STARTUP]                  ={1,     0       },
		[CMD_CDH_LOG_PCB_TEMP]                 ={1,     0       },
		[CMD_CDH_LOG_MCU_TEMP]                 ={1,     0       },
		[CMD_CDH_LOG_CONVERTER_STATUS]         ={1,     0       },
		[CMD_CDH_LOG_WELL_LIGHT]               ={1,     0       },
		[CMD_CDH_LOG_WELL_TEMP]                ={1,     0       },
		[CMD_CDH_LOG_LED_TEST]                 ={1,     0       },

		[CMD_CDH_TEST_FLASH]                   ={1,     0       },
		[CMD_CHD_TEST_MRAM]                    ={1,     0       },

		[CMD_CDH_ENABLE_ANTENNA_DEPLOYMENT]    ={1,     0       },
		[CMD_CDH_DEPLOY_ANTENNA]               ={1,     0       },
		[CMD_CDH_TRANSMIT_UHF_BEACON]          ={1,     0       },

		[CMD_CDH_GET_NUM_TASKS]                ={1,     0       },
		[CMD_CDH_SAMPLE_TIME_TAGGED_TASK]      ={1,     0       },

		[CMD_CDH_SET_RTC]                      ={1,     0       },
		[CMD_CDH_GET_RTC]                      ={1,     0       },

		[CMD_CDH_SET_TELEMETRY_INTERVAL]       ={1,     0       },

		[CMD_PLD_SET_WELL_LED]                 ={1,     0       },
		[CMD_PLD_SET_WELL_HEATER]              ={1,     0       },
		[CMD_PLD_SET_WELL_TEMP]                ={1,     0       },
		[CMD_PLD_GET_WELL_TEMP]                ={1,     0       },
		[CMD_PLD_GET_WELL_LIGHT]               ={1,     0       },
		[CMD_PLD_SET_TELEMETRY_INTERVAL]       ={1,     0       },
		[CMD_PLD_TEST_LEDS]                    ={1,     0       },

		[CMD_ADCS_SET_MAGNETORQUER_POWER]      ={1,     0       },
		[CMD_ADCS_SET_MAGNETORQUER_DIRECTION]  ={1,     0       },
		[CMD_ADCS_SET_TELEMETRY_INTERVAL]      ={1,     0       },

		[CMD_PWR_SET_SUBSYSTEM_POWER]          ={1,     0       },
		[CMD_PWR_SET_BATTERY_HEATER]           ={1,     0       },
		[CMD_PWR_GET_CONVERTER_STATUS]         ={1,     0       },
		[CMD_PWR_SET_TELEMETRY_INTERVAL]       ={1,     0       },
};

#endif /* CAN_WRAPPER_MODULE_INC_CAN_COMMAND_LIST_H_ */
