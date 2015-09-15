#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#include "resample.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#define MAXV(a, b)	(((a) > (b)) ? (a) : (b))
#define MINV(a, b)	(((a) < (b)) ? (a) : (b))


typedef struct _fa_resample_filter_t {
	int L;                  /* interp factor */
	int M;                  /* decimate factor */
	float fc;               /* cutoff freqency, normalized */

	
	float gain;

	int bytes_per_sample;
	int num_in;             /* number of sample in */
	int num_out;            /* number of sample out */
	int out_index;
	int bytes_in;
	int bytes_out;

}fa_resample_filter_t;

static  fa_resample_filter_t    resample_resflt;

void fa_resample_filter_init(int L, int M, int bytes_per_sample)
{
	//fa_resample_filter_t *resflt = NULL;
	int lm_gcd;
	float ratio;

	//resflt = (fa_resample_filter_t *)malloc(sizeof(fa_resample_filter_t));
	//lm_gcd = gcd(L, M);
	L = L / 50;
	M = M / 50;
	ratio = ((float)L) / M;
	if ((ratio      > FA_RATIO_MAX) ||
		((1. / ratio) > FA_RATIO_MAX))
		return ;
	//resample set
	resample_resflt.L = L;
	resample_resflt.M = M;
	resample_resflt.fc = MINV(1. / L, 1. / M);
	resample_resflt.gain = 1.0;

	resample_resflt.bytes_per_sample = bytes_per_sample;       /*default we set to 2 bytes per sample */
	resample_resflt.out_index = 0;

	//timevary_filter_init(&(resample_resflt.tvflt), L, M, resample_resflt.fc, L, win_type);

	//return (uintptr_t)resflt;
}


void fa_resample_set(int ilen, int L, int M)
{
	/*fa_resample_filter_t *resflt = (fa_resample_filter_t *)handle;
	if (resflt == NULL)
		return;*/
	resample_resflt.num_in = ilen / resample_resflt.bytes_per_sample;
	resample_resflt.num_out = (resample_resflt.num_in*L) / M;
	resample_resflt.bytes_in =  ilen;
	resample_resflt.bytes_out = resample_resflt.bytes_per_sample * resample_resflt.num_out;

	//resample_resflt.buf = NULL;
	//resample_resflt.buf_len = (resample_resflt.tvflt.k + resample_resflt.num_in) * resample_resflt.bytes_per_sample;
	//resample_resflt.buf = (unsigned char *)malloc(resample_resflt.buf_len * sizeof(char));
	//memset(resample_resflt.buf, 0, sizeof(char)*resample_resflt.buf_len);
}



int fa_resample( unsigned char *sample_in, int sample_in_size,
	unsigned char *sample_out, int *sample_out_size)
{
	if (resample_resflt.bytes_per_sample == 2)
		return fa_resample_16bit(sample_in, sample_in_size, sample_out, sample_out_size);
	else
		return fa_resample_8bit(sample_in, sample_in_size, sample_out, sample_out_size);
}

int fa_resample_16bit( unsigned char *sample_in, int sample_in_size,
	unsigned char *sample_out, int *sample_out_size)
{

	int i;
	int l, k;
	int offset;

	//short
	short *pbuf;
	short *psample_in;
	short *psample_out;
	short *x;
	short *xp;

	int L, M, Q;
	float gain = resample_resflt.gain;

	if (sample_in_size != resample_resflt.bytes_in)
		return -1;

	L = resample_resflt.L;
	M = resample_resflt.M;
	//Q = resample_resflt.tvflt.k;

	//pbuf = (short *)resample_resflt.buf;
	psample_in = (short *)sample_in;
	psample_out = (short *)sample_out;


	/*move the old datas (flt_len-1) to the buf[0]*/
	//offset = resample_resflt.bytes_in / resample_resflt.bytes_per_sample/* - Q*/;

	/*for (i = 0; i < Q; i++)
	{
		pbuf[i] = pbuf[offset + i];
	}

	for (i = 0; i < resample_resflt.num_in; i++)
	{
		pbuf[Q + i] = psample_in[i];
	}*/


	/*set the xp point to the first sample*/
	x =psample_in /*&pbuf[Q]*/;
	*sample_out_size = 0;

	for (i = 0; i < resample_resflt.num_out; i++) {
		float y = 0.0;

		xp = x + (i*M) / L;
		l = resample_resflt.out_index%L;
		resample_resflt.out_index++;

		//for (k = 0 ; k < Q; k++) {
		//	y += xp[-k] * resample_resflt.tvflt.g[l][k]; /* decimate by M */
		//      }
		y = *xp;

		y *= gain;

		if (y > 32767)
			y = 32767;
		if (y < -32768)
			y = -32768;

		psample_out[i] = (short)y;

	}

	*sample_out_size = resample_resflt.bytes_out;

	return 0;

}

int fa_resample_8bit(unsigned char *sample_in, int sample_in_size,
	unsigned char *sample_out, int *sample_out_size)
{
	int i;
	int l, k;
	int offset;

	//pthr
	char *pbuf;
	char *psample_in;
	char *psample_out;
	char *x;
	char *xp;

	int L, M, Q;
	float gain = resample_resflt.gain;

	if (sample_in_size != resample_resflt.bytes_in)
		return -1;

	L = resample_resflt.L;
	M = resample_resflt.M;
	//Q = resample_resflt.tvflt.k;

	//pbuf = (char *)resample_resflt.buf;
	psample_in = (char *)sample_in;
	psample_out = (char *)sample_out;


	/*move the old datas (flt_len-1) to the buf[0]*/
	


	/*set the xp point to the first sample*/
	x = psample_in;
	*sample_out_size = 0;

	for (i = 0; i < resample_resflt.num_out; i++) {
		float y = 0.0;

		xp = x + (i*M) / L;
		l = resample_resflt.out_index%L;
		resample_resflt.out_index++;

		//for (k = 0; k < Q; k++) {
		//	y += xp[-k] * resample_resflt.tvflt.g[l][k]; /* decimate by M */
		//}
		y = *xp;
		y *= gain;

		if (y > 127)
			y = 127;
		if (y < -128)
			y = -128;

		psample_out[i] = (char)y;

	}

	*sample_out_size = resample_resflt.bytes_out;

	return 0;

}

