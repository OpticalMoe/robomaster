/*
   第十六届RoboMaster沈阳理工大学Ambition战队-补给站
   作者：陈元亮

     servo类下有以下成员函数
          attach();                  //连接舵机，设定舵机的接口。2~13接口可利用。
          write();                   //角度控制,用于设定舵机旋转角度的语句，可设定的角度范围是0°到180°。这条语句是直接写角度，舵机的每个转动到了一个位置都有相应的角度。舵机通过控制PWM的宽度来决定转的角度。
          writeMicroseconds();       //用于设定舵机旋转角度的语句，直接用微秒作为参数。
          read();                    //读上一次舵机转动角度，用于读取舵机角度的语句，可理解为读取最后一条write()命令中的值。
          attached();                //判断舵机参数是否已发送到舵机所在接口。
          detach();                  //断开舵机连接
*/
#include <Servo.h>
#include <MsTimer2.h>               //定时器库的 头文件
#include <avr/wdt.h>                //看门狗

#define TIMEOUT WDTO_120MS             //看门狗  120ms
//所有舵机开关值
#define Servo_0_turn_on   1890            //开
#define Servo_0_turn_off  900             //关

#define Servo_1_turn_on   1000
#define Servo_1_turn_off  1970

#define Servo_2_turn_on   1000
#define Servo_2_turn_off  1970

#define Servo_3_turn_on   1890
#define Servo_3_turn_off  900

const long int time_1_execution = 1100;//舵机1开关时间
const long int time_2_execution = 1000;//舵机2开关时间

Servo myservo;
//Servo myservo_1;
Servo myservo_2;
//Servo myservo_3;

struct base                         //结构体
{
  unsigned long int time_execute = 0;//执行时间
  bool flag_detect, flag_time;//开关标志位，时间戳标志位
  int count;//计数
} supply_1, supply_2;

const int key[4] = {12, 14, 10, 11};  //触发开关
const int led[4] = {16, 17, 13, 15};  //RGB

bool flag_duoji = 0;

void setup()
{
  myservo.attach(3);                    //舵机控制引脚
  //  myservo_1.attach(5);
  myservo_2.attach(6);
  //  myservo_3.attach(9);
  myservo.writeMicroseconds(Servo_0_turn_off);
  //  myservo_1.writeMicroseconds(Servo_1_turn_off);
  myservo_2.writeMicroseconds(Servo_2_turn_off);
  //  myservo_3.writeMicroseconds(Servo_3_turn_off);

  Serial.begin(9600);                   //串口波特率

  //配置引脚模式
  for (int i = 0; i < 4; i++)
  {
    pinMode(key[i], INPUT);
    digitalWrite(key[i], HIGH);
  }
  for (int i = 0; i < 4; i++)
  {
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], HIGH);
  }

  MsTimer2::set(10, interrupt);        // 中断设置函数，每 10ms 进入一次中断
  MsTimer2::start();                  //开始计时
  Init_1();                           //初始化1口变量
  Init_2();                           //初始化2口变量

  wdt_enable(TIMEOUT);                //使能看门狗
}

void loop()
{
  //  test_Steering_gear();//测试程序，串口收脉冲时间
  //        Serial_to_pc();//串口返回数据

  carried_out_1();
  carried_out_2();

  wdt_reset();//喂狗

  //  Serial.println(millis());
  //测试程序
  /*  if (!digitalRead(key[2]))
    {
      while (!digitalRead(key[2]));
      flag_duoji = !flag_duoji;
    }
    if (flag_duoji)
      if (millis() % 3000 < 1500)  myservo_3.writeMicroseconds(Servo_3_turn_on); //开
      else   myservo_3.writeMicroseconds(Servo_3_turn_off);//关
    else myservo_3.writeMicroseconds(1500);//关
  */
}

void carried_out_1()//执行
{
  if (supply_1.count > 100)
  {
    supply_1.flag_detect = 0;//清除触发标志位，不再读取开关状态
    if (!supply_1.flag_time)//触发1s
    {
      supply_1.time_execute = millis();//记录时间戳
      supply_1.flag_time = 1;//设置标志位
    }
    if ((millis() - supply_1.time_execute < time_1_execution) && supply_1.time_execute != 0)//触发时间内
    {
      myservo.writeMicroseconds(Servo_0_turn_on);//开
      digitalWrite(led[1], LOW);//开
    }
    else
    {
      myservo.writeMicroseconds(Servo_0_turn_off);//关
      digitalWrite(led[1], HIGH);//关
      if (digitalRead(key[0]) && digitalRead(key[1])) Init_1();//开关松手检测，初始化变量
    }
  }
}

void carried_out_2()
{
  if (supply_2.count > 100)
  {
    supply_2.flag_detect = 0;
    if (!supply_2.flag_time)
    {
      supply_2.time_execute = millis();
      supply_2.flag_time = 1;
    }
    if ((millis() - supply_2.time_execute < time_2_execution) && supply_2.time_execute != 0)
    {
      myservo_2.writeMicroseconds(Servo_2_turn_on);//开
      digitalWrite(led[3], LOW);//开
    }
    else
    {
      myservo_2.writeMicroseconds(Servo_2_turn_off);//关
      digitalWrite(led[3], HIGH);//开
      if (digitalRead(key[2]) && digitalRead(key[3])) Init_2();
    }
  }
}

void interrupt()//中断函数，读开关
{
  //1
  if (supply_1.flag_detect)//开关标志位，是否读取开关状态
    if (!digitalRead(key[0]) || !digitalRead(key[1]))//触发
    {
      supply_1.count++;
      digitalWrite(led[0], LOW);//开
    }
    else
    {
      supply_1.count = 0;
      digitalWrite(led[0], HIGH);//开
    }

  //2
  if (supply_2.flag_detect)
    if (!digitalRead(key[2]) || !digitalRead(key[3]))
    {
      supply_2.count++;
      digitalWrite(led[2], LOW);//开
    }
    else
    {
      supply_2.count = 0;
      digitalWrite(led[2], HIGH);//开
    }
}

void Init_1()//初始化变量
{
  supply_1.flag_detect = 1;
  supply_1.flag_time = 0;
  supply_1.time_execute = 0;
  supply_1.count = 0;
}

void Init_2()//初始化变量
{
  supply_2.flag_detect = 1;
  supply_2.flag_time = 0;
  supply_2.time_execute = 0;
  supply_2.count = 0;
}

void Serial_to_pc()//串口回传数据
{
  Serial.print(millis());
  Serial.print("\t");
  Serial.print(supply_1.time_execute);
  Serial.print("\t");
  Serial.print(supply_2.time_execute);
  Serial.print("\t");

  Serial.print(supply_1.flag_detect);
  Serial.print(",");
  Serial.print(supply_1.flag_time);
  Serial.print(",");
  Serial.print(supply_1.count);
  Serial.print("\t");

  Serial.print(supply_2.flag_detect);
  Serial.print(",");
  Serial.print(supply_2.flag_time);
  Serial.print(",");
  Serial.print(supply_2.count);
  Serial.print("\t");

  //  Serial.print(flag_duoji);
  //  Serial.print("\t");


  //  Serial.print("key:");
  Serial.print(!digitalRead(key[0]));
  Serial.print(",");
  Serial.print(!digitalRead(key[1]));
  Serial.print(",");
  Serial.print(!digitalRead(key[2]));
  Serial.print(",");
  Serial.print(!digitalRead(key[3]));
  Serial.print("\t");

  Serial.println(" ");
}

void test_Steering_gear()//串口接收数据并对舵机赋值
{
  if (Serial.available() > 0)
  {
    int Servo_time = Serial_read();
    myservo.writeMicroseconds(Servo_time);
    //    myservo_1.writeMicroseconds(Servo_time);
    myservo_2.writeMicroseconds(Servo_time);
    //    myservo_3.writeMicroseconds(Servo_time);
  }
}

int Serial_read()//串口接收数据并返回整形
{
  int i = 0, pos = 0;
  char data[5];
  bool Serial_flag = false;

  while (Serial.available() > 0)
  {
    data [i] = Serial.read();
    i++;
    Serial_flag = true;
    delay(2);
  }

  if (Serial_flag == true)
  {
    data[i++] = 0;
    pos = atoi(data);
    Serial.println(pos);
  }
  return pos;
}
