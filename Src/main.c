/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include "main.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void){
	enable_processor_faults();

	//Initialization of the scheduler stack
	init_schedular_stack(SCHED_STACK_START);

	//Array filling capture addresses of tasks
	task_handlers[0] = (uint32_t)task1;
	task_handlers[1] = (uint32_t)task2;
	task_handlers[2] = (uint32_t)task3;
	task_handlers[3] = (uint32_t)task4;

	//Initialization of the task stack with dummy frames.
	init_task_stack();

	//Initialization of the SysTick timer
	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1(); //Call task1

	while(1);
}

void task1(void){
	while(1){
		printf("task1 \n");
	}
}
void task2(void){
	while(1){
		printf("task2 \n");
		}
}
void task3(void){
	while(1){
		printf("task3 \n");
		}
}
void task4(void){
	while(1){
		printf("task4 \n");
		}
}

void SysTick_Handler(void){

	/* SAVE the Current TASK */

	// get current's task PSP value
	__asm volatile("MRS R0, PSP");
	//using the PSP value, store the remaining registers ( stack frame 2 )
	__asm volatile("STMDB R0!, {R4-R11}");
	//Save the current PSP Value
	__asm volatile("BL save_psp_value");

	/* Retrieve the next TASK */

	//Decide next TASK to RUN
	__asm volatile("BL update_next_task");

	//Get the PAST PSP VALUE
	__asm volatile("BL get_psp_value");
	/*Note: Register R0 will automatically get the value of PSP*/

	//Using that PSP value, retrieve the Rest of the stack frame ( R4 - R11 )
	__asm volatile("LDMIA R0!,{R4-R11}");

	__asm volatile("MSR R0, PSP");

}

//Implement the fault handlers
void HardFault_Handler(void){
	printf("HardFault \n");
	while(1);
}
void MemManage_Handler(void){
	printf("MemManage \n");
	while(1);
}
void BusFault_Handler(void){
	printf("BusFault \n");
	while(1);
}
void UsageFault_Handler(void){
	printf("UsageFault \n");
	while(1);
}


void init_systick_timer(uint32_t tick_hz){
	uint32_t *pSYST_RVR = (uint32_t *) 0xE000E014;
	uint32_t *pSYST_CSR = (uint32_t *) 0xE000E010;
	uint32_t count_value = SYSTICK_TIM_CLK/tick_hz;

	//Clear the value of SVR
	*pSYST_RVR &= ~(0x00FFFFFF);

	//Load the value into SVR
	*pSYST_RVR |= count_value;

	//Do some settings
	*pSYST_CSR |= (1<<1); //Enables SysTick exception request
	*pSYST_CSR |= (1<<2); //Indicates the clock source, processor clock source
	*pSYST_CSR |= (1<<0); // Enables the counter
}

__attribute__((naked)) void init_schedular_stack(uint32_t sched_top_of_stack){
	__asm volatile("MSR MSP, %0": : "r" (sched_top_of_stack): );
	__asm volatile("BX LR");

}

void init_task_stack(void){
	uint32_t *pPSP;
	for(int i = 0; i< MAX_TASKS; i++){
		pPSP = (uint32_t*) psp_of_tasks[i];

		pPSP--;
		*pPSP = DUMMY_XPSR; //0x01000000

		pPSP--; //PC
		*pPSP = task_handlers[i];

		pPSP--; //LR
		*pPSP = 0xFFFFFFFD;

		for(int j=0; j<13; j++){
			pPSP--;
			*pPSP = 0;
		}

		psp_of_tasks[i] = (uint32_t) pPSP;

	}
}

void enable_processor_faults(void){
	uint32_t *pSHCRS = (uint32_t*) 0xE000ED24;
	*pSHCRS |= (1 << 16); //mem manage
	*pSHCRS |= (1 << 17); //Bus fault
	*pSHCRS |= (1 << 18); //Usage fault

}

void save_psp_value(uint32_t current_psp_value){
	psp_of_tasks[current_task] = current_psp_value;
}

uint32_t get_psp_value(void){
	return psp_of_tasks[current_task];
}

void update_next_task(void){
	current_task++;
	current_task %= MAX_TASKS;
}

__attribute__((naked)) void switch_sp_to_psp(void){
	//get the value of psp of current task
	__asm volatile("PUSH {LR}"); //preserve LR which connext back to main()
	__asm volatile("BL get_psp_value");
	__asm volatile("MSR PSP, R0"); //Initialize PSP.
	__asm volatile("POP {LR}");

	// change SP  to PSP using CONTROL Register
	__asm volatile ("MOV R0, #0x02");
	__asm volatile ("MSR CONTROL, R0");
	__asm volatile ("BX LR");

}
