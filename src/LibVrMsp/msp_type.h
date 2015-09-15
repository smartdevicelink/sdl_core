#ifndef   __MSP_PASSTHRU__
#define  __MSP_PASSTHRU__

#include "lib_msp_vr.h"
#include <pthread.h>

#include <stdio.h>
#include <string>
#include <stdlib.h>

#ifndef   OS_MAC
#include <include/msp_errors.h>
#include "asr.h"
#endif

#ifdef OS_WIN32
#include <Windows.h>
#else
#include <string.h>
#endif

typedef struct  __VR_parm
{
	int max_duration;
	int sampling_rate;
	int bits_per_sample;
	int audio_type;
}VR_parm;
//MAX SIZE
#define SHARE_MAX_SIZE   (1024*1024)
typedef struct  __VR_data
{
	pthread_mutex_t   mutex_;
	char data[SHARE_MAX_SIZE];
	int len;
	int begin;
	int end;
}VR_data;
typedef struct  __ShareMemData
{
	int vr_type;
	int state;
	VR_data data;
	bool login;//
	bool upload;//
	VR_parm  vr_parm;
}ShareMemData;


#define   MEM_SHARE_SIZE    (sizeof(ShareMemData))
#define   MEM_SHARE_DATA    (sizeof(ShareMemData))

//passthru
typedef int SR_DWORD;
typedef short int SR_WORD;


#endif
