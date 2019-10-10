#include "app.h"
LimiFlag_t LimiFlag;

void app_rescueWorkReady(void)
{
	/*待补充*/ //用按键控制小车启动//
	rescueRobotState.step = DOING;
	command = '0';
}
/***救援任务**/
void app_rescueWorkDoing(void){
	app_UCLIMI_FORNT(SRF_04_Data1.getDistance,SRF_04_Data2.getDistance); //前限位
	app_UCLIMI_LEFT (SRF_04_Data3.getDistance,SRF_04_Data4.getDistance);  //左限位
	app_UCLIMI_RIGHT(SRF_04_Data5.getDistance,SRF_04_Data6.getDistance); //右限位
	app_UCLIMI_SPEED(SRF_04_Data1.getDistance,SRF_04_Data2.getDistance);  //速度限制
	wifiControl(command);
}


///////////救援串口控制限位///////////////////
void app_UCLIMI_SPEED(float distance1,float distance2){
	if((distance1<25.0)&&(distance2<25.0)){
	  speed_cut = go;
	}
	else{
		speed_cut = stop;
	}
}
void app_UCLIMI_FORNT(float distance1,float distance2){//前限位
 if((distance1<15.0)&&(distance2<15.0))  LimiFlag.forward = stop;
   else  LimiFlag.forward = go;
} 
void app_UCLIMI_LEFT(float distance1,float distance2){ //左限位
 if((distance1<13.0)&&(distance2<13.0)) LimiFlag.left = stop;
   else  LimiFlag.left = go;
}  
void app_UCLIMI_RIGHT(float distance1,float distance2){//右限位
 if((distance1<13.0)&&(distance2<1.0))  LimiFlag.right = stop;
   else LimiFlag.right = go;
}


///////////rescue_AUTOTASK/////////////////
void rescue_AUTOTASK_PUTCLAW(void){
		app_CLAW_HOLDorUNON(claw_UNON,185);
    vTaskDelay(20);		
		app_CLAW_UPorDOWN(claw_DOWN,268);
		app_CAMERA_UPorDOWN(camera_DOWN,220);	
}
void rescue_AUTOTASK_HOLDCLAW(void){
		app_CLAW_HOLDorUNON(claw_HOLD,318);
    vTaskDelay(500);	
		app_CLAW_UPorDOWN(claw_UP,240);
		app_CAMERA_UPorDOWN(camera_UP,130);
}
