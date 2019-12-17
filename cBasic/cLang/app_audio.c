#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf51_bitfields.h"
#include "softdevice_handler.h"
#include "battery.h"
#include "app_util.h"
#include "boards.h"
#include "ble.h"
#include "ble_srv_common.h"

#define ADC_PRE_SCALING_COMPENSATION         4                                         /**< The ADC is configured to use VDD with 1/3 prescaling as input. And hence the result of conversion is to be multiplied by 3 to get the actual value of the battery voltage.*/
#define DIODE_FWD_VOLT_DROP_MILLIVOLTS       270                                       /**< Typical forward voltage drop of the diode (Part no: SD103ATW-7-F) that is connected in series with the voltage supply. This is the voltage drop when the forward current is 1mA. Source: Data sheet of 'SURFACE MOUNT SCHOTTKY BARRIER DIODE ARRAY' available at www.diodes.com. */

//ble_bcs_t                                    bcs; 
#define max_batt 	4 
/*
static uint8_t check_sum(uint8_t *data,uint16_t lenth)
{
	uint8_t checksum = 0;
	uint16_t i = 0;
	for(i = 0;i < lenth;i ++)
	{
		checksum ^= data[i];
	}
	return checksum;
}

uint32_t ble_battery_send(ble_bcs_t * p_bcs)
{
	uint8_t   BCS[3];
	uint8_t   batt[max_batt];
	uint32_t 					err_code;
    uint16_t  					hvx_len;
	ble_gatts_hvx_params_t 		hvx_params;	
	hvx_len	=	4;

	BCS[0]	=	0x0b;
	BCS[1]	=	0x01;
	BCS[2]	=	battery;
	batt[0]	=	BCS[0];
	batt[1]	=	BCS[1];
	batt[2]	=	BCS[2];
	batt[3]	=	check_sum(BCS, 3);

	if (p_bcs->conn_handle != BLE_CONN_HANDLE_INVALID)
	{
		memset(&hvx_params, 0, sizeof(hvx_params));
	
    	hvx_params.handle = p_bcs->battery_handles.value_handle;
    	hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    	hvx_params.offset = 0;
    	hvx_params.p_len  = &hvx_len;
    	hvx_params.p_data = batt;		

		err_code = sd_ble_gatts_hvx(p_bcs->conn_handle, &hvx_params);
	}
	
	else
	{
		err_code = NRF_ERROR_INVALID_STATE;
	}

	return err_code;
	
}

static void on_bcs_cccd_write(ble_bcs_t * p_bcs, ble_gatts_evt_write_t * p_evt_write)
{
    if (p_evt_write->len == 2)
    {
        // CCCD written, update notification state
        if (p_bcs->evt_handler != NULL)
        {
            ble_bcs_evt_t evt;

            if (ble_srv_is_notification_enabled(p_evt_write->data))
            {
                evt.evt_type = BLE_BCS_EVT_NOTIFICATION_ENABLED;
            }
            else
            {
                evt.evt_type = BLE_BCS_EVT_NOTIFICATION_DISABLED;
            }

            p_bcs->evt_handler(p_bcs, &evt);
        }
    }
}

static void on_write(ble_bcs_t * p_bcs, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (p_evt_write->handle == p_bcs->battery_handles.cccd_handle)
    {
        on_bcs_cccd_write(p_bcs, p_evt_write);
    }
}

void ble_bcs_on_ble_evt(ble_bcs_t * p_bcs, ble_evt_t * p_ble_evt)
{
	switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            p_bcs->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;

        case BLE_GAP_EVT_DISCONNECTED:
			UNUSED_PARAMETER(p_ble_evt);
			p_bcs->conn_handle = BLE_CONN_HANDLE_INVALID;
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_bcs, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }	
}

static uint32_t batt_char_add(ble_bcs_t * p_bcs, const ble_bcs_init_t * p_bcs_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t             batt[max_batt]={0x00};
    //uint8_t             encoded_report_ref[BLE_SRV_ENCODED_REPORT_REF_LEN];
    //uint8_t             init_len;

	
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    cccd_md.write_perm = p_bcs_init->battery_char_attr_md.cccd_write_perm;
    cccd_md.vloc       = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_BATT);

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_bcs_init->battery_char_attr_md.read_perm;
    attr_md.write_perm = p_bcs_init->battery_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;


    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = max_batt;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = max_batt;
    attr_char_value.p_value   = batt;

    err_code = sd_ble_gatts_characteristic_add(p_bcs->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_bcs->battery_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }


    return NRF_SUCCESS;
}

uint32_t ble_bcs_init(ble_bcs_t * p_bcs, const ble_bcs_init_t * p_bcs_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_bcs->evt_handler               = p_bcs_init->evt_handler;
    p_bcs->conn_handle               = BLE_CONN_HANDLE_INVALID;

    // 添加自定义服务
    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_BATT);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_bcs->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // 添加特征值
    return batt_char_add(p_bcs, p_bcs_init);

}

*/

void ADC_battery_init(void)
{
	//uint32_t err_code;
    // Configure ADC
    NRF_ADC->INTENSET   = ADC_INTENSET_END_Msk;
	
    NRF_ADC->CONFIG     = (ADC_CONFIG_RES_8bit						   << ADC_CONFIG_RES_Pos)     |
                          (ADC_CONFIG_INPSEL_AnalogInputNoPrescaling   << ADC_CONFIG_INPSEL_Pos)  |
                          (ADC_CONFIG_REFSEL_VBG                       << ADC_CONFIG_REFSEL_Pos)  |
                          (ADC_CONFIG_PSEL_AnalogInput3				   << ADC_CONFIG_PSEL_Pos)    |
                          (ADC_CONFIG_EXTREFSEL_None                   << ADC_CONFIG_EXTREFSEL_Pos);
	
    NRF_ADC->EVENTS_END = 0;
    
    // Enable ADC interrupt
    NRF_ADC->INTENSET   = ADC_INTENSET_END_Msk;
    NVIC_SetPriority(ADC_IRQn,3);
    NVIC_EnableIRQ(ADC_IRQn);
}

void battery_start(void)
{

	NRF_ADC->ENABLE     = ADC_ENABLE_ENABLE_Enabled;	
    NRF_ADC->EVENTS_END  = 0;    // Stop any running conversions.   
    NRF_ADC->TASKS_START = 1;
}

/**
 * @}
 */
