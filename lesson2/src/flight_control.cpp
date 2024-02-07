//Lesson 1
#include "flight_control.hpp"
#include "motor.hpp"

//Global variable
const float Control_period = 0.0025f;//400Hz //制御周期
volatile uint8_t Loop_flag = 0;
uint32_t Loop_counter = 0;

//割り込み関数
//Intrupt function
hw_timer_t * timer = NULL;
void IRAM_ATTR onTimer() 
{
  Loop_flag = 1;
}

//割り込み設定
void init_interrupt(void)
{
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 2500, true);
  timerAlarmEnable(timer);
}

//Initialize Multi copter
void init_copter(void)
{
  //Initialize Serial communication
  USBSerial.begin(115200);
  delay(1000);
  USBSerial.printf("Init StampFly!\r\n");

  //モータ設定
  init_motor();
  //割り込み設定
  init_interrupt();
  USBSerial.printf("Join StampFly!\r\n");
}

//Main loop
void loop_400Hz(void)
{
  //以下は変更しない
  while(Loop_flag==0);
  Loop_flag = 0;
  //以上は変更しない

  //Start of Loop_400Hz function
  //以下に記述したコードが400Hzで繰り返される
  if (Loop_counter < 800)
  {
    //Start motor
    set_motor_duty(FRONT_LEFT_MOTOR,  0.15);
    set_motor_duty(FRONT_RIGHT_MOTOR, 0.15);
    set_motor_duty(REAR_LEFT_MOTOR,   0.15);
    set_motor_duty(REAR_RIGHT_MOTOR,  0.15);
  }
  else
  {
    stop_motor();
  }
  
  Loop_counter ++ ;
  if(Loop_counter%400==0) USBSerial.printf("%d\r\n", Loop_counter);
  
  //End of Loop_400Hz function
}
