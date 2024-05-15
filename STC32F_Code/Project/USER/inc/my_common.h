#ifndef _MY_COMMON_H_
#define _MY_COMMON_H_

#include "headfile.h"

void Bubble_Sort_Int16(int16 data_array[], uint8 length);
void Trimmed_Mean_Filter(int16 data_array[], uint8 size, uint8 extreme_number, int16 *filtered_value);
void Insertion_Sort(int16 array[], uint8 size);
int16 Limit(int16 value, int16 min, int16 max);
float Inv_Sqrt(float x);
float My_Sqrt(float x);

#endif