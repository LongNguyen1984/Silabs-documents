/*
 * air_quality_app.c
 *
 *  Created on: Jun 15, 2023
 *      Author: nmhoa
 */

#include <stdio.h>
#include <string.h>

#include "sl_ccs811.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "gatt_db.h"
#include "app_assert.h"
#include "app_log.h"
#include "air_quality_app.h"

//CO2 result from CCS811
static uint16_t co2;

// tVOC result from CCS811
static uint16_t tvoc;

// Periodic timer handle.
//static sl_sleeptimer_timer_handle_t air_quality_monitor_periodic_timer;

sl_status_t air_quality_app_init(void){
  sl_status_t status;
  // Initialize sensor and set measure mode
    status = sl_ccs811_init(sl_i2cspm_sensor_gas);
    if (status != SL_STATUS_OK) {
      return status;
    }
    status = sl_ccs811_set_measure_mode(sl_i2cspm_sensor_gas,
                                        CCS811_MEASURE_MODE_DRIVE_MODE_1SEC);
    if (status != SL_STATUS_OK) {
      return status;
    }
    return status;
}
sl_status_t air_quality_user_read_callback(sl_bt_msg_t *evt)
{
  sl_status_t sc;
  uint16_t characteristic_size = 0;
  const uint8_t *characteristic_ptr = NULL;
  uint16_t sent_len;

  //read gas sensor
  air_quality_monitor_event_handler();

  switch (evt->data.evt_gatt_server_user_read_request.characteristic) {

    // co2 characteristics value read

    case gattdb_eCO2:
      characteristic_size = sizeof(co2);
      characteristic_ptr = (const uint8_t*) &co2;
      break;

    // tvoc characteristics value read
    case gattdb_eTVCO :
      characteristic_size = sizeof(tvoc);
      characteristic_ptr = (const uint8_t*) &tvoc;
      break;

    // Measurement period data characteristics value read
    //case gattdb_measurement_period_data:
    //  characteristic_size = sizeof(air_quality_data.measurement_period_data);
    //  characteristic_ptr =
    //    (const uint8_t*) &air_quality_data.measurement_period_data;
    //  break;

    // Do nothing
    default:
      break;
  }

    // Send response
  sc = sl_bt_gatt_server_send_user_read_response(
    evt->data.evt_gatt_server_user_read_request.connection,
    evt->data.evt_gatt_server_user_read_request.characteristic,
    (uint8_t) 0x00, // SUCCESS
    characteristic_size, characteristic_ptr, &sent_len);
  return sc;
}
void air_quality_monitor_event_handler(void)
{
  sl_status_t retval;
  if (sl_ccs811_is_data_available(sl_i2cspm_sensor_gas)) {
      // Get measurement data from the CCS811
      retval = sl_ccs811_get_measurement(sl_i2cspm_sensor_gas, &co2, &tvoc);

      //Report to USART
      if (retval==SL_STATUS_OK)
        printf("eco2: %4d, tvoc: %4d \r\n", co2, tvoc);
   }
}
