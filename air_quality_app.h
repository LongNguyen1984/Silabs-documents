/*
 * air_quality_app.h
 *
 *  Created on: Jun 15, 2023
 *      Author: nmhoa
 */

#ifndef CONFIG_AIR_QUALITY_APP_H_
#define CONFIG_AIR_QUALITY_APP_H_

#include "sl_bt_api.h"

//CO2 result from CCS811
//extern  uint16_t co2;

// tVOC result from CCS811
//extern uint16_t tvoc;
/*
typedef struct{
  uint8_t notification_data;        ///<Notification status(0:dis, 1: en)
  uint8_t measurement_period_data;  ///<Measurement update period in s (1-30)
}air_quality_data_t;
*/

sl_status_t air_quality_app_init(void);
sl_status_t air_quality_user_read_callback(sl_bt_msg_t *evt);
void air_quality_monitor_event_handler(void);
#endif /* CONFIG_AIR_QUALITY_APP_H_ */
