/**
  ******************************************************************************
  * @file    morse_laser/main.c 
  * @author  Angus Lohrisch
  * @date    08-02-2018
  * @brief   main.c file for my bluetooth morse cose laser project
  *          Bluetooth implemented after functionality of a pc to controller 
  *          interface is completed.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "init_hardware.h"
#include "debug.h"

#include "hal_laser.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CAST_TASK       (void (*)(void*))
#define LED_PRIORITY    (tskIDLE_PRIORITY + 1)
#define LED_SIZE        (configMINIMAL_STACK_SIZE * 2)
/* Private variables ---------------------------------------------------------*/
TaskHandle_t led_task_h;
/* Private function prototypes -----------------------------------------------*/
void hardware_init(void);
void led_task(void);
extern int get_tick(void);

/** 
  * @brief Main program
  * @param None
  * @retval None
  */
int main(void) {

    hardware_init();

    // Create main.c tasks
    xTaskCreate(CAST_TASK led_task, (const char*) "LED", 
            LED_SIZE, NULL, LED_PRIORITY, &led_task_h);

    vTaskStartScheduler();

    ssprintf("Start failed!\n\r"); // If we reach here there was error in RTOS
    for (;;) {

    }
    return 0;
}

/** 
  * @brief Initialises the hardware and RTOS components
  * @param None
  * @retval None
  */
void hardware_init(void) {
    portDISABLE_INTERRUPTS();

    init_hardware();
    init_debug();

    hal_laser_init();

    portENABLE_INTERRUPTS();
}

/** 
  * @brief Cyclic executive task for the board LED
  * @param None
  * @retval None
  */
void led_task(void) {
    BSP_LED_Off(LED);

    for (;;) {
        BSP_LED_Toggle(LED);
        vTaskDelay(1000); // Delay for 1 second
    }
}

/** 
  * @brief External wrapper function to retrieve the tick count in ms
  * @param None
  * @retval Tick count in milliseconds
  */
extern int get_tick(void) {
    return xTaskGetTickCount();
}

/**
  * @brief  vApplicationStackOverflowHook
  * @param  Task Handler and Task Name
  * @retval None
  */
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
    /* This function will get called if a task overflows its stack.   If the
    parameters are corrupt then inspect pxCurrentTCB to find which was the
    offending task. */

    BSP_LED_Off(LED);
    (void) pxTask;
    (void) pcTaskName;

    for (;;);
}