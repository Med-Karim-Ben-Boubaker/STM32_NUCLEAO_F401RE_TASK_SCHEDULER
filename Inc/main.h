/*
 * main.h
 *
 *  Created on: Jul 20, 2023
 *      Author: user
 */

#ifndef MAIN_H_
#define MAIN_H_


//Task names initialization
void task1(void);
void task2(void);
void task3(void);
void task4(void);
void init_task_stack(void);
void enable_processor_faults(void);
__attribute__((naked)) void switch_sp_to_psp(void);

//Initialaization of SysTick Timer
void init_systick_timer(uint32_t tick_hz);
__attribute__((naked)) void init_schedular_stack(uint32_t sched_top_of_stack);



#define MAX_TASKS			4
#define SRAM_ADDRESS 			0x20000000U
#define SRAM_SIZE			( (128)*(1024) )
#define SRAM_END			( (SRAM_ADDRESS) + (SRAM_SIZE) )
#define STACK_START			SRAM_END

#define TASK_SIZE			1024U
#define TASK1_START			STACK_START
#define TASK2_START			( (STACK_START) - (1 * TASK_SIZE) )
#define TASK3_START			( (STACK_START) - (2 * TASK_SIZE) )
#define TASK4_START			( (STACK_START) - (3 * TASK_SIZE) )
#define SCHED_STACK_START	( (STACK_START) - (4 * TASK_SIZE) )

#define TICK_HZ				1000U //Desired switching frequency
#define SYSTICK_TIM_CLK		16000000U
#define DUMMY_XPSR 			0x01000000U

#endif /* MAIN_H_ */
