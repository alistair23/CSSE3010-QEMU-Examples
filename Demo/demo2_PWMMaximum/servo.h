#ifndef SERVO_H
#define SERVO_H

void Servo_Console_Control(char console_rx);
void Servo_Radio_Control(char* payload);

void Servo_Pan_Init(void);
void Servo_Pan_Set_Angle(int8_t angle);
void Servo_Pan_Increment_Angle(int8_t angle);
int8_t Servo_Get_Pan_Angle(void);

void Servo_Tilt_Init(void);
void Servo_Tilt_Set_Angle(int8_t angle);
void Servo_Tilt_Increment_Angle(int8_t angle);
int8_t Servo_Get_Tilt_Angle(void);

#endif
