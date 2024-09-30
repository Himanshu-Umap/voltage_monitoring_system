#include <stdio.h>
#include "driver/adc.h"
#include "driver/adc_oneshot.h"
#include "esp_log.h"
#include "esp_err.h"

#define ADC_CHANNEL ADC1_CHANNEL_0 //ESP DEV-KIT V1
#define ADC_WIDTH ADC_WIDTH_BIT_12 // 12 BIT Resolution
#define ADC_ATTEN ADV_ATTEN_DB_11


// Tag for debugging
static const char *TAG = "Voltage_Monitor_ADC_ONE_SHOT";

void app_main(void)
{
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 ={
        .unit_id = ADC_UNIT_1,
    }
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_WIDTH,    // Set ADC width (resolution)
        .atten = ADC_ATTEN        // Set ADC attenuation
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL, &config));//??&config is out_raw
    
    while(1){
     // 3. Read ADC value in one-shot mode
        int adc_raw;
        esp_err_t result = adc_oneshot_read(adc1_handle, ADC_CHANNEL, &adc_raw);

        if (result == ESP_OK) {
            // Convert raw ADC value to voltage in millivolts
            float adc_voltage = (float)adc_raw * (3.3 / 4095.0);  // Convert to voltage in volts (3.3V reference)

            // Calculate the battery voltage using the voltage divider formula
            // Vout = Vin * (R2 / (R1 + R2))
            // Vin = Vout * ((R1 + R2) / R2)
            float battery_voltage = adc_voltage * ((R1 + R2) / R2);

            // Print the raw ADC value and the calculated battery voltage
            ESP_LOGI(TAG, "ADC Raw Value: %d, ADC Voltage: %.2f V, Battery Voltage: %.2f V", adc_raw, adc_voltage, battery_voltage);
        } else {
            ESP_LOGE(TAG, "Failed to read ADC value");
        }

        // Delay for 1 second before reading again
        vTaskDelay(pdMS_TO_TICKS(1000));  // 1000ms delay
    }
}