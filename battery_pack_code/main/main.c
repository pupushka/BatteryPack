/* Hello World Example
 
 This example code is in the Public Domain (or CC0 licensed, at your option.)
 
 Unless required by applicable law or agreed to in writing, this
 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <driver/adc.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_wifi.h"
#include "xi2c.h"
#include "fonts.h"
#include "ssd1306.h"
#include "nvs_flash.h"

#include "driver/ledc.h"
#include "esp_err.h"
#include <driver/adc.h>

#include <soc/rtc.h>
#include <esp32/rom/ets_sys.h>

#include "i2c_manual.h"
#include "traffic_light.h"

#include "mcp3425a1.h"

#include <esp_task_wdt.h>
#include <math.h>
//#include <c++/9/bits/ios_base.h>

#include "init_ESP.h"

static void comutation (int sw);



xSemaphoreHandle print_mux;

void
app_main (void)
{

  float delitel_v_in = 3.37; //za whoda
  float delitel_v_bat = 10.7;
  float volt_res[8];
  char print_in_volt[19];
  float res_err_v_in = 1.0031727; //za whoda
  float res_err_v_bat = 0.943625466;
  float delitel_v_out = 3.37;
  float res_err_v_out = 1.001376;
  int count = 0;
  float percent_bat_volt = 0;

  initSW ();

  print_mux = xSemaphoreCreateMutex ();
  comutation (0);
  displayInit ();
  SSD1306_Init ();

  while (1)
    {
      for (int i = 0; i < 8; i++)
        {
          comutation (i);
          vTaskDelay (1 / portTICK_PERIOD_MS);
          for (int mm = 0; mm < 15; mm++)
            {
              if ((i == 2) || (i == 3) || (i == 5))
                {
                  volt_res[i] = meassureMCP (SPS_15, PGA_8);
                }
              else if (i == 7)
                {
                  volt_res[i] = meassureMCP (SPS_15, PGA_4);
                }
              else
                {
                  volt_res[i] = meassureMCP (SPS_15, PGA_1);
                }
            }
          vTaskDelay (50 / portTICK_PERIOD_MS);
        }

      count++;

      if (count > 8)
        {
          count = 0;
        }

      volt_res[1] = volt_res[1] * delitel_v_in * res_err_v_in;
      volt_res[2] = (volt_res[2]* 1000) / 83.529411765;
      volt_res[3] = (volt_res[3] * 1000 * (-1)) / 66.1973584;  ////az    Dilqna-69.23;
      volt_res[4] = volt_res[4] * delitel_v_bat * res_err_v_bat - volt_res[7];
      volt_res[5] = (volt_res[5]* 1000) / 46.43781036; ////az     Dilqna-57.894737;
      volt_res[6] = volt_res[6] * delitel_v_out * res_err_v_out - volt_res[7];
      volt_res [7] = volt_res[7] * 1.01921633;///moeto        //0.969827586;----- Dilqna

      percent_bat_volt = (volt_res[4] - 9) / (11.9 - 9);
      percent_bat_volt = percent_bat_volt * 100;
      if (percent_bat_volt > 100)
        {
          percent_bat_volt = 100;
        }
      if (percent_bat_volt < 0)
        {
          percent_bat_volt = 0;
        }

      if (count >= 0 && count < 4)
        {
          sprintf (print_in_volt, "Vin=%.3fV   ", volt_res[1]);
          SSD1306_GotoXY (8, 5);
          SSD1306_Puts (print_in_volt, &Font_7x10, SSD1306_COLOR_WHITE);


          sprintf (print_in_volt, "Iin=%.3fA    ", volt_res[2]);
          SSD1306_GotoXY (8, 20);
          SSD1306_Puts (print_in_volt, &Font_7x10, SSD1306_COLOR_WHITE);

          sprintf (print_in_volt, "I Bat=%.3fA    ", volt_res[3]);
          SSD1306_GotoXY (8, 35);
          SSD1306_Puts (print_in_volt, &Font_7x10, SSD1306_COLOR_WHITE);

          sprintf (print_in_volt, "V Bat=%.3fV    ", volt_res[4]);
          SSD1306_GotoXY (8, 50);
          SSD1306_Puts (print_in_volt, &Font_7x10, SSD1306_COLOR_WHITE);

        }
      if (count >= 4 && count < 8)
        {
          sprintf (print_in_volt, "perc=%.3f%%    ", percent_bat_volt);
          SSD1306_GotoXY (8, 5);
          SSD1306_Puts (print_in_volt, &Font_7x10, SSD1306_COLOR_WHITE);


          sprintf (print_in_volt, "I out=%.3fA    ", volt_res[5]);
          SSD1306_GotoXY (8, 20);
          SSD1306_Puts (print_in_volt, &Font_7x10, SSD1306_COLOR_WHITE);

          sprintf (print_in_volt, "V Out=%.3fV    ", volt_res[6]);
          SSD1306_GotoXY (8, 35);
          SSD1306_Puts (print_in_volt, &Font_7x10, SSD1306_COLOR_WHITE);

          sprintf (print_in_volt, "I char=%.3fA    ", volt_res[7] / 1.6);
          SSD1306_GotoXY (8, 50);
          SSD1306_Puts (print_in_volt, &Font_7x10, SSD1306_COLOR_WHITE);
        }

      vTaskDelay (50 / portTICK_PERIOD_MS);
      SSD1306_UpdateScreen ();
      /* Make a little delay */
      vTaskDelay (50 / portTICK_PERIOD_MS);
    }
}


static void
comutation (int sw)
{
  switch (sw)
    {
    case 0:
      gpio_set_level (V_OUT, 1);
      gpio_set_level (OUT_GND_NEG, 1);
      gpio_set_level (D_GND, 1);
      gpio_set_level (OUT_I, 1);
      gpio_set_level (V_BAT, 1);
      gpio_set_level (BAT_I, 1);
      gpio_set_level (V_IN, 1);
      gpio_set_level (IN_I, 1);
      gpio_set_level (OUT_GND_POS, 1);
      break;

    case 1:
      gpio_set_level (V_OUT, 1);
      gpio_set_level (OUT_GND_NEG, 1);
      gpio_set_level (OUT_I, 1);
      gpio_set_level (V_BAT, 1);
      gpio_set_level (BAT_I, 1);
      gpio_set_level (IN_I, 1);
      gpio_set_level (OUT_GND_POS, 1);
      gpio_set_level (D_GND, 0);
      gpio_set_level (V_IN, 0);
      break;

    case 2:
      gpio_set_level (V_OUT, 1);
      gpio_set_level (OUT_GND_NEG, 1);
      gpio_set_level (D_GND, 1);
      gpio_set_level (OUT_I, 1);
      gpio_set_level (V_BAT, 1);
      gpio_set_level (BAT_I, 1);
      gpio_set_level (V_IN, 1);
      gpio_set_level (OUT_GND_POS, 1);
      gpio_set_level (IN_I, 0);
      break;

    case 3:
      gpio_set_level (V_OUT, 1);
      gpio_set_level (OUT_GND_NEG, 1);
      gpio_set_level (D_GND, 1);
      gpio_set_level (OUT_I, 1);
      gpio_set_level (V_BAT, 1);
      gpio_set_level (V_IN, 1);
      gpio_set_level (IN_I, 1);
      gpio_set_level (OUT_GND_POS, 1);
      gpio_set_level (BAT_I, 0);
      break;

    case 4:
      gpio_set_level (V_OUT, 1);
      gpio_set_level (OUT_GND_NEG, 1);
      gpio_set_level (OUT_I, 1);
      gpio_set_level (BAT_I, 1);
      gpio_set_level (V_IN, 1);
      gpio_set_level (IN_I, 1);
      gpio_set_level (OUT_GND_POS, 1);
      gpio_set_level (D_GND, 0);
      gpio_set_level (V_BAT, 0);
      break;

    case 5:
      gpio_set_level (V_OUT, 1);
      gpio_set_level (OUT_GND_NEG, 1);
      gpio_set_level (D_GND, 1);
      gpio_set_level (V_BAT, 1);
      gpio_set_level (BAT_I, 1);
      gpio_set_level (V_IN, 1);
      gpio_set_level (IN_I, 1);
      gpio_set_level (OUT_GND_POS, 1);
      gpio_set_level (OUT_I, 0);
      break;

    case 6:
      gpio_set_level (OUT_GND_NEG, 1);
      gpio_set_level (OUT_I, 1);
      gpio_set_level (V_BAT, 1);
      gpio_set_level (BAT_I, 1);
      gpio_set_level (V_IN, 1);
      gpio_set_level (IN_I, 1);
      gpio_set_level (OUT_GND_POS, 1);
      gpio_set_level (D_GND, 0);
      gpio_set_level (V_OUT, 0);
      break;

    case 7:
      gpio_set_level (V_OUT, 1);
      gpio_set_level (OUT_GND_NEG, 1);
      gpio_set_level (OUT_I, 1);
      gpio_set_level (V_BAT, 1);
      gpio_set_level (BAT_I, 1);
      gpio_set_level (V_IN, 1);
      gpio_set_level (IN_I, 1);
      gpio_set_level (OUT_GND_POS, 0);
      gpio_set_level (D_GND, 0);
      break;

    default:
      gpio_set_level (V_OUT, 1);
      gpio_set_level (OUT_GND_NEG, 1);
      gpio_set_level (D_GND, 1);
      gpio_set_level (OUT_I, 1);
      gpio_set_level (V_BAT, 1);
      gpio_set_level (BAT_I, 1);
      gpio_set_level (V_IN, 1);
      gpio_set_level (IN_I, 1);
      gpio_set_level (OUT_GND_POS, 1);
      break;
    }
}

