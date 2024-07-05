#ifndef __MY_COMMON_H_
#define __MY_COMMON_H_

#include "headfile.h"

typedef unsigned char bool;
#define true 1
#define false 0

#define PIT_TIME_MS 5
#define TIMER_COUNT_MS(real_time_ms) (real_time_ms / PIT_TIME_MS)

//====================================================�궨�庯����====================================================
//-------------------------------------------------------------------------------------------------------------------
// �������     ����ֵ���� ���ݷ�Χ�� [-32767,32767]
// ����˵��     dat             ��Ҫ�����ֵ����
// ���ز���     int             ���ؾ���ֵ
// ʹ��ʾ��     dat = func_abs(dat);                            // ��dat�������
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
#define FUNC_ABS(x) ((x) >= 0 ? (x) : (-(x)))

//-------------------------------------------------------------------------------------------------------------------
// �������     �޷� ���ݷ�Χ�� [-32768,32767]
// ����˵��     x               ���޷�������
// ����˵��     y               �޷���Χ(���ݻᱻ������-y��+y֮��)
// ���ز���     int             �޷�֮�������
// ʹ��ʾ��     int dat = func_limit(500, 300);                 // ���ݱ�������-300��+300֮��  ��˷��صĽ����300
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
#define FUNC_LIMIT(x, y) ((x) > (y) ? (y) : ((x) < -(y) ? -(y) : (x)))

//-------------------------------------------------------------------------------------------------------------------
// �������     ˫���޷� ���ݷ�Χ�� [-32768,32767]
// ����˵��     x               ���޷�������
// ����˵��     a               �޷���Χ��߽�
// ����˵��     b               �޷���Χ�ұ߽�
// ���ز���     int             �޷�֮�������
// ʹ��ʾ��     int dat = func_limit_ab(500, -300, 400);        //���ݱ�������-300��+400֮��  ��˷��صĽ����400
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
#define FUNC_LIMIT_AB(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

extern int8 element_busy_flag;

void Bubble_Sort_Int16(int16 data_array[], uint8 length);
void Fast_De_Extremum_Averaging(int16 input_array[], uint8 array_length, int16 *output_avage);
void Trimmed_Mean_Filter(int16 data_array[], uint8 size, uint8 extreme_number, int16 *filtered_value);
void Insertion_Sort(int16 array[], uint8 size);
int16 Low_Pass_Filter(int16 new_value, int16 last_value, float fliter_factor);

float Inv_Sqrt(float x);
float My_Sqrt(float x);

float Calculate_Minkowski_Distance(int16 inductor[], int16 target[], uint8 size, uint8 p);
float Calculate_Slope(int *data_array, int n);
#endif