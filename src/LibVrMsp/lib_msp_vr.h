#ifndef LIB_MSP_VR_H
#define LIB_MSP_VR_H

#define  ASR_RELEASE

#include <vector>

#define   RECORD_HAS_FILE    0
#define   RECORD_TEST        0

#define  ASR_RUN_RELEASE 1
#define  ASR_RUN_DEBUG   2




typedef void(*msp_log)(char buf[]);
typedef void(*MSP_CALLBACK_PFUNC)(void *data,int cmdId, char text[], int error);
extern msp_log    _msp_log;
#ifdef ASR_RELEASE
#define  MSP_Printf    write_log
#else
#define  MSP_Printf    printf
#endif
void msp_log_mode(int mode);

#ifdef   ASR_HAS_VR
typedef void ( *Proc_msp_void_func)();
typedef int ( *Proc_msp_int_return_func)();
typedef int ( *Proc_msp_pchar_func)(char*,int);
typedef void ( *Proc_msp_int_func)(int);
typedef char* ( *Proc_msp_pchar_return_func)();
#endif

void msp_vr_register_log_callback(msp_log  logFun);
void   msp_vr_init(void *data,MSP_CALLBACK_PFUNC  callback);

#ifdef   ASR_HAS_VR
void msp_vr_send_result(int cmdId, char result[], int error);
void msp_vr_register_callback(void *data,MSP_CALLBACK_PFUNC  pCallbackFunc);
void* msp_vr_thread(void *);
bool msp_vr_start();
void msp_vr_terminate();
void msp_vr_reset();


bool msp_vr_get_config();
int   msp_vr_begin();
int   msp_vr_upload_buffer();
int   msp_vr_result(int *cmdId, char result[], int max);
#endif

int   msp_vr_state();
bool  msp_vr_set_state(int state);
bool  msp_vr_upload_data(char *data,int len);
bool msp_vr_download_data(char **dest, int *len);
bool msp_mem_data_address(char **dest, int *len);

bool msp_vr_is_login();
void msp_vr_login(bool login);

bool msp_vr_is_upload();
void msp_vr_upload(bool load);

extern char* get_current_path();

//passthru
typedef void(*passthru_callback)(char *data, int len);
bool msp_passthru_start(passthru_callback callback);
void msp_passthru_stop();

//soxr ratio
typedef int *  PCM_HANDLE;
void soxr_wav_convert_init(int in_rate, int out_rate, int bits);
void soxr_wav_convert_destroy();
int soxr_wav_ratio_convert(int in_rate, int out_rate, int bits, char *ibuf, int ilen, char *obuf, int*  olen);
int soxr_convert_file(char *input, char *output, int out_rate);
PCM_HANDLE  soxr_pcm_open(char *name);
void  soxr_pcm_close(PCM_HANDLE pHandle, int data_len, int sample_rate, int sample_size);
void soxr_pcm_write(PCM_HANDLE pHandle, char *buffer, int len);
//pcm to wav 
bool soxr_pcm_to_wav(const std::vector<unsigned char> &pcm_data, int sample_rate, int sample_size, std::vector<unsigned char> &wav_data);
//wav to pcm
bool soxr_wav_to_pcm(const std::vector<unsigned char> &wav_data, std::vector<unsigned char> &pcm_data);

//VR TYPE
#define MSP_VR_APP      1
#define MSP_VR_PASSTHRU   2



//share mem type
#define  MSP_MEM_SERVER    1
#define  MSP_MEM_CLIENT    2


//record state
#define  MSP_RECORD_INIT  0
#define  MSP_RECORD_START  1
#define  MSP_RECORD_WAIT  2
#define  MSP_RECORD_END  3
#define  MSP_RECORD_TERMINATE  4

//local vr state
#define MSP_LOCAL_NORMAL_INIT   0
#define MSP_LOCAL_NORMAL_RUN   1
#define MSP_LOCAL_NORMAL_END   2
#define MSP_LOCAL_FORCE_QUIT	4


//share memory operator
bool msp_mem_assert();
void  msp_mem_init(int mode);
void msp_mem_destroy();
void msp_mem_data_init();

bool  msp_mem_state_write(int state);
int  msp_mem_state_read();
/*
vr parm set
*/
void msp_vrparm_set(int type,int max_duration, int sampling_rate,
	int bits_per_sample, int audio_type);

/*
vr parm get
*/
bool msp_vrparm_get(int *type,int *max_duration, int *sampling_rate,
	int *bits_per_sample, int *audio_type);


#define   MAX_CMD_SIZE     500
#define   MAX_CMD_COUNT    1000
//
typedef struct __GrammarCMD
{
	char  cmd[MAX_CMD_SIZE];
	int   cmdId;
}GrammarCMD;
void msp_vr_grammar_trim(const char cmd[], char out[], int len);
void msp_vr_grammar_construct(int cancelId,int exitId); 
void msp_vr_grammar_addcmd(char cmd[],int cmdid);
void msp_vr_grammar_deletecmd(int cmdId);

//void msp_utf8_to_ansi(std::string  out,std::string  in);

void StartVRMsp();
bool IsProcessRun(char *pName);


//#ifdef __cplusplus
//} /* extern "C" */
//#endif /* C++ */
#endif // LIB_MSP_VR_H
