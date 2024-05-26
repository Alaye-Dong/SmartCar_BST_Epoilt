#ifndef _MY_COMMON_H_
#define _MY_COMMON_H_

#include "headfile.h"

//====================================================宏定义函数区====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     绝对值函数 数据范围是 [-32767,32767]
// 参数说明     dat             需要求绝对值的数
// 返回参数     int             返回绝对值
// 使用示例     dat = func_abs(dat);                            // 将dat变成正数
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define     FUNC_ABS(x)             ((x) >= 0 ? (x): (-(x)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     限幅 数据范围是 [-32768,32767]
// 参数说明     x               被限幅的数据
// 参数说明     y               限幅范围(数据会被限制在-y至+y之间)
// 返回参数     int             限幅之后的数据         
// 使用示例     int dat = func_limit(500, 300);                 // 数据被限制在-300至+300之间  因此返回的结果是300
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define     FUNC_LIMIT(x, y)        ((x) > (y) ? (y) : ((x) < -(y) ? -(y) : (x)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     双边限幅 数据范围是 [-32768,32767]
// 参数说明     x               被限幅的数据
// 参数说明     a               限幅范围左边界
// 参数说明     b               限幅范围右边界
// 返回参数     int             限幅之后的数据         
// 使用示例     int dat = func_limit_ab(500, -300, 400);        //数据被限制在-300至+400之间  因此返回的结果是400
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define     FUNC_LIMIT_AB(x, a, b)  ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

void Bubble_Sort_Int16(int16 data_array[], uint8 length);
void Trimmed_Mean_Filter(int16 data_array[], uint8 size, uint8 extreme_number, int16 *filtered_value);
void Insertion_Sort(int16 array[], uint8 size);
int16 Low_Pass_Filter(int16 new_value, int16 last_value, float fliter_factor);
float Inv_Sqrt(float x);
float My_Sqrt(float x);
float Knn_Calculation(int16 Knn_Left_V, int16 Knn_Right_V, int16 Knn_Left_H, int16 Knn_Right_H);
float Calculate_Similarity_Manhattan(int16 actual_value, int16 target_value);
float Proximity_Coefficient(int16 actual_values[], int16 target_values[], uint8 size);

#endif