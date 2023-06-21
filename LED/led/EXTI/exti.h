#ifndef __exit_H
#define __exit_H 
#include "main.h"  	

typedef struct
{
	int mit_count;
	uint8_t mit_flag;
}_MIT;


extern _MIT MIT1_S;
extern _MIT MIT2_S;
extern _MIT MIT3_S;
extern _MIT MIT4_S;
void MIT_init(void);

#endif




