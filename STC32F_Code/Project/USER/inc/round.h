#ifndef _ROUND_H_
#define _ROUND_H_

#include "headfile.h"

typedef enum ROUND_STATE //RIGHT_ANGLE_STATE��һ����ʶ�������Կ���������ϵ����֣���һ����ѡ����ǿ��п��޵��
{
    ROUND_NONE = 0,
    ROUND_LEFT_PRE, //Ԥ�뻷
    ROUND_LEFT_INTO, //�뻷
    ROUND_LEFT_IN,   //����
    ROUND_LEFT_OUT,
    ROUND_RIGHT_PRE,
    ROUND_RIGHT_INTO,    
    ROUND_RIGHT_IN,
    ROUND_RIGHT_OUT
} ROUND_STATE_enmu;

extern ROUND_STATE_enmu round_flag;
extern float round_right_knn;
extern float round_proximity_coeff;

void Round_Recognition(void);
void Round_Turn_Process(void);

#endif