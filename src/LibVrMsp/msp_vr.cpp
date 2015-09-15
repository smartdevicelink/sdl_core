#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#include "lib_msp_vr.h"
#include <pthread.h>

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "asr.h"

#ifdef  ASR_HAS_VR
#include <include/msp_errors.h>
#endif

#ifdef OS_WIN32
#include <Windows.h>
#else
#include <string.h>
#endif

#include "msp_type.h"



int vr_local_state;
int vr_local_time;
extern int vr_run_mode;


#ifdef ASR_HAS_VR
pthread_t thread_;
pthread_mutex_t  mutex_ = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  judge_mutex_ = PTHREAD_MUTEX_INITIALIZER;
void*               call_object;
MSP_CALLBACK_PFUNC  result_callback;
GrammarCMD   grammarCmds[MAX_CMD_COUNT];
int          grammarIndex;
bool         grammarHasnew;
#endif


#ifdef ASR_HAS_VR
int msp_get_config_cstr(char *config_cstr, char result[])
{
	int pi = 0, pt = 0, ps = 0;
	MSP_Printf(config_cstr);
	while (config_cstr[pi] != '\n' && config_cstr[pi] != '\0')
	{
		//printf("pt=%d,pi=%d,config_cstr[%d]=%c\n", pt, pi,pi, config_cstr[pi]);
		if ((config_cstr[pi] == '/'&&config_cstr[pi + 1] == '/'))
			break;
		if (config_cstr[pi] == ' '&&pt == 0)
		{
			pi++;
			continue;
		}
		result[pt++] = config_cstr[pi++];

	}
	//printf("pt=%d\n", pt);
	result[pt] = 0;
	pt = pt - 1;
	//trim
	while (pt >= 0)
	{
		//printf("result[%d]=%c", pt,result[pt]);
		if ((result[pt] == ' ' || result[pt] == ';'))
			result[pt--] = '\0';
		else
			break;
	}

	//printf("trim pt=%d\n", pt);
	if (pt <= 0)
		return 0;
	return (pt + 1);
}


bool msp_vr_get_config()
{
	FILE  *fp = fopen("msp_vr.ini", "r");
	if (fp == NULL)
	{
		MSP_Printf("msp_vr.ini open failed");
		return false;
	}

	while (!feof(fp))
	{
		char buf[301] = { 0 };
		fgets(buf, 300, fp);
		printf(buf);
		if ((buf[0] != '$'))
			continue;
		int index = -1;
		if ((index = indexOfString(buf, "$VR_RUN_CONFIG=", strlen("$VR_RUN_CONFIG="))) != -1)
		{
			char *config_cstr = &buf[index];
			MSP_Printf(config_cstr);
			if (indexOfString(config_cstr, "debug", strlen("debug")) != -1)
				vr_run_mode = ASR_RUN_DEBUG;
			else
				vr_run_mode = ASR_RUN_RELEASE;
			MSP_Printf("vr_run_mode=%d", vr_run_mode);

		}
		else if ((index = indexOfString(buf, "$VR_USER=", strlen("$VR_USER="))) != -1)
		{
			char config[300] = { 0 };
			if (msp_get_config_cstr(&buf[index], config)){

				if (strcmp(config, "default") != 0)
				{
					ASRUserNameConfig(config);
					MSP_Printf(config);
				}
			}

		}
		else if ((index = indexOfString(buf, "$VR_PASSWORD=", strlen("$VR_PASSWORD="))) != -1)
		{
			char config[300] = { 0 };
			if (msp_get_config_cstr(&buf[index], config)){

				if (strcmp(config, "default") != 0)
				{
					ASRUserNameConfig(config);
					MSP_Printf(config);
				}
			}
		}
		else if ((index = indexOfString(buf, "$VR_PARM=", strlen("$VR_PARM="))) != -1)
		{
			char config[300] = { 0 };
			int len = 0;
			if (len = msp_get_config_cstr(&buf[index], config)){
				config[0] = ' ';
				config[len - 1] = ' ';
				if (strcmp(config, "default") != 0)
				{
					ASRParmConfig(config);
					MSP_Printf(config);
				}
			}
		}
		else if ((index = indexOfString(buf, "$VR_APPID=", strlen("$VR_APPID="))) != -1)
		{
			char config[300] = { 0 };
			int len = 0;
			if (len = msp_get_config_cstr(&buf[index], config)){
				if (strcmp(config, "default") != 0)
				{
					ASRAppIdConfig(config);
					MSP_Printf(config);
				}
			}
		}
		else if ((index = indexOfString(buf, "$vr_confidence=", strlen("$vr_confidence="))) != -1)
		{
			char config[300] = { 0 };
			int len = 0;
			if (len = msp_get_config_cstr(&buf[index], config)){
				if (strcmp(config, "default") != 0)
				{
					ASRConfidenceConfig(atoi(config));
					MSP_Printf(config);
				}
			}
		}
		else if ((index = indexOfString(buf, "$vr_cancle=", strlen("$vr_cancle="))) != -1)
		{
			char config[300] = { 0 };
			int len = 0;
			if (len = msp_get_config_cstr(&buf[index], config)){
				if (strcmp(config, "default") != 0)
				{
					ASRGrammarCancelConfig(config);
					MSP_Printf(config);
				}
			}
		}
		else if ((index = indexOfString(buf, "$vr_quit=", strlen("$vr_quit="))) != -1)
		{
			char config[300] = { 0 };
			int len = 0;
			if (len = msp_get_config_cstr(&buf[index], config)){
				if (strcmp(config, "default") != 0)
				{
					ASRGrammarQuitConfig(config);
					MSP_Printf(config);
				}
			}
		}

	}
	fclose(fp);
	return true;
}
#endif

void  msp_vr_init(void *data, MSP_CALLBACK_PFUNC  pCallbackFunc)
{
	vr_run_mode = ASR_RUN_RELEASE;
#ifdef ASR_HAS_VR
	ASRUserNameConfig("myxuan475@126.com");
	ASRUserPasswordConfig("xf1032075878myx");
	ASRParmConfig("sub=iat,auf=audio/L16;rate=8000,aue=speex,ent=sms8k,rst=plain,rse=gb2312");
	ASRAppIdConfig("541b6ba8");//53f1d450
	ASRConfidenceConfig(33);
	ASRGrammarCancelConfig("取消|cancel!en-us");
	ASRGrammarQuitConfig("退出|离开|(quit|exit)!en-us");
	if (!msp_vr_get_config())
		MSP_Printf("get msp_vr.ini failed");
	ASRInitConfig();
    pthread_mutex_init(&mutex_, NULL);
	pthread_mutex_init(&judge_mutex_, NULL);
	grammarIndex = 0;
	memset(grammarCmds, 0, sizeof(grammarCmds));
	grammarHasnew = false;
#endif

	
	_msp_log = NULL;
	vr_local_state = MSP_LOCAL_NORMAL_INIT;
	vr_local_time = 0;
    
#ifdef ASR_HAS_VR
    result_callback = NULL;
	msp_vr_register_callback(data, pCallbackFunc);
#endif

}



#ifdef ASR_HAS_VR
void msp_vr_grammar_trim(const char cmd[], char out[], int len)
{
	int i = 0;
	int j = 0;
	while (i < len){
		unsigned char dw = (unsigned char)cmd[i];
		//MSP_Printf("cmd[%d]=%u\n", i, dw);
		if (dw >= 0xa1){
			if (dw != 0xa3){//chinese
				out[j++] = cmd[i];
				out[j++] = cmd[i + 1];
			}
			i = i + 2;
		}
		else{//english 
			if ((dw >= 'a'&&dw <= 'z') || (dw >= 'A'&&dw <= 'Z') || (dw >= '0'&&dw <= '9') || dw == ' ' || dw == '(' || dw == ')' || dw == '|' || dw == '!' || dw == '-'){
				out[j++] = cmd[i];
			}
			i = i + 1;

		}

	}
	//MSP_Printf("trim:cmd = %s, out = %s\n", cmd, out);
}

//语法构建
void msp_vr_grammar_construct(int cancelId, int quitId)
{
	if (grammarHasnew || !grammar_has_construct())
		construct_grammar(grammarCmds, grammarIndex, cancelId, quitId);
	grammarHasnew = false;
	grammarIndex = 0;
	memset(grammarCmds, 0, sizeof(grammarCmds));
}

void msp_vr_grammar_addcmd(char cmd[], int cmdId)
{
	if (grammarIndex >= MAX_CMD_COUNT)
	{
		MSP_Printf("cmd id count overflow");
		return;
	}
	/* for (int i = grammarIndex - 1; i >= 0; i--)
	{
	if (grammarCmds[i].cmdId == cmdId)
	{
	if (strcmp(grammarCmds[i].cmd, cmd) == 0)
	return;
	else
	{
	strcpy(grammarCmds[i].cmd, cmd);
	grammarHasnew = true;
	return;
	}

	return;
	}
	}*/
	/*char out[200] = {0};
	msp_vr_grammar_trim(cmd,out,strlen(cmd));*/

	strcpy(grammarCmds[grammarIndex].cmd, cmd);
	grammarCmds[grammarIndex].cmdId = cmdId;
	MSP_Printf("add cmd :index=%d,grammar=%s,id=%d", grammarIndex, cmd, cmdId);
	grammarIndex++;
	grammarHasnew = true;

}

void msp_vr_grammar_deletecmd(int cmdId)
{
	if (grammarIndex == 0)
	{
		MSP_Printf("cmd list is empty");
		return;
	}
	bool  find = false;
	for (int i = grammarIndex - 1; i >= 0; i--)
	{

		if (grammarCmds[i].cmdId == cmdId)
		{
			find = true;
			for (int j = i; j < grammarIndex - 1; j++)
			{
				strcpy(grammarCmds[j].cmd, grammarCmds[j + 1].cmd);
				grammarCmds[j].cmdId = grammarCmds[j + 1].cmdId;

				break;
			}
			grammarIndex--;
			if (find)
				break;

		}
	}
}

void msp_vr_send_result(int cmdId, char result[], int error)
{
	if (result_callback == NULL)
	{
		vr_local_state = MSP_LOCAL_NORMAL_END;
		MSP_Printf("result_callback is null");
		return;
	}
	if (vr_local_state == MSP_LOCAL_FORCE_QUIT)
		result_callback(call_object, cmdId, result, -5);
	else
	    result_callback(call_object, cmdId, result, error);
	vr_local_state = MSP_LOCAL_NORMAL_END;
	printf("msp_vr_send_result:state=normal end\n");
}

void msp_vr_register_callback(void *data, MSP_CALLBACK_PFUNC  pCallbackFunc)
{
	call_object = data;
	result_callback = pCallbackFunc;
}



void msp_vr_record_loop()
{
	int code = msp_vr_begin();
	if (code != 0)
	{
		msp_vr_send_result(0, NULL, code);
		ASRUnRegisterUser();
		return;
	}

	int time = 0;
	int state = msp_vr_state();
	while ((state==MSP_RECORD_START||state==MSP_RECORD_WAIT) && (vr_local_state==MSP_LOCAL_NORMAL_RUN))
	{
		MSP_Printf("please say...");
#ifdef OS_WIN32
		Sleep(100);
#else
		usleep(100000);
#endif
		if (state == MSP_RECORD_WAIT)
		{
			if (-1 == msp_vr_upload_buffer())
				time++;
		}
		else
		{
			time++;
			MSP_Printf("vr not start");
		}
		if (time > 50){
			msp_vr_send_result(0, NULL, 10114);
			ASRUnRegisterUser();
			return;
		}
		state = msp_vr_state();

	}
	//if forced or unexpected stop
	if (state == MSP_RECORD_TERMINATE || vr_local_state != MSP_LOCAL_NORMAL_RUN)
	{
		msp_vr_send_result(0, NULL, -5);
		ASRUnRegisterUser();
		return;
	}
	int cmdId = 0; char result[200] = { 0 };
	int ret = msp_vr_result(&cmdId, result, 200);
	msp_vr_send_result(cmdId, result, ret);
	ASRUnRegisterUser();

}

void* msp_vr_thread(void *)
{
	pthread_mutex_lock(&mutex_);

	if (!msp_vr_set_state(MSP_RECORD_START))
	{
		msp_vr_send_result(0, NULL, -1);
		vr_local_state = MSP_LOCAL_NORMAL_END;
		pthread_mutex_unlock(&mutex_);
		return NULL;
	}

	msp_vr_record_loop();
	//vr_local_state = MSP_LOCAL_NORMAL_END;
	//printf("msp vr thread:state=normal end\n");
	pthread_mutex_unlock(&mutex_);
	return NULL;
}

bool msp_vr_start()
{
	pthread_mutex_lock(&judge_mutex_);
	if (vr_local_state == MSP_LOCAL_NORMAL_RUN)
	{
		pthread_mutex_unlock(&judge_mutex_);
		return false;
	}
	
	printf("msp_vr_start:state=normal run\n");
	if (0 != pthread_create(&thread_, 0, &msp_vr_thread, NULL)) {
		MSP_Printf("Create thread fail.");
		msp_vr_send_result(0, NULL, -1);
		pthread_mutex_unlock(&judge_mutex_);
		return false;
	}
	vr_local_state = MSP_LOCAL_NORMAL_RUN;
	//pthread_join(thread_, NULL);

	pthread_mutex_unlock(&judge_mutex_);
	return true;
}

void msp_vr_terminate()
{
	pthread_mutex_lock(&judge_mutex_);
	msp_vr_set_state(MSP_RECORD_TERMINATE);
	vr_local_state = MSP_LOCAL_FORCE_QUIT;
	pthread_mutex_unlock(&judge_mutex_);
	printf("msp vr terminate:state=force quit\n");
	pthread_join(thread_, NULL);//wait for thread stop

	/*pthread_mutex_lock(&judge_mutex_);
	
	pthread_mutex_unlock(&judge_mutex_);*/
}




int msp_vr_begin()
{
	if (!msp_mem_assert())
	{
		MSP_Printf("share mem not load\n");
		return -1;
	}

	//MSP_Printf("type=%d,parm=%s\n", shareData->vr_parm.type, shareData->vr_parm.data);

	int code1 = ASRBeginSession();
	if (code1)
	{
		MSP_Printf("session begin code=%d", code1);
		return code1;
	}

	return code1;
}


int msp_vr_upload_buffer()
{
	if (!msp_mem_assert())
	{
		MSP_Printf("share mem not load");
		return -1;
	}

	char *buffer;
	int len = 0;
	if (msp_vr_download_data(&buffer, &len)){
		if (len <= 0)
			return -1;
		int code = ASRWriteAudioBuffer(buffer, len);
		free(buffer);
		return code;
	}
	return -1;
}



int  msp_vr_result(int *cmdId, char result[], int max)
{
	//char *parm = "sub=iat,auf=audio/L16;rate=8000,aue=speex,ent=sms8k,rst=plain,rse=gb2312"; //"sub=iat,auf=audio/L16;rate=8000,aue=speex,ent=sms8k,rst=plain,rse=gb2312";//
	if (!msp_mem_assert())
	{
		MSP_Printf("share mem not load\n");
		return -1;
	}
	char *buffer=NULL;
	int len = 0;
	char cmdIdStr[20] = { 0 };
	msp_vr_download_data(&buffer, &len);
	int code1 = ASRGetResult(cmdIdStr, result, max, buffer, len);
	if (code1 != 0)
		return code1;

	char rst[20] = { 0 };
	int asrlen = strlen(cmdIdStr);
	int j = 0;
	for (int i = 0; i < asrlen; i++){
		if (cmdIdStr[i] != ','){
			rst[j++] = cmdIdStr[i];
		}
	}
	MSP_Printf("%s->%s\n", cmdIdStr, rst);

	*cmdId = atoi(rst);
	return 0;
	//return 1;
}
#endif
