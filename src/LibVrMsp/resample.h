#ifndef   __RESAMPLE_H
#define  __RESAMPLE_H

#include "fa_fir.h"

//typedef unsigned uintptr_t;

#define FA_DEFAULT_FRAMELEN     1024            /* default num of sample in */
#define FA_FRAMELEN_MAX         (160*147+8192)  /* 8192 no meaning, I just set here to safe regarding */

#define FA_RATIO_MAX            16

int fa_resample( unsigned char *sample_in, int sample_in_size,
	unsigned char *sample_out, int *sample_out_size);

void fa_resample_filter_init(int L, int M, int bytes_per_sample);
void fa_resample_set(int ilen, int L, int M);
int fa_resample_8bit(unsigned char *sample_in, int sample_in_size,
	unsigned char *sample_out, int *sample_out_size);
int fa_resample_16bit(unsigned char *sample_in, int sample_in_size,
	unsigned char *sample_out, int *sample_out_size);

#endif