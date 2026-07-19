#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"

/* Global define */
#define TASK1_TASK_PRIO     1
#define TASK1_STK_SIZE      512
#define TASK2_TASK_PRIO     2
#define TASK2_STK_SIZE      512

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;

/*********************************************************************
 * @fn      task1_task
 *
 * @brief   task1 program.
 *
 * @param  *pvParameters - Parameters point of task1
 *
 * @return  none
 */
void task1_task(void *pvParameters)
{
    while(1)
    {
        printf("Task1 Running!\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*********************************************************************
 * @fn      task2_task
 *
 * @brief   task2 program.
 *
 * @param  *pvParameters - Parameters point of task2
 *
 * @return  none
 */
void task2_task(void *pvParameters)
{
    while(1)
    {
        printf("Task2 Running!\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("FreeRTOS Kernel Version:%s\r\n", tskKERNEL_VERSION_NUMBER);

    /* Create task1 */
    xTaskCreate((TaskFunction_t)task1_task,
                (const char*)"task1",
                (uint16_t)TASK1_STK_SIZE,
                (void*)NULL,
                (UBaseType_t)TASK1_TASK_PRIO,
                (TaskHandle_t*)&Task1Task_Handler);

    /* Create task2 */
    xTaskCreate((TaskFunction_t)task2_task,
                (const char*)"task2",
                (uint16_t)TASK2_STK_SIZE,
                (void*)NULL,
                (UBaseType_t)TASK2_TASK_PRIO,
                (TaskHandle_t*)&Task2Task_Handler);

    /* Start FreeRTOS scheduler */
    vTaskStartScheduler();
    
    /* Should not reach here */
    while(1)
    {
        printf("Shouldn't run at here!!\n");
    }
}