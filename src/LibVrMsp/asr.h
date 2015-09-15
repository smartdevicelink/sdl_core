#ifndef ASR_H
#define ASR_H

#include "include/msp_types.h"
#include "lib_msp_vr.h"


//#include "asr_global.h"
extern void write_log(const char *format,...);

typedef unsigned int MSP_ERROR_CODE;
typedef enum
{
    ASR_IAT=0,
    ASR_VR,
    ASR_TTS
}ASRType;

#define   MSP_SPEEKER_COMPLETE     0x80000000
#define   MSP_SPEEKER_HAS_RESULT   0x00000001
#define   MSP_SPEEKER_RESULT_FALSE 0x00000010
#define   MSP_SPEEKER_TIME_OUT     0x00000100

typedef int SR_DWORD;
typedef short int SR_WORD ;

extern int asr_file_buffer(char **pPCM,char *asrfile);
extern void asr_write_buffer(const char* rec_result,int len,const char *file);
extern char* get_current_path();

#ifdef ASR_HAS_VR
extern void construct_grammar(GrammarCMD   appCmd[], int num,int cancelid, int quitId);

extern bool  grammar_has_construct();

void release_grammar();


extern MSP_ERROR_CODE ASRWriteAudioBuffer(const char buffer[],int buffer_len=6400,int audStat=MSP_AUDIO_SAMPLE_CONTINUE);

//vr configure 
extern void ASRInitConfig();
extern void ASRAppIdConfig(const char *_appid);
extern void ASRParmConfig(const char *_parm);
void ASRUserNameConfig(const char *_user_name);
void ASRUserPasswordConfig(const char *_user_pwd);
void ASRConfidenceConfig(int confidence);
void ASRGrammarCancelConfig(char *_cancel);
void ASRGrammarQuitConfig(char *_quit);

extern MSP_ERROR_CODE  ASRRegisterUser();
extern void ASRUnRegisterUser();
extern MSP_ERROR_CODE  ASRBeginSession();


extern MSP_ERROR_CODE  ASRGetResult(char cmdIdStr[],char out_text[],int max,char data[],int len);


//extern void ASRInitConfig(const char *_login_config,const char *_session_param,const char *_grammar_file);


//extern MSP_ERROR_CODE  ASRGetResult(const char *wav_file,char out_text[],int max);
int getVRResult(char out_text[], char out_cmdId[], int max, char in_result[]);
int getResult(char out_text[], int max, const char result[], const char*  search);
#endif


int indexOfString(const char* sstr,const char str[],int len);


#endif // ASR_H
