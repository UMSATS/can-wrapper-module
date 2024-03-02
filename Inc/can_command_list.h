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

typedef enum
{
	////////////////////////////////////////////
	/// COMMON
	////////////////////////////////////////////
	CMD_SHUTDOWN                         = 0x00,

	////////////////////////////////////////////
	/// CDH
	////////////////////////////////////////////
	// Misc.(1)
	CMD_CDH_HEARTBEAT                    = 0x20,

	// Telemetry Logging
	CMD_CDH_LOG_BATTERY_CHARGE           = 0x30,
	CMD_CDH_LOG_BATTERY_TEMP,
	CMD_CDH_LOG_BATTERY_CONSUMPTION,
	CMD_CDH_LOG_WELL_LIGHT_LEVEL,
	CMD_CDH_LOG_WELL_TEMP,
	CMD_CDH_LOG_ROTATION_VEC_1,
	CMD_CDH_LOG_ROTATION_VEC_2,

	// Misc. (2)
	CMD_CDH_RESET                        = 0x40,
	CMD_CDH_TEST_FLASH_STORAGE,
	CMD_CDH_TEST_MRAM,
	CMD_CDH_ENABLE_ANTENNA_DEPLOYMENT,
	CMD_CDH_DEPLOY_ANTENNA,
	CMD_CDH_SEND_UHF_BEACON,
	CMD_CDH_GET_NUM_TASKS,
	CMD_CDH_SAMPLE_TIME_TAGGED_TASK,
	CMD_CDH_SET_RTC,
	CMD_CDH_GET_RTC,

	// Error Reporting
	CMD_CDH_REPORT_PAYLOAD_ERROR         = 0x50,
	CMD_CDH_REPORT_ADCS_ERROR,
	CMD_CDH_REPORT_POWER_ERROR,

	////////////////////////////////////////////
	/// PAYLOAD
	////////////////////////////////////////////
	CMD_PLD_RESET                        = 0xA0,
	CMD_PLD_LED_ON,
	CMD_PLD_LED_OFF,
	CMD_PLD_HEATER_ON                    = 0xA5,
	CMD_PLD_HEATER_OFF,
	CMD_PLD_GET_PCB_TEMP,
	CMD_PLD_GET_WELL_LIGHT,
	CMD_PLD_GET_WELL_TEMP,
	CMD_PLD_SET_TELEMETRY_INTERVAL,
	CMD_PLD_TEST_LEDS,
	CMD_PLD_GET_LIGHT_BASELINE,

	////////////////////////////////////////////
	/// ADCS
	////////////////////////////////////////////
	CMD_ADCS_RESET                       = 0xB0,
	CMD_ADCS_MAGNETORQUER_ON,
	CMD_ADCS_MAGNETORQUER_OFF,
	CMD_ADCS_MAGNETORQUER_FORWARD,
	CMD_ADCS_MAGNETORQUER_REVERSE,

	////////////////////////////////////////////
	/// POWER
	////////////////////////////////////////////
	CMD_PWR_RESET                        = 0xC0,
	CMD_PWR_PAYLOAD_ON,
	CMD_PWR_PAYLOAD_OFF,
	CMD_PWR_ADCS_ON,
	CMD_PWR_ADCS_OFF,
	CMD_PWR_BATTERY_ACCESS_ON,
	CMD_PWR_BATTERY_ACCESS_OFF,
	CMD_PWR_BATTERY_HEATER_ON,
	CMD_PWR_BATTERY_HEATER_OFF,
	CMD_PWR_GET_CONVERTER_STATUS
} CmdID;

typedef struct
{
	uint8_t dlc;
	uint8_t priority;
} CmdConfig;

const CmdConfig cmd_configs[0xD0] = {
		///CMD                              //DLC  //PRIORITY
		[CMD_SHUTDOWN]                      ={1,     0       },

		[CMD_CDH_HEARTBEAT]                 ={1,     0       },

		[CMD_CDH_LOG_BATTERY_CHARGE]        ={1,     0       },
		[CMD_CDH_LOG_BATTERY_TEMP]          ={1,     0       },
		[CMD_CDH_LOG_BATTERY_CONSUMPTION]   ={1,     0       },
		[CMD_CDH_LOG_WELL_LIGHT_LEVEL]      ={1,     0       },
		[CMD_CDH_LOG_WELL_TEMP]             ={1,     0       },
		[CMD_CDH_LOG_ROTATION_VEC_1]        ={1,     0       },
		[CMD_CDH_LOG_ROTATION_VEC_2]        ={1,     0       },

		[CMD_CDH_RESET]                     ={1,     0       },
		[CMD_CDH_TEST_FLASH_STORAGE]        ={1,     0       },
		[CMD_CDH_TEST_MRAM]                 ={1,     0       },
		[CMD_CDH_ENABLE_ANTENNA_DEPLOYMENT] ={1,     0       },
		[CMD_CDH_DEPLOY_ANTENNA]            ={1,     0       },
		[CMD_CDH_SEND_UHF_BEACON]           ={1,     0       },
		[CMD_CDH_GET_NUM_TASKS]             ={1,     0       },
		[CMD_CDH_SAMPLE_TIME_TAGGED_TASK]   ={1,     0       },
		[CMD_CDH_SET_RTC]                   ={1,     0       },
		[CMD_CDH_GET_RTC]                   ={1,     0       },

		[CMD_CDH_REPORT_PAYLOAD_ERROR]      ={1,     0       },
		[CMD_CDH_REPORT_ADCS_ERROR]         ={1,     0       },
		[CMD_CDH_REPORT_POWER_ERROR]        ={1,     0       },

		[CMD_PLD_RESET]                     ={1,     0       },
		[CMD_PLD_LED_ON]                    ={1,     0       },
		[CMD_PLD_LED_OFF]                   ={1,     0       },
		[CMD_PLD_HEATER_ON]                 ={1,     0       },
		[CMD_PLD_HEATER_OFF]                ={1,     0       },
		[CMD_PLD_GET_PCB_TEMP]              ={1,     0       },
		[CMD_PLD_GET_WELL_LIGHT]            ={1,     0       },
		[CMD_PLD_GET_WELL_TEMP]             ={1,     0       },
		[CMD_PLD_SET_TELEMETRY_INTERVAL]    ={1,     0       },
		[CMD_PLD_TEST_LEDS]                 ={1,     0       },
		[CMD_PLD_GET_LIGHT_BASELINE]        ={1,     0       },

		[CMD_ADCS_RESET]                    ={1,     0       },
		[CMD_ADCS_MAGNETORQUER_ON]          ={1,     0       },
		[CMD_ADCS_MAGNETORQUER_OFF]         ={1,     0       },
		[CMD_ADCS_MAGNETORQUER_FORWARD]     ={1,     0       },
		[CMD_ADCS_MAGNETORQUER_REVERSE]     ={1,     0       },

		[CMD_PWR_RESET]                     ={1,     0       },
		[CMD_PWR_PAYLOAD_ON]                ={1,     0       },
		[CMD_PWR_PAYLOAD_OFF]               ={1,     0       },
		[CMD_PWR_ADCS_ON]                   ={1,     0       },
		[CMD_PWR_ADCS_OFF]                  ={1,     0       },
		[CMD_PWR_BATTERY_ACCESS_ON]         ={1,     0       },
		[CMD_PWR_BATTERY_ACCESS_OFF]        ={1,     0       },
		[CMD_PWR_BATTERY_HEATER_ON]         ={1,     0       },
		[CMD_PWR_BATTERY_HEATER_OFF]        ={1,     0       },
		[CMD_PWR_GET_CONVERTER_STATUS]      ={1,     0       },
};

#endif /* CAN_WRAPPER_MODULE_INC_CAN_COMMAND_LIST_H_ */
