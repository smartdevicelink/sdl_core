#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#include "asr.h"
#include "lib_msp_vr.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>

#ifdef OS_WIN32
//#include <conio.h>
#include <Windows.h>
#ifndef OS_WINCE
#include <direct.h>
#endif
HINSTANCE  lib_FUNC;

#else
#include <string.h>
#include <unistd.h>
#endif////os_win32


//from iflytek

#ifdef ASR_HAS_VR
#include "include/qisr.h"
#include "include/msp_cmn.h"
#include "include/msp_errors.h"
#include "include/qtts.h"
#define  LOADDLL

#ifdef LOADDLL
Proc_MSPUploadData uploadData;
Proc_QISRAudioWrite audioWrite;
Proc_MSPLogin login;
Proc_MSPLogout logout;
Proc_QISRSessionBegin sessionBegin;
Proc_QISRGrammarActivate grammarActivate;
//Proc_QISRAudioWrite audioWriter;
Proc_QISRGetResult getAudioResult;
Proc_QISRSessionEnd sessionEnd;
#endif


#define  		BUFFER_NUM  		    4096
#define 		MAX_KEYWORD_LEN 		4096
#define 		MAX_INFO_SIZE     		120
#define  		MAX_GRAMMAR_SIZE		(1024*10)



ASRType   session_type;
char GrammarID[128];
char login_config[MAX_INFO_SIZE];
char session_param[MAX_INFO_SIZE];
char grammar_file[MAX_INFO_SIZE];
const char *sessionID=NULL;
char asr_grammar[MAX_GRAMMAR_SIZE];
char asr_grammar_cancel[MAX_CMD_SIZE];
char asr_grammar_quit[MAX_CMD_SIZE];


int  vr_confidence;

char vr_user_name[MAX_INFO_SIZE];
char vr_user_password[MAX_INFO_SIZE];
#endif
#define 		MAX_PATH_SIZE     		120
char current_path[MAX_PATH_SIZE]={0};
extern int vr_local_state;
extern int vr_run_mode;

void write_log(const char *format,...)
{
	if (vr_run_mode == ASR_RUN_RELEASE){
        return;
    }
	/*if (_msp_log == NULL)
		return;*/
    char buf[1000]={0};
    va_list  ap;
//    int retval;
    va_start(ap,format);
	vsprintf(buf,format,ap);
    va_end(ap);
    strcat(buf,"\n");
	if (_msp_log==NULL)
		printf("%s",buf);
	else
		_msp_log(buf);
}



char* get_current_path()
{
    memset(current_path,0,MAX_PATH_SIZE);
#ifdef OS_WIN32
#ifndef OS_WINCE
    _chdir("release/");
    _getcwd(current_path,MAX_PATH_SIZE);
#endif
#else
#ifdef OS_ANDROID
	
#else
		getcwd(current_path,MAX_PATH_SIZE);
#endif
#endif
    return current_path;
}


int asr_file_buffer(char **pPCM,char *asrfile)
{
    FILE *f_pcm = NULL;
    int pcmSize=0;
    f_pcm=fopen(asrfile, "rb");
    if (NULL != f_pcm) {
            fseek(f_pcm, 0, SEEK_END);
            pcmSize = ftell(f_pcm);
            fseek(f_pcm, 0, SEEK_SET);
            *pPCM = (char *)malloc(pcmSize);
            pcmSize=fread((void *)*pPCM, pcmSize, 1, f_pcm);
            fclose(f_pcm);
            f_pcm = NULL;
    }
    return pcmSize;
}

void asr_write_buffer(const char* result,int len,const char *file)
{
    FILE* result_txt = NULL;
    if( NULL != result )
    {
        //printText(QString().sprintf( "got a result: %s\n", rec_result ));
        result_txt = fopen(file, "at+");
        if (result_txt)
        {
            fwrite(result, 1,len, result_txt);
            fclose(result_txt);
        }
    }
}

#ifdef ASR_HAS_VR
void ASRGrammarCancelConfig(char *_cancel)
{
	if (_cancel == NULL)
		return;
	memset(asr_grammar_cancel, 0, MAX_CMD_SIZE);
	strcpy(asr_grammar_cancel, _cancel);
}

void ASRGrammarQuitConfig(char *_quit)
{
	if (_quit == NULL)
		return;
	memset(asr_grammar_quit, 0, MAX_CMD_SIZE);
	strcpy(asr_grammar_quit, _quit);
}



void construct_grammar(GrammarCMD   appCmd[],int num,int cancelid,int quitId)//
{
	memset((void*)asr_grammar, 0, MAX_GRAMMAR_SIZE);
	if (num != 0)
	{
		sprintf(asr_grammar,
			"#ABNF 1.0 GB2312;\n"
			"language zh-CN;\n"
			"mode voice;\n"
			"root $main;\n"
			"$main = $sdl_command;\n"
			"$sdl_command=$vr_cancel{out.cmdId=%d;}|$vr_quit{out.cmdId=%d;}|$vr_app;\n"
			"$vr_cancel=%s;\n"
			"$vr_quit=$vr_exit;\n"
			"$vr_exit=%s;\n"
			"$vr_app=%s{out.cmdId=%d;}"
			,cancelid, quitId,asr_grammar_cancel,asr_grammar_quit,appCmd[0].cmd, appCmd[0].cmdId);
		for (int i = 1; i < num; i++)
		{
			char grammar[100] = { 0 };
			sprintf(grammar, "\n|%s{out.cmdId=%d;}", appCmd[i].cmd, appCmd[i].cmdId);
			strcat(asr_grammar, grammar);
		}
		strcat(asr_grammar, ";\n");
	}
	else
	{
		sprintf(asr_grammar,
			"#ABNF 1.0 GB2312;\n"
			"language zh-CN;\n"
			"mode voice;\n"
			"root $main;\n"
			"$main = $sdl_command;\n"
			"$sdl_command=$vr_cancel{out.cmdId=%d;}|$vr_quit{out.cmdId=%d;};\n"
			"$vr_cancel=%s;\n"
			"$vr_quit=$vr_exit;\n"
			"$vr_exit=%s;\n"
			,cancelid,quitId,asr_grammar_cancel,asr_grammar_quit);//, helpid, cancelid, quitId

	}
	printf("%s\n", asr_grammar);
}


bool  grammar_has_construct()
{
	return (asr_grammar[0] != '\0');
}



void ASRRegisterDLLFuncs()
{
#ifdef LOADDLL
#ifdef OS_WIN32
	lib_FUNC = LoadLibrary("msc.dll");
	if (lib_FUNC == NULL)
	{
		MSP_Printf("msc.dll not found");
		return;
	}
	uploadData = (Proc_MSPUploadData)GetProcAddress(lib_FUNC, "MSPUploadData");
	if (uploadData == NULL)
	{
		MSP_Printf("MSPUploadData not found");
		return;
	}
	audioWrite = (Proc_QISRAudioWrite)GetProcAddress(lib_FUNC, "QISRAudioWrite");
	if (audioWrite == NULL)
	{
		MSP_Printf("QISRAudioWrite not found");
		return;
	}

	login = (Proc_MSPLogin)GetProcAddress(lib_FUNC, "MSPLogin");
	if (login == NULL)
	{
		MSP_Printf("MSPLogin not found");
		return ;
	}

	logout = (Proc_MSPLogout)GetProcAddress(lib_FUNC, "MSPLogout");
	if (logout == NULL)
	{
		MSP_Printf("MSPLogout not found");
		return;
	}

	sessionBegin = (Proc_QISRSessionBegin)GetProcAddress(lib_FUNC, "QISRSessionBegin");
	if (sessionBegin == NULL)
	{
		MSP_Printf("QISRSessionBegin not found");
		return ;
	}

	grammarActivate = (Proc_QISRGrammarActivate)GetProcAddress(lib_FUNC, "QISRGrammarActivate");
	if (grammarActivate == NULL)
	{
		MSP_Printf("QISRGrammarActivate not found");
		return ;
	}
	getAudioResult = (Proc_QISRGetResult)GetProcAddress(lib_FUNC, "QISRGetResult");
	if (getAudioResult == NULL)
	{
		MSP_Printf("QISRGetResult not found");
		return ;
	}

	sessionEnd = (Proc_QISRSessionEnd)GetProcAddress(lib_FUNC, "QISRSessionEnd");
	if (sessionEnd == NULL)
	{
		MSP_Printf("QISRSessionEnd not found");
		return ;
	}
#endif
#endif

}




MSP_ERROR_CODE ASRWriteAudioBuffer(const char buffer[],int buffer_len,int audStat)
{
    MSP_ERROR_CODE ret=0;
    int epStatus = MSP_EP_LOOKING_FOR_SPEECH;
    int recStatus = MSP_REC_STATUS_SUCCESS ;

    if(buffer_len<=0)
        return MSP_SPEEKER_COMPLETE;

    MSP_Printf("csid=%s,write len=%d,aus=%d,",sessionID,buffer_len,audStat);
#ifdef LOADDLL
	ret = audioWrite(sessionID, (const void *)buffer, buffer_len, audStat, &epStatus, &recStatus);//
#else
    ret = QISRAudioWrite(sessionID, (const void *)buffer, buffer_len, audStat, &epStatus, &recStatus);//
#endif
    MSP_Printf("eps=%d,rss=%d,ret=%d\n",epStatus,recStatus,ret);

    if (recStatus == MSP_REC_STATUS_SUCCESS) {
         ret=MSP_SPEEKER_HAS_RESULT;
//         int recStatus=0,errCode=0;
//         const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &errCode);//
//         if (NULL != rslt)
//            strcat(rec_result,rslt);
      }
     if (epStatus == MSP_EP_AFTER_SPEECH)
        ret=ret|MSP_SPEEKER_COMPLETE;
    return ret;
}

void ASRUserNameConfig(const char *_user_name)
{
	if (_user_name != NULL)
	{
		msp_vr_login(false);
		memset(vr_user_name, 0, MAX_INFO_SIZE);//=_login_config;
		strcpy(vr_user_name, _user_name);
	}
}

void ASRUserPasswordConfig(const char *_user_pwd)
{
	if (_user_pwd != NULL)
	{
		msp_vr_login(false);
		memset(vr_user_password, 0, MAX_INFO_SIZE);
		strcpy(vr_user_password, _user_pwd);
	}
}

void ASRAppIdConfig(const char *_appid)
{
	if (_appid != NULL)
	{
		msp_vr_login(false);
		char buf[50] = { 0 };
		sprintf(buf, "appid = %s, work_dir = .  ", _appid);
		memset(login_config, 0, MAX_INFO_SIZE);//=_login_config;
		strcpy(login_config, buf);
		MSP_Printf("login_config=%s",login_config);
	}
}


void ASRParmConfig(const char *_parm)
{
	if (_parm != NULL)
	{
		memset(session_param, 0, MAX_INFO_SIZE);
		strcpy(session_param, _parm);
		//       MSP_Printf("session_parm=%s",session_param);
	}
}


void ASRConfidenceConfig(int confidence)
{
	vr_confidence = confidence;
}



void  ASRInitConfig()
{
	session_type = ASR_VR;
	memset((void*)asr_grammar, 0, MAX_GRAMMAR_SIZE);

	msp_vr_login(false);
	msp_vr_upload(false);

#ifdef LOADDLL
	ASRRegisterDLLFuncs();
#endif
    sessionID=NULL;

}



MSP_ERROR_CODE  ASRRegisterUser()
{
    int ret=0;
    if(!msp_vr_is_login())
    {
#ifdef LOADDLL
		ret = login(vr_user_name, vr_user_password, login_config);
#else
        ret = MSPLogin(NULL, NULL, login_config);//
#endif
		if ( ret != MSP_SUCCESS )
        {
                MSP_Printf("MSPLogin failed , Error code %d.\n",ret);
				ASRUnRegisterUser();
                //asr_is_login=false;
				msp_vr_login(false);
                return ret;
        }
        //asr_is_login=true;
		msp_vr_login(true);
    }

	if (asr_grammar[0] != '\0')
		printf("%s\n", asr_grammar);
	else
	{
		printf("grammar is empty\n");
	}
     return ret;
}

void ASRUnRegisterUser()
{
	if (sessionID != NULL){
#ifdef LOADDLL
		sessionEnd(sessionID, NULL);
#else
		QISRSessionEnd(sessionID, NULL);
#endif
		sessionID = NULL;
	}

	if(msp_vr_is_login())
    {
#ifdef LOADDLL
		logout();

#else
        MSPLogout();
#endif
		msp_vr_login(false);
    }
}


MSP_ERROR_CODE  ASRBeginSession()
{

    int errCode=0;
	errCode = ASRRegisterUser();
    session_type=ASR_VR;

    MSP_Printf("RegisterUser code=%d",errCode);
	sessionID = NULL;
	if (errCode==0)
	{
		if (session_type != ASR_IAT)
		{
#ifdef LOADDLL
			sessionID = sessionBegin(NULL, session_param, &errCode); //asr

#else
			sessionID = QISRSessionBegin(NULL, session_param, &errCode); //asr
#endif

#ifdef LOADDLL
			grammarActivate(sessionID, asr_grammar, NULL, 0); //asr

#else
			QISRGrammarActivate(sessionID, asr_grammar, NULL, 0);
#endif

		}
		else
		{
#ifdef LOADDLL

			sessionID=sessionBegin(NULL, session_param, &errCode); //asr
			//Win32_Func(sessionID,QISRSessionBegin,NULL, _session_param, &errCode)
#else
			sessionID = QISRSessionBegin(NULL, session_param, &errCode); //asr
#endif
		}
	}
    
       
    if(errCode!=0)
    {
        ASRUnRegisterUser();
        MSP_Printf("begin session err=%d",errCode);
        return errCode;
    }
    //-------------------------------------------------
    //memset(rec_result,0,sizeof(rec_result));
    return errCode;
}





MSP_ERROR_CODE  ASRGetResult(char cmdIdStr[],char out_text[],int max,char data[],int len)
{
    int ret = MSP_SUCCESS;
    int epStatus = MSP_EP_LOOKING_FOR_SPEECH;
    int recStatus = MSP_REC_STATUS_SUCCESS ;
    char result[1024] = {0};

#ifdef LOADDLL
	
	ret=audioWrite(sessionID, (const void*)data, len, MSP_AUDIO_SAMPLE_LAST, &epStatus, &recStatus); //
#else
	ret=QISRAudioWrite(sessionID, (const void*)data, len, MSP_AUDIO_SAMPLE_LAST, &epStatus, &recStatus);
#endif
    MSP_Printf("eps=%d,rss=%d,ret=%d\n",epStatus,recStatus,ret);
	int time = 0;
    while (recStatus != MSP_REC_STATUS_COMPLETE && ret==0) {

		const char *rslt = NULL;
#ifdef LOADDLL
		rslt = getAudioResult(sessionID, &recStatus, 0, &ret);
#else
	   rslt = QISRGetResult(sessionID,&recStatus, 0, &ret);
#endif
       if (NULL != rslt)
       {
           int len=strlen(rslt);
           max=max-len;
           if(max<0)
               break;
           strcat(result,rslt);
       }
       else
	   {
#ifdef OS_WIN32
          Sleep(150);
#else
		  usleep(150000);
#endif
	   }
	   if (vr_local_state == MSP_LOCAL_FORCE_QUIT)
		   return -5;
    }
	printf("%s", result);
	if (ret == 0)
	{
		return getVRResult(out_text, cmdIdStr, max, result);
	}   
    return ret;
}


int getVRResult(char out_text[], char out_cmdId[], int max,char in_result[])
{
	if (in_result[0] == '\0')
		return MSP_SPEEKER_RESULT_FALSE;
	int text_ind = indexOfString(in_result, "input=", strlen("input="));
	int id_ind = indexOfString(in_result, "cmdId=", strlen("cmdId="));
	int conf_id = indexOfString(in_result, "confidence=", strlen("confidence="));
	if (text_ind == -1 || id_ind == -1||conf_id==-1)
		return MSP_SPEEKER_RESULT_FALSE;
	//fetch confidence
	int out = 0;
	char confidence[20] = {0};
	while (in_result[conf_id] != ' ')
	{
		confidence[out++] = in_result[conf_id++];
		if (out >= 19)
			break;
	}
	confidence[out] = '\0';
	int confi = atoi(confidence);
	printf(confidence);
	if (confi < vr_confidence)
		return MSP_SPEEKER_RESULT_FALSE;

	//fetch cmdId
	out = 0;
	while (in_result[id_ind] != ' ')
	{
		out_cmdId[out++] = in_result[id_ind++];
		if (out >= max-1)
			break;
	}
	out_cmdId[out] = '\0';

	//fetch text
	out = 0;
	while (in_result[text_ind] != '\0'&&in_result[text_ind] != '\n')
	{
		out_text[out++] = in_result[text_ind++];
		if (out >= max-1)
			break;
	}
	out_text[out] = '\0';
	return 0;
}



int getResult(char out_text[],int max,const char result[],const char*  search)
{
	
	if (result[0] == '\0')
		return MSP_SPEEKER_RESULT_FALSE;
    if(session_type==ASR_VR)
    {
        int index=indexOfString(result,search,strlen(search));
        if(index<=0||index>=200)
			return MSP_SPEEKER_RESULT_FALSE;
        int out=0;
        while(result[index]!='\0'&&result[index]!='\n'&&result[index]!=' ')
        {
            out_text[out++]=result[index++];
            if(out>=max-1)
                break;
        }
        out_text[out]='\0';
		return 0;
    }
    else if(session_type==ASR_IAT)
    {
        strcpy(out_text,result);
		return 0;
    }
	return 0;
}
#endif

int indexOfString(const char* sstr,const char *str,int len)
{
    if(sstr==NULL||len<=0)
        return (-1);
    int index=0;
    while(sstr[index]!='\0')
    {
        if(sstr[index]==str[0])
        {
            int idx=1;
            bool search=true;
            while(idx<len)
            {
                if(sstr[index+idx]!=str[idx])
                {
                    search=false;
                    break;
                }
                idx++;
            }
            if(search)
                return (index+len);
        }
        index++;
    }
    return (-1);
}



