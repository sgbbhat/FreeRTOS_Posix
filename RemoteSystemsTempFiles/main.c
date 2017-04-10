
/* System headers. */
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "partest.h"
#include "semphr.h"

/* Demo file headers. */
#include "BlockQ.h"
#include "PollQ.h"
#include "death.h"
#include "crflash.h"
#include "flop.h"
#include "print.h"
#include "fileIO.h"
#include "semtest.h"
#include "integer.h"
#include "dynamic.h"
#include "mevents.h"
#include "crhook.h"
#include "blocktim.h"
#include "GenQTest.h"
#include "QPeek.h"
#include "countsem.h"
#include "recmutex.h"

#include "AsyncIO/AsyncIO.h"
#include "AsyncIO/AsyncIOSocket.h"
#include "AsyncIO/PosixMessageQueueIPC.h"
#include "AsyncIO/AsyncIOSerial.h"

/* Priority definitions for the tasks in the demo application. */
#define mainLED_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainCREATOR_TASK_PRIORITY	( tskIDLE_PRIORITY + 3 )
#define mainPRINT_TASK_PRIORITY		( tskIDLE_PRIORITY + 4 )
#define mainQUEUE_POLL_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_BLOCK_PRIORITY	( tskIDLE_PRIORITY + 3 )
#define mainCOM_TEST_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainSEMAPHORE_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
#define mainGENERIC_QUEUE_PRIORITY	( tskIDLE_PRIORITY )

#define mainDEBUG_LOG_BUFFER_SIZE	( ( unsigned short ) 20480 )

/* The number of flash co-routines to create. */
#define mainNUM_FLASH_CO_ROUTINES	( 8 )

/* The create delete Tasks routines are very expensive so they are
 * disabled unless required for testing. */
#define mainUSE_SUICIDAL_TASKS_DEMO		1

/* UDP Packet size to send/receive. */
#define mainUDP_SEND_ADDRESS		"127.0.0.1"
#define mainUDP_PORT				( 9999 )

/* Remove some of the CPU intensive tasks. */
#define mainCPU_INTENSIVE_TASKS		0

/* Just used to count the number of times the example task callback function is
called, and the number of times a queue send passes. */
static unsigned long uxQueueSendPassedCount = 0;
/*-----------------------------------------------------------*/

#include "FreeRTOSConfig.h"

unsigned long ulIdleCycleCount = 0UL;


/*-----------------------------------------------------------*/

void vMainQueueSendPassed( void )
{
	/* This is just an example implementation of the "queue send" trace hook. */
	uxQueueSendPassedCount++;
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{

}
/*-----------------------------------------------------------*/

void vTaskFunctionContinues1(void *p)
{
	int j = 0;
	for(;;)
	{
		printf("Task 1 : %d\n", ++j);
		j++;
		vTaskDelay(3000);
	}
}

void vTaskFunctionContinues2(void *p)
{
	int i = 0;
	for(;;)
	{
		printf("Task 2 : %d\n", i);
		i++;
		vTaskDelay(1000);
	}
}

void vTaskFunctionPeriodic(void *p)
{
	int k=0;
	for(;;)
	{
		printf("Task 3 : %d\n", k);
		k++;
		vTaskDelay(5000);
	}
}

int main( void )
{
	xTaskCreate(vTaskFunctionContinues1, (unsigned char *) "Function1", 1024, NULL, 1, NULL);
	xTaskCreate(vTaskFunctionContinues2, (unsigned char *) "Function2", 1024, NULL, 1, NULL);
	xTaskCreate(vTaskFunctionPeriodic, (unsigned char *) "Function3", 1024, NULL, 2, NULL);
	vTaskStartScheduler();
	return 1;
}
