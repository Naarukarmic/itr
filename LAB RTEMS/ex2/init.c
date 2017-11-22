/* Standard C header files */
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/* RTEMS header files */
#include <rtems.h>
#include <bsp.h>
#include <rtems/bspIo.h>
#include <rtems/cpuuse.h> 
 
/* Functions */
rtems_task Init(rtems_task_argument argument);
rtems_task Task_1(rtems_task_argument argument);
rtems_task Task_2(rtems_task_argument argument);
rtems_task Task_3(rtems_task_argument argument);

/****************************************************************************/
/* RTEMS resources configuration                                            */
/****************************************************************************/

#define MAX_TASKS 3
#define MAX_PERIODS 100 

/* RTEMS system configuration informations */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS              9
#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_MAXIMUM_PERIODS            10000
#define CONFIGURE_MICROSECONDS_PER_TICK      100 
#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/****************************************************************************/
/* The following is used to waste some CPU time, roughly 100 ms with
 * tsim-leon3.  
 * Note: this requires optimization level of -O0 wih gcc
 */
#define TEMPO 0xffffff
#define WASTE_CPU for (i =0 ; i < TEMPO; i++)

/******************************************************************************/
/* Global variables                                                           */
/******************************************************************************/

rtems_name          Task_name[9]      = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
rtems_task_priority Task_Priority[9]  = { 0, 21, 22, 23, 0, 0, 0, 0, 0 };
rtems_id            Task_id[9]        = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
rtems_name          Semaphore_name[2] = { 0, 0 };
rtems_id            Semaphore_id[2]   = { 0, 0 };

/******************************************************************************/
/* Task #1 (high priority)                                                    */
/******************************************************************************/

rtems_task Task_1( rtems_task_argument argument) {\
  int i;
  rtems_interval offset = 500;

  rtems_task_wake_after(offset);
  rtems_semaphore_obtain(Semaphore_id[1], 0, 0);
  WASTE_CPU;
  rtems_semaphore_release(Semaphore_id[1]);
  WASTE_CPU;

  rtems_task_delete(RTEMS_SELF);
}

/******************************************************************************/
/* Task #2 (medium priority)                                                  */
/******************************************************************************/

rtems_task Task_2( rtems_task_argument argument) {
}

/******************************************************************************/
/* Task #3 (low priority)                                                     */
/******************************************************************************/

rtems_task Task_3( rtems_task_argument argument) {
}

/****************************************************************************/
/* Init(): main entrypoint                                                  */
/****************************************************************************/

rtems_task Init( rtems_task_argument argument)
{
  rtems_status_code status;
  rtems_task_priority the_priority;
  rtems_id id;
  rtems_time_of_day time;
  int j;
  char c_name = '0';
  
  status=rtems_task_wake_after(1000);  /* wait about 100 ms */
  
  rtems_cpu_usage_reset(); 
  rtems_cpu_usage_report();  

  /* get clock value */
  status = rtems_clock_get( RTEMS_CLOCK_GET_TOD, &time );    

  /* Set current task priority to 10 */
  status = rtems_task_set_priority(RTEMS_SELF, 10, &the_priority);
  status = rtems_task_set_priority
    (RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &the_priority);
  printf("actual current priority:%d\n", (int)the_priority);
  
  /* Create semaphore SM1 */
  Semaphore_name[ 1 ] = rtems_build_name( 'S', 'M', '1', ' ' );
  status = rtems_semaphore_create(Semaphore_name[1],
				  1,
				  RTEMS_PRIORITY
				  | RTEMS_BINARY_SEMAPHORE
				  | RTEMS_GLOBAL,
				  Task_Priority[1],
				  &id);
  Semaphore_id[1] = id;
  printf("main-- Semaphore 1 created, status = %d, priority = %d, id = %x\n",
	 (int)status, (int)Task_Priority[1], (int)Semaphore_id[1]);

  
  /* Tasks initialization */
  for (j = 0; j < MAX_TASKS; j++){ 
    Task_name[j+1] = rtems_build_name('T','_', 'A', c_name + j + 1);
    status = rtems_task_create(Task_name[j+1], 
			       Task_Priority[j+1],
			       RTEMS_MINIMUM_STACK_SIZE * 2,
			       RTEMS_DEFAULT_MODES,
			       RTEMS_DEFAULT_ATTRIBUTES,
			       &id);
    Task_id[j+1] = id;
    printf("main-- Task nb. %d created, status = %d, priority = %d, id = %x\n",
	   j+1, (int)status, (int)Task_Priority[j+1], (int)Task_id[j+1]); 
  } 

  /* Tasks launching */
  status = rtems_task_start(Task_id[1], (rtems_task_entry)Task_1, 1);
  status = rtems_task_start(Task_id[2], (rtems_task_entry)Task_2, 2); 
  status = rtems_task_start(Task_id[3], (rtems_task_entry)Task_3, 3);

  /* Lower main task  priority so others tasks begin running  */
  status = rtems_task_set_priority(RTEMS_SELF, 40, &the_priority);
  printf("main -- current priority : %d, is set to (40) : \n", 
	 (int)the_priority);
  
  /* This will print when other tasks are finished */
  status = rtems_task_set_priority
    (RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &the_priority);
  printf("main -- current priority : %d\n", (int)the_priority);
  
  printf("main (init) : exit\n");
  rtems_task_delete(RTEMS_SELF);
}   /* Init end */


