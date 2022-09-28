# robomaster
全国大学生机器人大赛 RoboMaster 机甲大师赛 

团队项目，所以只能展示一小部分

资料整理中...

![image](https://user-images.githubusercontent.com/47512823/192125111-bae2a9d6-8e50-4656-bad6-5d96ba111ccf.png)

# 2017

补给站采用四个碰撞开关触发，两个舵机控制加弹舱门，通过实践控制加弹数量的静态装置。

补给站每两个开关和一个舵机为一组，共两组。可以同时给两辆车补给弹丸。需要补给的机器人向前撞击碰撞开关前的挡板1S,主控控制舵机打开舱门，1.5S后关闭舱门，然后检测开关状态，只有当开关松开后，补给站才能进入下次补给准备阶段，否则不会再次打开舱门。避免误触发。

优点：可以同时处理两路任务，同时加弹，互不影响。

缺点：上部漏斗面积过大、卡弹，导致加弹数量不好控制，加弹过程及加弹量操作手不知道。

改进意见：上部漏斗慢慢试卡弹问题，面积缩小。机器人回补给站次数有限，所以加弹数量不限制，一次将漏斗中弹药加完，加弹机器人摄像头前方加面镜子，保证操作手能看见加弹数量。


```cpp
// 1.	servo类下有以下成员函数
          attach();      		//连接舵机，设定舵机的接口。2~13接口可利用。
          write();               //角度控制,用于设定舵机旋转角度的语句，可设定的角度范围是
							   0°到180°。这条语句是直接写角度，舵机的每个转动到了一个
							位置都有相应的角度。舵机通过控制PWM的宽度来决定转的角度。
          writeMicroseconds();   //用于设定舵机旋转角度的语句，直接用微秒作为参数。
          read();                //读上一次舵机转动角度，用于读取舵机角度的语句，可理解为读
							   取最后一条write()命令中的值。
          attached();            //判断舵机参数是否已发送到舵机所在接口。
          detach();              //断开舵机连接
// 2.	void test_Steering_gear()//串口接收数据并对舵机赋值（调试用）
```
