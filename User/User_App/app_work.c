#include "app.h"

#define WORK_TASK_PERIOD  1


appStruct_t appWork;
robotSelectStruct_t robotSelect;

void app_fanMode(fanMode_e fanDirection)
{
	
}

void app_fan()
{
	PDout(15) = 1;
	PDout(14) = 1;
	vTaskDelay(800);
	PDout(15) = 0;
	PDout(14) = 0;
	vTaskDelay(500);
	outfireRobotState.fireBassarl = true;
}


calibrationFinish_e checkFire(void){
	calibrationFinish_e finshFlag;
	finshFlag = CALIBRATION_NULL;
	if(outfireRobotState.fireBassarl == true){
		if(adcTestData.adcData[L_ADC] < 300|| adcTestData.adcData[R_ADC] < 300){
			app_fan();
			finshFlag = CALIBRATION_WAITING;
		}
		else if(adcTestData.adcData[L_ADC] > 300 && adcTestData.adcData[R_ADC] > 300){
			outfireRobotState.fireBassarl = false;
			finshFlag = CALIBRATION_FINISHED;
		}
	}
		return finshFlag;
}


void app_WorkCommandReset(void)    //恢复初始状态标志位
{
	outfireRobotState.testTarget = WAITING;
	outfireRobotState.lastMoveWays = OTHER;
	outfireRobotState.moveWays = STOP;
	outfireRobotState.step = READY;
	outfireRobotState.beginFlag = FREE;
	outfireRobotState.workMode = TEST;
	driver_FanMotorOff();	
}

/***灭火1机器人主任务**/
void app_outfireOneRobotTask(void){
		switch(outfireRobotState.step){
			case INIT: app_WorkCommandReset(); break;
			case READY: app_outfireOneWorkReady(); break;
			case DOING: app_outfireOneWorkDoing(); break;
			case FINISH: break;
			default:break;
		}
}
	/***灭火机器人2主任务**/
void app_outfireTwoRobotTask(void){
	switch(outfireRobotState.step){
			case INIT: app_WorkCommandReset(); break;
			case READY: app_outfireTwoWorkReady(); break;
			case DOING: app_outfireTwoWorkDoing(); break;
			case FINISH: break;
			default:break;
	}
}
	/***救援机器人主任务**/
void app_rescueRobotTask(void){

}

void app_WorkUpdata(robotSelectStruct_t *robotSelect) 	//机器人任务分类
{
	
	switch(robotSelect->robotSelect){
		case OUT_FIRE1:
			app_outfireOneRobotTask();
			break;
		case OUT_FIRE2:
			app_outfireTwoRobotTask();
			break;
		case RESCUE:
			app_rescueRobotTask();
			break;
	}
}
void app_WorkTask(void *Parameters)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeTime, WORK_TASK_PERIOD);
		app_WorkUpdata(&robotSelect);
		vTaskDelay(1);
	}
}

void app_WorkTaskInit(void){
	xTaskCreate(app_WorkTask,"WORK",WORK_STACK_SIZE,NULL,WORK_PRIORITY,&appWork.xHandleTask);
	robotSelect.robotSelect = OUT_FIRE2;
}

