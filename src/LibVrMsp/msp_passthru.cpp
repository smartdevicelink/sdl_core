#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#include "lib_msp_vr.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "asr.h"

#ifndef  ASR_HAS_VR
#include <include/msp_errors.h>
#endif

#include "resample.h"

#include <ctype.h>
#include <pthread.h>
#include "msp_type.h"
#include <vector>

#ifdef OS_WIN32
#include <Windows.h> 
#endif

pthread_t   pass_pthread_;

passthru_callback   passthru_callback_;

//wav header
typedef struct __wave_pcm_hdr
{
	char            riff[4];                        // = "RIFF"
	SR_DWORD        size_8;                         // = FileSize - 8
	char            wave[4];                        // = "WAVE"
	char            fmt[4];                         // = "fmt "
	SR_DWORD        dwFmtSize;                      // = next struct size : 16

	SR_WORD         format_tag;              // = PCM : 1
	SR_WORD         channels;                       // = channel: 1
	SR_DWORD        samples_per_sec;        // = sample rate : 8000 |11025 | 16000|22050|44100
	SR_DWORD        avg_bytes_per_sec;      // = bytes per second : dwSamplesPerSec * wBitsPerSample / 8
	SR_WORD         block_align;            // = bytes per sample point : wBitsPerSample / 8
	SR_WORD         bits_per_sample;         // = bits per sample: 8 | 16

	char            data[4];                        // = "data";
	SR_DWORD        data_size;                // = data length outclude header : FileSize - 44 
}wave_pcm_hdr;


void msp_passthru_send(char *data, int len)
{
	if (len <= 0||data==NULL||passthru_callback_==NULL)
		return;
	passthru_callback_(data, len);
}

void msp_passthru_loop()
{
	int state = msp_vr_state();
	while (state != MSP_RECORD_END && state != MSP_RECORD_TERMINATE)
	{
		MSP_Printf("please say...");
#ifdef OS_WIN32
		Sleep(250);
#else
		usleep(250000);
#endif
		if (state == MSP_RECORD_WAIT)
		{
			char *buffer=NULL;
			int len = 0;
			if (msp_vr_download_data(&buffer, &len)){
				msp_passthru_send(buffer, len);
				free(buffer);
			}
		}
		else
		{
			MSP_Printf("vr not start");
		}
		state = msp_vr_state();

	}
	char *buffer=NULL;
	int len = 0;
	
	if (msp_vr_download_data(&buffer, &len)){
		if (len == 0 || buffer == NULL)
			return;
		msp_passthru_send(buffer, len);
		free(buffer);
	}
}
void* msp_passthru_thread(void *)
{
	msp_vr_set_state(MSP_RECORD_START);
	msp_passthru_loop();
	return NULL;
}

bool msp_passthru_start(passthru_callback callback)
{
	passthru_callback_ = callback;
	
	if (0 != pthread_create(&pass_pthread_, 0, &msp_passthru_thread, NULL)) {
		MSP_Printf("Create thread fail.");
		msp_vr_set_state(MSP_RECORD_END);
		return false;
	}
	
	return true;
}

void msp_passthru_stop()
{
	msp_vr_set_state(MSP_RECORD_END);
	pthread_join(pass_pthread_, NULL);
	MSP_Printf("passthru stop");
}


void   soxr_wav_convert_init(int in_rate, int out_rate, int bits)
{
	//fa_wavfmt_t  fmt = fa_wavfmt_setheader(in_rate, bits);
	int soxr_convert_byte = (bits + 8 - 1) / 8;
	fa_resample_filter_init(out_rate, in_rate, soxr_convert_byte);
}

void soxr_wav_convert_destroy()
{
}

int soxr_wav_ratio_convert(int in_rate, int out_rate, int bits, char *ibuf, int ilen, char *obuf, int*  olen)
{
	int ret = 0;
	fa_resample_set(ilen, out_rate, in_rate);
	int in_len_bytes = ilen;
	printf("in_len=%d\n", in_len_bytes);

	//resample
	int out_len = 0;
	if (-1 == fa_resample( (unsigned char*)ibuf, in_len_bytes, (unsigned char*)obuf, &out_len))
		return -1;
	*olen = out_len;
	return ret;
}


int soxr_convert_file(char *input, char *output, int out_rate)
{
	FILE* pfin = fopen(input, "r+b");
	if (pfin == NULL)
	{
		return -1;
	}
	FILE* pfout = fopen(output, "w+b");
	if (pfout == NULL)
	{
		return -1;
	}
	wave_pcm_hdr   pcm_hdr;

	fread(&pcm_hdr, 1, sizeof(wave_pcm_hdr), pfin);
	int in_rate = pcm_hdr.samples_per_sec;
	int read_len = in_rate*pcm_hdr.block_align / 50;
	pcm_hdr.samples_per_sec = out_rate;
	pcm_hdr.avg_bytes_per_sec = out_rate*pcm_hdr.block_align;
	fwrite(&pcm_hdr, 1, sizeof(wave_pcm_hdr), pfout);
	soxr_wav_convert_init(in_rate, out_rate, pcm_hdr.bits_per_sample);

	fseek(pfin, 44, SEEK_SET);
	fseek(pfout, 44, SEEK_SET);
	int total_size = 0;
	int ret = 0;
	while (!feof(pfin))
	{
		char buf[3000] = { 0 };
		char obuf[3000] = { 0 };
		int olen = 0;
		//fgets(buf,(pcm_hdr.avg_bytes_per_sec/50),pfin);
		int ilen = fread(buf, 1, read_len, pfin);
		if (ilen <= 0){
			ret = -1;
			break;
		}
		ret = soxr_wav_ratio_convert(in_rate, out_rate, pcm_hdr.bits_per_sample, buf, ilen, obuf, &olen);
		if (-1 == ret)
			break;
		fwrite(obuf, 1, olen, pfout);
		total_size += olen;
	}
	pcm_hdr.data_size = total_size;
	pcm_hdr.size_8 = pcm_hdr.data_size + 44;
	fseek(pfout, 0, SEEK_SET);
	fwrite(&pcm_hdr, 1, sizeof(pcm_hdr), pfout);
	fclose(pfin);
	fclose(pfout);
	return ret;
}


PCM_HANDLE  soxr_pcm_open(char *name)
{
	FILE* pfout = fopen(name, "w+b");
	if (pfout == NULL)
		return NULL;
	fseek(pfout, 44, SEEK_SET);
	return (PCM_HANDLE)pfout;
}

void  soxr_pcm_close(PCM_HANDLE pHandle,int data_len,int sample_rate,int sample_size)
{
	FILE* pfout = (FILE*)pHandle;
	if (pfout == NULL)
		return;
	fseek(pfout, 0, SEEK_SET);
	wave_pcm_hdr default_pcmwavhdr =
	{
		{ 'R', 'I', 'F', 'F' },
		data_len + 44,
		{ 'W', 'A', 'V', 'E' },
		{ 'f', 'm', 't', ' ' },
		16,
		1,
		1,
		sample_rate,
		sample_rate*sample_size / 8,
		(short int)(sample_size / 8),
		(short int)sample_size,
		{ 'd', 'a', 't', 'a' },
		data_len
	};

	fwrite(&default_pcmwavhdr, 1, sizeof(default_pcmwavhdr), pfout);
	fclose(pfout);
}

void soxr_pcm_write(PCM_HANDLE pHandle ,char *buffer, int len)
{
	FILE* pfout = (FILE*)pHandle;
	if (pfout == NULL)
		return;
	fwrite(buffer, 1, len, pfout);
}

//pcm to wav

bool soxr_pcm_to_wav(const std::vector<unsigned char> &pcm_data, int sample_rate, int sample_size,std::vector<unsigned char> &wav_data)
{
	if (pcm_data.size()==0)
		return false;
	int length = pcm_data.size();
	wave_pcm_hdr default_pcmwavhdr =
	{
		{ 'R', 'I', 'F', 'F' },
		length + 44,
		{ 'W', 'A', 'V', 'E' },
		{ 'f', 'm', 't', ' ' },
		16,
		1,
		1,
		sample_rate,
		sample_rate*sample_size / 8,
		(short int)(sample_size / 8),
		(short int)sample_size,
		{ 'd', 'a', 't', 'a' },
		length
	};
	wav_data.resize(sizeof(default_pcmwavhdr)+pcm_data.size());
#ifdef OS_WINCE
	memcpy(&wav_data[0], (char*)&default_pcmwavhdr, sizeof(default_pcmwavhdr));
	memcpy(&wav_data[0] + sizeof(default_pcmwavhdr), &pcm_data[0], pcm_data.size());
#else
	memcpy(wav_data.data(), (char*)&default_pcmwavhdr, sizeof(default_pcmwavhdr));
	memcpy(wav_data.data() + sizeof(default_pcmwavhdr), pcm_data.data(), pcm_data.size());
#endif
	return true;
}

bool soxr_wav_to_pcm(const std::vector<unsigned char> &wav_data ,std::vector<unsigned char> &pcm_data)
{
	if (wav_data.size() == 0)
		return false;
	pcm_data.resize(wav_data.size() - sizeof(wave_pcm_hdr));
#ifdef OS_WINCE
	memcpy(&pcm_data[0], &wav_data.at(sizeof(wave_pcm_hdr)), wav_data.size() - sizeof(wave_pcm_hdr));
#else
	memcpy(pcm_data.data(), &wav_data.at(sizeof(wave_pcm_hdr)), wav_data.size() - sizeof(wave_pcm_hdr));
#endif
	return true;
}






