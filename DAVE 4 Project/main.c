#include <DAVE.h>

#include "FREERTOS/FreeRTOS.h"
#include "FREERTOS/task.h"

#include "UserTasks.h"

int main(void){

  if(DAVE_Init() != DAVE_STATUS_SUCCESS){
    while(1);
  }

  startTasks();

  vTaskStartScheduler();

  while(1);
}
