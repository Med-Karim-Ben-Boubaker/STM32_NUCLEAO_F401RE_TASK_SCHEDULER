/*
 * main.h
 *
 *  Created on: Jul 20, 2023
 *      Author: user
 */

#ifndef MAIN_H_
#define MAIN_H_

//GLOBAL DEFINITIONS
#define MAX_TASKS			4
#define SRAM_ADDRESS 			0x20000000U
#define SRAM_SIZE			( (64) * (1024) )
#define SRAM_END			( (SRAM_ADDRESS) + (SRAM_SIZE) )

#define TASK_SIZE			1024U
#define TASK1_START			SRAM_END
#define TASK2_START			( (SRAM_END) - (1 * TASK_SIZE) )
#define TASK3_START			( (SRAM_END) - (2 * TASK_SIZE) )
#define TASK4_START			( (SRAM_END) - (3 * TASK_SIZE) )
#define SCHED_STACK_START	( (SRAM_END) - (4 * TASK_SIZE) )

#define TICK_HZ				1000U //Desired switching frequency
#define SYSTICK_TIM_CLK		16000000U
#define DUMMY_XPSR 			0x01000000U

//Task names initialization
void task1(void);
void task2(void);
void task3(void);
void task4(void);

///Initialization of task stack
void init_task_stack(void);

//Initialization of SysTick Timer
void init_systick_timer(uint32_t tick_hz);

//Initialization of Scheduler stack
__attribute__((naked)) void init_schedular_stack(uint32_t sched_top_of_stack);

//Enable processor faults
void enable_processor_faults(void);

//Switch Stack Pointer to PSP
__attribute__((naked)) void switch_sp_to_psp(void);

//Save PSP Value
void save_psp_value(uint32_t);

//Update the next task
void update_next_task(void);



#endif /* MAIN_H_ */
