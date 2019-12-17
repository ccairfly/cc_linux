#ifndef BATTERY_H__
#define BATTERY_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

extern uint8_t battery;
/**@brief Battery Service event type. */
typedef enum
{
    BLE_BCS_EVT_NOTIFICATION_ENABLED,                             /**< Battery value notification enabled event. */
    BLE_BCS_EVT_NOTIFICATION_DISABLED                             /**< Battery value notification disabled event. */
} ble_bcs_evt_type_t;

/**@brief Battery Service event. */
typedef struct
{
    ble_bcs_evt_type_t evt_type;                                  /**< Type of event. */
} ble_bcs_evt_t;

// Forward declaration of the ble_bas_t type. 
typedef struct ble_bcs_s ble_bcs_t;

/**@brief Battery Service event handler type. */
typedef void (*ble_bcs_evt_handler_t) (ble_bcs_t * p_bcs, ble_bcs_evt_t * p_evt);

/**@brief Battery Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    ble_bcs_evt_handler_t         evt_handler;                    /**< Event handler to be called for handling events in the Battery Service. */
    //bool                          support_notification;           /**< TRUE if notification of Battery Level measurement is supported. */
    ble_srv_report_ref_t *        p_report_ref;                   /**< If not NULL, a Report Reference descriptor with the specified value will be added to the Battery Level characteristic */
    uint8_t                       initial_battery;             /**< Initial battery level */
    ble_srv_cccd_security_mode_t  battery_char_attr_md;     /**< Initial security level for battery characteristics attribute */
    ble_gap_conn_sec_mode_t       battery_report_read_perm; /**< Initial security level for battery report read attribute */
} ble_bcs_init_t;

/**@brief Battery Service structure. This contains various status information for the service. */
struct ble_bcs_s
{
    ble_bcs_evt_handler_t         evt_handler;                    /**< Event handler to be called for handling events in the Battery Service. */
    uint16_t                      service_handle;                 /**< Handle of Battery Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t      battery_handles;          /**< Handles related to the Battery Level characteristic. */
    uint16_t                      report_ref_handle;              /**< Handle of the Report Reference descriptor. */
    uint8_t                       battery_level_last;             /**< Last Battery Level measurement passed to the Battery Service. */
    uint16_t                      conn_handle;                    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    //bool                          is_notification_supported;      /**< TRUE if notification of Battery Level is supported. */
};

/**@brief Function for making the ADC start a battery level conversion.
 */
void battery_start(void);
void ADC_battery_init(void);
//uint32_t ble_bcs_init(ble_bcs_t * p_bcs, const ble_bcs_init_t * p_bcs_init);

#endif // BATTERY_H__

/** @} */
/** @endcond */
