#include "os.h"

void task1(void) {
  int i = 0;
  for(;;){
    i++;
    //#if with_scheduler == 1
    delay(10);
    /*#else
    yield();
    #endif*/
  }
}

void task2(void) {
  int i = 0;
  volatile float a = 0.01;
  for(;;){
    i++;
    a = a + 0.05;
    //#if with_scheduler == 1
    delay(20);
    /*#else
    yield();
    #endif*/
  }
}

//sem_t semaf;
void task3(void) {
  int i = 0;
  volatile float a = 0.01;
  //(void)sem_init(&semaf);
  for(;;){
    i++;
    a += 0.01;
    //#if with_scheduler == 1
    delay(30);
    //(void)sem_post(&semaf);
   // #else
    //yield();
    //#endif
  }
}


void task4(void) {
  int i = 0;
  for(;;){
    i++;
    delay(5);
      //(void)sem_pend(&semaf, 300);
  }
}
