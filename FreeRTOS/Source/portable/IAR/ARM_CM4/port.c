/*
    FreeRTOS V7.0.0 - Copyright (C) 2011 Real Time Engineers Ltd.


    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/*
    Change from V4.2.1:

    + Introduced usage of configKERNEL_INTERRUPT_PRIORITY macro to set the
      interrupt priority used by the kernel.
*/

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the ARM CM3 port.
 *----------------------------------------------------------*/
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* Constants required to manipulate the NVIC. */
#define portNVIC_SYSTICK_CTRL       ( ( volatile unsigned long *) 0xe000e010 )
#define portNVIC_SYSTICK_LOAD       ( ( volatile unsigned long *) 0xe000e014 )
#define portNVIC_INT_CTRL           ( ( volatile unsigned long *) 0xe000ed04 )
#define portNVIC_SYSPRI2            ( ( volatile unsigned long *) 0xe000ed20 )
#define portNVIC_SYSTICK_CLK        0x00000004
#define portNVIC_SYSTICK_INT        0x00000002
#define portNVIC_SYSTICK_ENABLE     0x00000001
#define portNVIC_PENDSVSET          0x10000000
#define portNVIC_PENDSV_PRI         ( ( ( unsigned long ) configKERNEL_INTERRUPT_PRIORITY ) << 16 )
#define portNVIC_SYSTICK_PRI        ( ( ( unsigned long ) configKERNEL_INTERRUPT_PRIORITY ) << 24 )

/* Constants required to set up the initial stack. */
#define portINITIAL_XPSR            ( 0x01000000 )

/* For backward compatibility, ensure configKERNEL_INTERRUPT_PRIORITY is
defined.  The value 255 should also ensure backward compatibility.
FreeRTOS.org versions prior to V4.3.0 did not include this definition. */
#ifndef configKERNEL_INTERRUPT_PRIORITY
    #define configKERNEL_INTERRUPT_PRIORITY 0
#endif

/* Each task maintains its own interrupt status in the critical nesting
variable. */
static unsigned portBASE_TYPE uxCriticalNesting = 0xaaaaaaaa;

/*
 * Setup the timer to generate the tick interrupts.
 */
static void prvSetupTimerInterrupt( void );

/*
 * Setup the MPU for areas common to all tasks.
 */
static void prvSetupMpu( void );

/*
 * Exception handlers.
 */
void xPortSysTickHandler( void );

/*
 * Start first task is a separate function so it can be tested in isolation.
 */
extern void vPortStartFirstTask( void );

/*
 * Raise a tasks privileges to supervisor level
*/
static portBASE_TYPE prvRaisePrivilege( void );

/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */


#if( portUSING_MPU_WRAPPERS == 1 )

#define portINITIAL_CONTROL_IF_UNPRIVILEGED     ( 0x03 )
#define portINITIAL_CONTROL_IF_PRIVILEGED       ( 0x02 )


portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters, portBASE_TYPE xRunPrivileged )
{
    /* Simulate the stack frame as it would be created by a context switch
    interrupt. */
    pxTopOfStack--; /* Offset added to account for the way the MCU uses the stack on entry/exit of interrupts. */
    *pxTopOfStack = portINITIAL_XPSR;   /* xPSR */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) pxCode;  /* PC */
    pxTopOfStack--;
    *pxTopOfStack = 0;  /* LR */
    pxTopOfStack -= 5;  /* R12, R3, R2 and R1. */
    *pxTopOfStack = ( portSTACK_TYPE ) pvParameters;    /* R0 */
    pxTopOfStack -= 9;  /* R11, R10, R9, R8, R7, R6, R5 and R4. & execution state */

    if( xRunPrivileged == pdTRUE )
    {
        *pxTopOfStack = portINITIAL_CONTROL_IF_PRIVILEGED;
    }
    else
    {
        *pxTopOfStack = portINITIAL_CONTROL_IF_UNPRIVILEGED;
    }

    return pxTopOfStack;
}

#else

portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
    /* Simulate the stack frame as it would be created by a context switch
    interrupt. */
    pxTopOfStack--; /* Offset added to account for the way the MCU uses the stack on entry/exit of interrupts. */
    *pxTopOfStack = portINITIAL_XPSR;   /* xPSR */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) pxCode;  /* PC */
    pxTopOfStack--;
    *pxTopOfStack = 0;  /* LR */
    pxTopOfStack -= 5;  /* R12, R3, R2 and R1. */
    *pxTopOfStack = ( portSTACK_TYPE ) pvParameters;    /* R0 */
    pxTopOfStack -= 8;  /* R11, R10, R9, R8, R7, R6, R5 and R4. */

    return pxTopOfStack;
}

#endif /* portUSING_MPU_WRAPPERS == 1 */
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
portBASE_TYPE xPortStartScheduler( void )
{
    /* Make PendSV and SysTick the lowest priority interrupts. */
    *(portNVIC_SYSPRI2) |= portNVIC_PENDSV_PRI;
    *(portNVIC_SYSPRI2) |= portNVIC_SYSTICK_PRI;

    /* Configure the regions in the MPU that are common to all tasks. */
    prvSetupMpu();

    /* Start the timer that generates the tick ISR.  Interrupts are disabled
    here already. */
    prvSetupTimerInterrupt();

    /* Initialise the critical nesting count ready for the first task. */
    uxCriticalNesting = 0;

    /* Start the first task. */
    vPortStartFirstTask();

    /* Should not get here! */
    return 0;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
    /* It is unlikely that the CM3 port will require this function as there
    is nothing to return to.  */
}
/*-----------------------------------------------------------*/

void vPortYieldFromISR( void )
{
    /* Set a PendSV to request a context switch. */
    *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET;
}
/*-----------------------------------------------------------*/

void vPortEnterCritical( void )
{
    portDISABLE_INTERRUPTS();
    uxCriticalNesting++;
}
/*-----------------------------------------------------------*/

void vPortExitCritical( void )
{
    uxCriticalNesting--;
    if( uxCriticalNesting == 0 )
    {
        portENABLE_INTERRUPTS();
    }
}
/*-----------------------------------------------------------*/

void xPortSysTickHandler( void )
{
unsigned long ulDummy;

    /* If using preemption, also force a context switch. */
    #if configUSE_PREEMPTION == 1
        *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET;
    #endif

    ulDummy = portSET_INTERRUPT_MASK_FROM_ISR();
    {
        vTaskIncrementTick();
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( ulDummy );
}
/*-----------------------------------------------------------*/

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
void prvSetupTimerInterrupt( void )
{
    /* Configure SysTick to interrupt at the requested rate. */
    *(portNVIC_SYSTICK_LOAD) = ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
    *(portNVIC_SYSTICK_CTRL) = portNVIC_SYSTICK_CLK | portNVIC_SYSTICK_INT | portNVIC_SYSTICK_ENABLE;
}
/*-----------------------------------------------------------*/

#include "MK10X256VMD100.h"
#include "mpu.h"

#pragma section =  "__VECTOR_RAM"
#pragma section = "HEAP"
#pragma section = "CodeRelocateRam"
extern unsigned portLONG __VECTOR_RAM_SIZE[];

PRIVILEGED_FUNCTION
static void prvSetupMpu()
{
    MPU_MemMapPtr pMpu = MPU_BASE_PTR;

    pMpu->CESR = 0UL;
    /* MPU Configuration */
    pMpu->WORD[portUNUSED_REGION][2] = 0UL;// Disable RGD0
    // Set RGD1 all flash privileged read/execute region memory
    pMpu->WORD[portPRIVILEGED_FLASH_REGION][0] = FLASH_START;// Start address
    pMpu->WORD[portPRIVILEGED_FLASH_REGION][1] = (FLASH_START + FLASH_SIZE);// End Address
    pMpu->WORD[portPRIVILEGED_FLASH_REGION][2] = portMPU_REGION_PRIVILEGED_READ_ONLY;
    pMpu->WORD[portPRIVILEGED_FLASH_REGION][3] = 0x00000001;// region is valid

    // Set RGD2 flash unpriviliged read/execute region memory
    pMpu->WORD[portUNPRIVILEGED_FLASH_REGION][0] = (unsigned portLONG)__section_end("privileged_functions") + 1;// Start address
    pMpu->WORD[portUNPRIVILEGED_FLASH_REGION][1] = (FLASH_START + FLASH_SIZE);// End Address
    pMpu->WORD[portUNPRIVILEGED_FLASH_REGION][2] = portMPU_REGION_READ_ONLY;
    pMpu->WORD[portUNPRIVILEGED_FLASH_REGION][3] = 0x00000001;// region is valid

    // Set RGD2 Internal SRAM
    pMpu->WORD[portPRIVILEGED_RAM_REGION][0] = SRAM_START;
    pMpu->WORD[portPRIVILEGED_RAM_REGION][1] = (SRAM_START + SRAM_SIZE);// End Address
    pMpu->WORD[portPRIVILEGED_RAM_REGION][2] = portMPU_REGION_PRIVILEGED_READ_WRITE;
    pMpu->WORD[portPRIVILEGED_RAM_REGION][3] = 0x00000001;// region is valid

    pMpu->WORD[portUNPRIVILEGED_RAM_REGION][0] = (((unsigned portLONG)__section_end("privileged_data")) + 1);// Start address
    pMpu->WORD[portUNPRIVILEGED_RAM_REGION][1] = (((unsigned portLONG)__section_end("HEAP")));// End Address
    pMpu->WORD[portUNPRIVILEGED_RAM_REGION][2] = portMPU_REGION_READ_WRITE;
    pMpu->WORD[portUNPRIVILEGED_RAM_REGION][3] = 0x00000001;// region is valid

    pMpu->WORD[portVECTOR_RAM_REGION][0] = (unsigned portLONG)__section_begin("__VECTOR_RAM");
    pMpu->WORD[portVECTOR_RAM_REGION][1] = ((unsigned portLONG)__section_begin("__VECTOR_RAM") + (unsigned portLONG)__VECTOR_RAM_SIZE) - 32;// End Address
    pMpu->WORD[portVECTOR_RAM_REGION][2] = portMPU_REGION_READ_WRITE_EX;
    pMpu->WORD[portVECTOR_RAM_REGION][3] = 0x00000001;// region is valid        S1(SPVR_RD_EX)


    pMpu->WORD[portRAMFUNC_REGION][0] = (unsigned portLONG)__section_begin("CodeRelocateRam");
    pMpu->WORD[portRAMFUNC_REGION][1] = (unsigned portLONG)__section_end("CodeRelocateRam");
    pMpu->WORD[portRAMFUNC_REGION][2] = portMPU_REGION_READ_WRITE_EX;
    pMpu->WORD[portRAMFUNC_REGION][3] = 0x00000001;// region is valid        S1(SPVR_RD_EX)

    // Set RGD2  Peripherals and I/O
    pMpu->WORD[portGENERAL_PERIPHERALS_REGION][0] = PERIPHERAL_START;
    pMpu->WORD[portGENERAL_PERIPHERALS_REGION][1] = (PERIPHERAL_START + PERIPHERAL_SIZE);// End Address
    pMpu->WORD[portGENERAL_PERIPHERALS_REGION][2] =  portMPU_REGION_READ_WRITE;
    pMpu->WORD[portGENERAL_PERIPHERALS_REGION][3] = 0x00000001;// region is valid

    // Set RGD2 ARM defined private peripherals
    pMpu->WORD[portPRIVATE_PERIPHERALS_REGION][0] = PRIVATE_PERIPHERAL_START;
    pMpu->WORD[portPRIVATE_PERIPHERALS_REGION][1] = (PRIVATE_PERIPHERAL_START + PERIPHERAL_SIZE);// End Address
    pMpu->WORD[portPRIVATE_PERIPHERALS_REGION][2] = portMPU_REGION_PRIVILEGED_READ_WRITE;
    pMpu->WORD[portPRIVATE_PERIPHERALS_REGION][3] = 0x00000001;// region is valid

    // Set RGD2 ARM defined FlexMem area for persistant storage
    pMpu->WORD[portPERSISTANT_DATA_REGION][0] = PERSISTANT_DATA_START;
    pMpu->WORD[portPERSISTANT_DATA_REGION][1] = (PERSISTANT_DATA_START + PERSISTANT_DATA_SIZE);// End Address
    pMpu->WORD[portPERSISTANT_DATA_REGION][2] = portMPU_REGION_READ_WRITE;
    pMpu->WORD[portPERSISTANT_DATA_REGION][3] = 0x00000001;// region is valid

    // Enable MPU function
    pMpu->CESR = 0x00000001;
}

PRIVILEGED_FUNCTION
#if 1
void vPortStoreTaskMPUSettings( xMPU_SETTINGS *xMPUSettings, const struct xMEMORY_REGION * const xRegions, portSTACK_TYPE *pxBottomOfStack, unsigned short usStackDepth )
{
    xMPUSettings->xRegion[ 0 ].ulRegionAttribute = 0xDEADD00D;
}
#else
void vPortStoreTaskMPUSettings( xMPU_SETTINGS *xMPUSettings, const struct xMEMORY_REGION * const xRegions, portSTACK_TYPE *pxBottomOfStack, unsigned short usStackDepth )
{
    unsigned portBASE_TYPE lIndex;
    unsigned portBASE_TYPE ul;

    if( xRegions == NULL )
    {
        /* No MPU regions are specified so allow read/execute access to all ROM. */
        xMPUSettings->xRegion[ 0 ].ulRegionBaseAddress = ((unsigned portBASE_TYPE)__section_end("privileged_functions")) + 1;
        xMPUSettings->xRegion[ 0 ].ulRegionEndAddress =  xMPUSettings->xRegion[ 0 ].ulRegionBaseAddress +
                                                        (FLASH_SIZE - ((unsigned portBASE_TYPE)__section_size("privileged_functions")));
        xMPUSettings->xRegion[ 0 ].ulRegionAttribute =  portMPU_REGION_READ_ONLY;

        /* No MPU regions are specified so allow read/write access to all non-privileged RAM. */
        xMPUSettings->xRegion[ 1 ].ulRegionBaseAddress = ((unsigned portBASE_TYPE)__section_end("privileged_data")) + 1;
        xMPUSettings->xRegion[ 1 ].ulRegionEndAddress = xMPUSettings->xRegion[ 1 ].ulRegionBaseAddress +
                                                        (SRAM_SIZE - ((unsigned portBASE_TYPE)__section_size("privileged_data")));
        xMPUSettings->xRegion[ 1 ].ulRegionAttribute =  portMPU_REGION_READ_WRITE;

        /* Invalidate all other regions. */
        for( ul = 2; ul <= portNUM_CONFIGURABLE_REGIONS; ul++ )
        {
            xMPUSettings->xRegion[ ul ].ulRegionBaseAddress = 0UL;
            xMPUSettings->xRegion[ ul ].ulRegionEndAddress = 0UL;
            xMPUSettings->xRegion[ ul ].ulRegionAttribute = 0UL;
        }
    }
    else
    {
        /* This function is called automatically when the task is created - in
        which case the stack region parameters will be valid.  At all other
        times the stack parameters will not be valid and it is assumed that the
        stack region has already been configured. */
        if( usStackDepth > 0 )
        {
            /* Define the region that allows access to the stack. */
            xMPUSettings->xRegion[ 0 ].ulRegionBaseAddress = (unsigned portBASE_TYPE) pxBottomOfStack;
            xMPUSettings->xRegion[0].ulRegionEndAddress = xMPUSettings->xRegion[ 0 ].ulRegionBaseAddress +
                                                          (usStackDepth * (unsigned portBASE_TYPE)sizeof( portSTACK_TYPE ));
            xMPUSettings->xRegion[ 0 ].ulRegionAttribute = portMPU_REGION_READ_WRITE;
        }

        lIndex = 0;
        for( ul = 1; ul <= portNUM_CONFIGURABLE_REGIONS; ul++ )
        {
            if( ( xRegions[ lIndex ] ).ulLengthInBytes > 0UL )
            {
                /* Translate the generic region definition contained in
                xRegions into the CM3 specific MPU settings that are then
                stored in xMPUSettings. */
                xMPUSettings->xRegion[ ul ].ulRegionBaseAddress =
                                           (unsigned portBASE_TYPE)(xRegions[ lIndex ].pvBaseAddress); /* Region number. */
                xMPUSettings->xRegion[ ul ].ulRegionEndAddress = xMPUSettings->xRegion[ ul ].ulRegionBaseAddress + (xRegions[ lIndex ].ulLengthInBytes);
                xMPUSettings->xRegion[ ul ].ulRegionAttribute = xRegions [ lIndex ].ulParameters;
            }
            else
            {
                /* Invalidate the region. */
                xMPUSettings->xRegion[ ul ].ulRegionBaseAddress = 0UL;
                xMPUSettings->xRegion[ ul ].ulRegionEndAddress = 0UL;
                xMPUSettings->xRegion[ ul ].ulRegionAttribute = 0UL;
            }

            lIndex++;
        }
    }
}
#endif



// Assembly language raisepriv
extern portBASE_TYPE raisepriv( void );
static portBASE_TYPE prvRaisePrivilege( void )
{
    return (raisepriv());
}

#define portRESET_PRIVILEGE( xRunningPrivileged ) if( xRunningPrivileged != pdTRUE ) asm( "push {r0} \n" "mrs r0, control \n" "orr r0, r0, #1 \n" "msr control, r0 \n" "pop {r0}")

signed portBASE_TYPE MPU_xTaskGenericCreate( pdTASK_CODE pvTaskCode, const signed char * const pcName, unsigned short usStackDepth, void *pvParameters, unsigned portBASE_TYPE uxPriority, xTaskHandle *pxCreatedTask, portSTACK_TYPE *puxStackBuffer, const xMemoryRegion * const xRegions );
void MPU_vTaskAllocateMPURegions( xTaskHandle xTask, const xMemoryRegion * const xRegions );
void MPU_vTaskDelete( xTaskHandle pxTaskToDelete );
void MPU_vTaskDelayUntil( portTickType * const pxPreviousWakeTime, portTickType xTimeIncrement );
void MPU_vTaskDelay( portTickType xTicksToDelay );
unsigned portBASE_TYPE MPU_uxTaskPriorityGet( xTaskHandle pxTask );
void MPU_portYIELD();
void MPU_vTaskPrioritySet( xTaskHandle pxTask, unsigned portBASE_TYPE uxNewPriority );
void MPU_vTaskSuspend( xTaskHandle pxTaskToSuspend );
portBASE_TYPE MPU_xTaskIsTaskSuspended( xTaskHandle xTask );
void MPU_vTaskResume( xTaskHandle xTaskToResume );
void MPU_vTaskSuspendAll( void );
portBASE_TYPE MPU_xTaskResumeAll( void );
portTickType MPU_xTaskGetTickCount( void );
unsigned portBASE_TYPE MPU_uxTaskGetNumberOfTasks( void );
void MPU_vTaskList( signed char *pcWriteBuffer );
void MPU_vTaskGetRunTimeStats( signed char *pcWriteBuffer );
void MPU_vTaskStartTrace( signed char * pcBuffer, unsigned long ulBufferSize );
unsigned long MPU_ulTaskEndTrace( void );
void MPU_vTaskSetApplicationTaskTag( xTaskHandle xTask, pdTASK_HOOK_CODE pxTagValue );
pdTASK_HOOK_CODE MPU_xTaskGetApplicationTaskTag();
portBASE_TYPE MPU_xTaskCallApplicationTaskHook( xTaskHandle xTask, void *pvParameter );
unsigned portBASE_TYPE MPU_uxTaskGetStackHighWaterMark( xTaskHandle xTask );
xTaskHandle MPU_xTaskGetCurrentTaskHandle( void );
portBASE_TYPE MPU_xTaskGetSchedulerState( void );

xQueueHandle MPU_xQueueCreate( unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize );
xQueueHandle MPU_xQueueCreateMutex( void );
portBASE_TYPE MPU_xQueueGiveMutexRecursive( xQueueHandle xMutex );
portBASE_TYPE MPU_xQueueTakeMutexRecursive( xQueueHandle xMutex, portTickType xBlockTime );
xQueueHandle MPU_xQueueCreateCountingSemaphore( unsigned portBASE_TYPE uxCountValue, unsigned portBASE_TYPE uxInitialCount );
portBASE_TYPE MPU_xQueueGenericSend( xQueueHandle xQueue, const void * const pvItemToQueue, portTickType xTicksToWait, portBASE_TYPE xCopyPosition );
portBASE_TYPE MPU_xQueueAltGenericSend( xQueueHandle pxQueue, const void * const pvItemToQueue, portTickType xTicksToWait, portBASE_TYPE xCopyPosition );
portBASE_TYPE MPU_xQueueAltGenericReceive( xQueueHandle pxQueue, void * const pvBuffer, portTickType xTicksToWait, portBASE_TYPE xJustPeeking );
portBASE_TYPE MPU_xQueueGenericReceive( xQueueHandle pxQueue, void * const pvBuffer, portTickType xTicksToWait, portBASE_TYPE xJustPeeking );
unsigned portBASE_TYPE MPU_uxQueueMessagesWaiting( const xQueueHandle pxQueue );
void MPU_vQueueDelete( xQueueHandle pxQueue );

void* MPU_pvPortMalloc( size_t xSize );
void MPU_vPortFree( void* pv );
size_t MPU_xPortGetFreeHeapSize( void );
void MPU_vPortInitialiseBlocks( void );

#if configQUEUE_REGISTRY_SIZE > 0
    void MPU_vQueueAddToRegistry( xQueueHandle xQueue, signed char *pcName );
    void MPU_vQueueUnregisterQueue( xQueueHandle xQueue );
#endif
// Allow a reset from user mode
void MPU_vPortReset();


/* mpu wrappers for elevating privilige before execution from unpriviliged contexts */

signed portBASE_TYPE MPU_xTaskGenericCreate( pdTASK_CODE pvTaskCode, const signed char * const pcName, unsigned short usStackDepth, void *pvParameters, unsigned portBASE_TYPE uxPriority, xTaskHandle *pxCreatedTask, portSTACK_TYPE *puxStackBuffer, const xMemoryRegion * const xRegions )
{
    signed portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTaskGenericCreate( pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask, puxStackBuffer, xRegions );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}

PRIVILEGED_FUNCTION
void MPU_vTaskAllocateMPURegions( xTaskHandle xTask, const xMemoryRegion * const xRegions )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskAllocateMPURegions( xTask, xRegions );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


void MPU_vTaskDelete( xTaskHandle pxTaskToDelete )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskDelete( pxTaskToDelete );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


void MPU_vTaskDelayUntil( portTickType * const pxPreviousWakeTime, portTickType xTimeIncrement )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskDelayUntil( pxPreviousWakeTime, xTimeIncrement );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


void MPU_vTaskDelay( portTickType xTicksToDelay )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskDelay( xTicksToDelay );
    portRESET_PRIVILEGE( xRunningPrivileged );
}

void MPU_portYIELD()
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vPortYieldFromISR();
    portRESET_PRIVILEGE( xRunningPrivileged );
}

unsigned portBASE_TYPE MPU_uxTaskPriorityGet( xTaskHandle pxTask )
{
    unsigned portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = uxTaskPriorityGet( pxTask );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


void MPU_vTaskPrioritySet( xTaskHandle pxTask, unsigned portBASE_TYPE uxNewPriority )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskPrioritySet( pxTask, uxNewPriority );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


void MPU_vTaskSuspend( xTaskHandle pxTaskToSuspend )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskSuspend( pxTaskToSuspend );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


portBASE_TYPE MPU_xTaskIsTaskSuspended( xTaskHandle xTask )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTaskIsTaskSuspended( xTask );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


void MPU_vTaskResume( xTaskHandle xTaskToResume )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskResume( xTaskToResume );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


void MPU_vTaskSuspendAll( void )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskSuspendAll();
    portRESET_PRIVILEGE( xRunningPrivileged );
}


portBASE_TYPE MPU_xTaskResumeAll( void )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTaskResumeAll();
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


portTickType MPU_xTaskGetTickCount( void )
{
    portTickType xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTaskGetTickCount();
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


unsigned portBASE_TYPE MPU_uxTaskGetNumberOfTasks( void )
{
    unsigned portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = uxTaskGetNumberOfTasks();
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


void MPU_vTaskList( signed char *pcWriteBuffer )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskList( pcWriteBuffer );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


void MPU_vTaskGetRunTimeStats( signed char *pcWriteBuffer )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskGetRunTimeStats( pcWriteBuffer );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


void MPU_vTaskStartTrace( signed char * pcBuffer, unsigned long ulBufferSize )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    portRESET_PRIVILEGE( xRunningPrivileged );
}


unsigned long MPU_ulTaskEndTrace( void )
{
    unsigned long xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = MPU_ulTaskEndTrace();
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}
#if ( configUSE_APPLICATION_TASK_TAG == 1 )


void MPU_vTaskSetApplicationTaskTag( xTaskHandle xTask, pdTASK_HOOK_CODE pxTagValue )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vTaskSetApplicationTaskTag( xTask, pxTagValue );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


pdTASK_HOOK_CODE MPU_xTaskGetApplicationTaskTag( void )
{
    pdTASK_HOOK_CODE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTaskGetApplicationTaskTag();
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


portBASE_TYPE MPU_xTaskCallApplicationTaskHook( xTaskHandle xTask, void *pvParameter )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTaskCallApplicationTaskHook( xTask, pvParameter );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}

#endif /* configUSE_APPLICATION_TASK_TAG == 1 */


unsigned portBASE_TYPE MPU_uxTaskGetStackHighWaterMark( xTaskHandle xTask )
{
    unsigned portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = uxTaskGetStackHighWaterMark( xTask );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


xTaskHandle MPU_xTaskGetCurrentTaskHandle( void )
{
    xTaskHandle xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTaskGetCurrentTaskHandle();
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


portBASE_TYPE MPU_xTaskGetSchedulerState( void )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTaskGetSchedulerState();
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


xQueueHandle MPU_xQueueCreate( unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize )
{
    xQueueHandle xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xQueueCreate( uxQueueLength, uxItemSize );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


xQueueHandle MPU_xQueueCreateMutex( void )
{
    xQueueHandle xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xQueueCreateMutex();
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


portBASE_TYPE MPU_xQueueGiveMutexRecursive( xQueueHandle xMutex )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xQueueGiveMutexRecursive( xMutex );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


portBASE_TYPE MPU_xQueueTakeMutexRecursive( xQueueHandle xMutex, portTickType xBlockTime )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xQueueTakeMutexRecursive( xMutex, xBlockTime );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


xQueueHandle MPU_xQueueCreateCountingSemaphore( unsigned portBASE_TYPE uxCountValue, unsigned portBASE_TYPE uxInitialCount )
{
    xQueueHandle xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xQueueCreateCountingSemaphore( uxCountValue, uxInitialCount );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


portBASE_TYPE MPU_xQueueGenericSend( xQueueHandle xQueue, const void * const pvItemToQueue, portTickType xTicksToWait, portBASE_TYPE xCopyPosition )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xQueueGenericSend( xQueue, pvItemToQueue, xTicksToWait, xCopyPosition );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


portBASE_TYPE MPU_xQueueAltGenericSend( xQueueHandle pxQueue, const void * const pvItemToQueue, portTickType xTicksToWait, portBASE_TYPE xCopyPosition )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xQueueAltGenericSend( pxQueue, pvItemToQueue, xTicksToWait, xCopyPosition );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


portBASE_TYPE MPU_xQueueAltGenericReceive( xQueueHandle pxQueue, void * const pvBuffer, portTickType xTicksToWait, portBASE_TYPE xJustPeeking )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xQueueAltGenericReceive( pxQueue, pvBuffer, xTicksToWait, xJustPeeking );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


portBASE_TYPE MPU_xQueueGenericReceive( xQueueHandle pxQueue, void * const pvBuffer, portTickType xTicksToWait, portBASE_TYPE xJustPeeking )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xQueueGenericReceive( pxQueue, pvBuffer, xTicksToWait, xJustPeeking );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


unsigned portBASE_TYPE MPU_uxQueueMessagesWaiting( const xQueueHandle pxQueue )
{
    unsigned portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = uxQueueMessagesWaiting( pxQueue );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


void MPU_vQueueDelete( xQueueHandle pxQueue )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vQueueDelete( pxQueue );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


void* MPU_pvPortMalloc( size_t xSize )
{
    void* xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = pvPortMalloc( xSize );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


void MPU_vPortFree( void* pv )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vPortFree( pv );
    portRESET_PRIVILEGE( xRunningPrivileged );
}


size_t MPU_xPortGetFreeHeapSize( void )
{
    size_t xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xPortGetFreeHeapSize();
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}


void MPU_vPortInitialiseBlocks( void )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vPortInitialiseBlocks();
    portRESET_PRIVILEGE( xRunningPrivileged );
}

#if  configQUEUE_REGISTRY_SIZE > 0


void MPU_vQueueAddToRegistry( xQueueHandle xQueue, signed char *pcName )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vQueueAddToRegistry( xQueue, pcName );
    portRESET_PRIVILEGE( xRunningPrivileged );
}

/*

void MPU_vQueueUnregisterQueue( xQueueHandle xQueue )
{
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    vQueueUnregisterQueue( xQueue );
    portRESET_PRIVILEGE( xRunningPrivileged );
}
*/

#endif

#if configUSE_TIMERS > 0


xTimerHandle MPU_xTimerCreate( const signed char *pcTimerName, portTickType xTimerPeriod, unsigned portBASE_TYPE uxAutoReload, void * pvTimerID, tmrTIMER_CALLBACK pxCallbackFunction )
{
    xTimerHandle xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTimerCreate( pcTimerName, xTimerPeriod, uxAutoReload, pvTimerID, pxCallbackFunction );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}

portBASE_TYPE MPU_xTimerIsTimerActive( xTimerHandle xTimer )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTimerIsTimerActive( xTimer );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}

portBASE_TYPE MPU_xTimerGenericCommand( xTimerHandle xTimer, portBASE_TYPE xCommandID, portTickType xOptionalValue, portBASE_TYPE *pxHigherPriorityTaskWoken, portTickType xBlockTime )
{
    portBASE_TYPE xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = xTimerGenericCommand( xTimer, xCommandID, xOptionalValue, pxHigherPriorityTaskWoken, xBlockTime );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}

void* MPU_pvTimerGetTimerID( xTimerHandle xTimer )
{
    void* xReturn;
    portBASE_TYPE xRunningPrivileged = prvRaisePrivilege();
    xReturn = pvTimerGetTimerID( xTimer );
    portRESET_PRIVILEGE( xRunningPrivileged );
    return xReturn;
}
#endif

/* port code for a privileged system reset */

#include "core_cm4.h"
extern void NVIC_SystemReset(void);

void MPU_vPortReset()
{
    prvRaisePrivilege();
    NVIC_SystemReset();
}


