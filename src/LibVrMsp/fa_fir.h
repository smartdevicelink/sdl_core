/*
  falab - free algorithm lab 
  Copyright (C) 2012 luolongzhi 罗龙智 (Chengdu, China)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  filename: fa_fir.h 
  version : v1.0.0
  time    : 2012/07/09 23:01 
  author  : luolongzhi ( falab2012@gmail.com luolongzhi@gmail.com )
  code URL: http://code.google.com/p/falab/

*/


#ifndef _FA_FIR_H
#define _FA_FIR_H

#ifndef		M_PI
#define		M_PI							3.14159265358979323846
#endif

#ifdef __cplusplus 
extern "C"
{ 
#endif  


#ifdef OS_MAC
typedef unsigned long uintptr_t;
#else
typedef unsigned uintptr_t;
#endif
typedef int win_t;

enum {
    HAMMING = 0,
    BLACKMAN,
    KAISER,
};

/*
    4 type filter init and uninit
    note: the fc is the normalized frequency according the fmax, fmax = 0.5*fs
*/
uintptr_t fa_fir_filter_lpf_init(int frame_len, 
                                 int flt_len, float fc, win_t win_type);

uintptr_t fa_fir_filter_hpf_init(int frame_len, 
                                 int flt_len, float fc, win_t win_type);

uintptr_t fa_fir_filter_bandpass_init(int frame_len, 
                                      int flt_len, float fc1, float fc2, win_t win_type);

uintptr_t fa_fir_filter_bandstop_init(int frame_len, 
                                      int flt_len, float fc1, float fc2, win_t win_type);

void      fa_fir_filter_uninit(uintptr_t handle);


/*
    main function, do fir filting
*/
int fa_fir_filter(uintptr_t handle, float *buf_in, float *buf_out, int frame_len);

int fa_fir_filter_flush(uintptr_t handle, float *buf_out);

/*
    three main ascending cos window
*/

int   fa_hamming(float *w,const int N);

int   fa_blackman(float *w,const int N);

int   fa_kaiser(float *w, const int N);
int   fa_kaiser_beta(float *w, const int N, const float beta);
float fa_kaiser_atten2beta(float atten);

/*
    below are the utils which will be used in fir, and they maybe used seperatly, so
    I place them here
*/
int fa_hamming_cof_num(float ftrans);

int fa_blackman_cof_num(float ftrans);

int fa_kaiser_cof_num(float ftrans, float atten);

/*
    these below 5 functions, you can use them directly to get the filter coffs,
    WARN: the **h is dynamic be genearted and be malloced, you MUST free this 
    data memory when you no need use
*/
int fa_fir_lpf_cof(float **h, int N, float fc, win_t win_type);

int fa_fir_hpf_cof(float **h, int N, float fc, win_t win_type);

int fa_fir_bandpass_cof(float **h, int N, float fc1, float fc2, win_t win_type);

int fa_fir_bandstop_cof(float **h, int N, float fc1, float fc2, win_t win_type);

float fa_conv(const float *x, const float *h, int h_len);

#ifdef __cplusplus 
}
#endif  


#endif
