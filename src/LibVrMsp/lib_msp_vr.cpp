#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#include "lib_msp_vr.h"
#include "asr.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>

#ifndef  ASR_HAS_VR
#include <include/msp_errors.h>

#endif
#include <ctype.h>
#include <pthread.h>

#ifdef OS_WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#include<fcntl.h>
#endif

#include "msp_type.h"

//share mutex

ShareMemData   *shareData = NULL;

#ifdef OS_WIN32
typedef HANDLE  ShareMemHandle;
#define SHARE_NULL   NULL
#ifdef OS_WINCE
#define  SHARE_MEM_NAME  reinterpret_cast<LPCWSTR>("Sdl_MspVRMem")
#else
#define  SHARE_MEM_NAME  "Sdl_MspVRMem"
#endif // OS_WINCE
#else
typedef int     ShareMemHandle;
#define SHARE_NULL   ((ShareMemHandle)-1)
#define  SHARE_MEM_NAME  "Sdl_MspVRMem"
#endif


int  vr_run_mode;
msp_log    _msp_log;



//int record_state=0;//0:stop,1:start

ShareMemHandle  shareMem;
//ShareMemHandle  mutexMem;





void msp_vr_register_log_callback(msp_log  logFun)
{
	_msp_log = logFun;
}



void msp_log_mode(int mode)
{
	vr_run_mode = mode;
}



 int   msp_vr_state()
{
    return msp_mem_state_read();
//    return record_state;
}

 bool  msp_vr_set_state(int state)
 {
//    record_state=state;
    return msp_mem_state_write(state);
}


 bool msp_mutex_enter()
 {
#ifdef  OS_WIN32
	 //DWORD d = WaitForSingleObject(mutexMem, 500);//INFINITE
	 //printf("WAIT D=%d\n", d);
	 //return (d == WAIT_OBJECT_0);
	 
#endif
	 //int lock=pthread_mutex_lock(&shareData->data.mutex_);
	 return true;
 }

 void msp_mutex_leave()
 {
#ifdef  OS_WIN32
	 /*ReleaseMutex(mutexMem);*/
#endif
	 //pthread_mutex_unlock(&shareData->data.mutex_);
 }

 int msp_vr_data_len()
 {
	 return (shareData->data.end - shareData->data.begin);
 }
 bool  msp_vr_upload_data(char *data,int len)
{
	 if (!msp_mem_assert()||len<=0)
		 return false;
	 if (!msp_mutex_enter())
	 {
		 MSP_Printf("mutex enter failed");
		 return false;
	 }
	 int valid_len = shareData->data.end -shareData->data.begin;
	 if (valid_len <= 0)
	 {
		 shareData->data.begin = shareData->data.end = 0;
		 shareData->data.len = 0;
	 } 
	 if (shareData->data.end + len >= SHARE_MAX_SIZE)
	 {
		 MSP_Printf("mem data over");
		 return false;
	 }
    memcpy((char *)&shareData->data.data[shareData->data.end],data,len);
    shareData->data.len +=len;
	//shareData->data.begin = shareData->data.end;
	shareData->data.end = shareData->data.end + len;
	return true;
}

 bool msp_vr_download_data(char **dest,int *len)
 {
	 if (!msp_mem_assert())
		 return false;		 
	 
	 int valid_len = shareData->data.end - shareData->data.begin;
	 if (valid_len <= 0 || shareData->data.begin<0 || shareData->data.begin >= SHARE_MAX_SIZE || shareData->data.end<0 || shareData->data.end >= SHARE_MAX_SIZE)
	 {
		 return false;
	 }
	 if (!msp_mutex_enter())
	 {
		 MSP_Printf("mutex enter failed");
		 return false;
	 }
	 *dest = (char *)malloc(valid_len+1);
	 if (*dest == NULL)
	 {
		 *len = 0;
		 msp_mutex_leave();
		 MSP_Printf("buffer malloc failed");
		 return false;
	 }
	 memcpy(*dest, (char *)&shareData->data.data[shareData->data.begin],valid_len);
	 *len = valid_len;
	 MSP_Printf("begin=%d,end=%d,len=%d", shareData->data.begin, shareData->data.end, valid_len);
	 shareData->data.len = shareData->data.len-valid_len;
	 shareData->data.begin = shareData->data.end;
	 msp_mutex_leave();
	 return true;
 }

 bool msp_mem_data_address(char **dest, int *len)
 {
	 if (!msp_mem_assert())
		 return false;

	 int valid_len = shareData->data.end - shareData->data.begin;
	 if (valid_len <= 0)
	 {
		 return true;
	 }
	 msp_mutex_enter();

	 *dest = (char *)&shareData->data.data[shareData->data.begin];
	 *len = valid_len;
	 MSP_Printf("begin=%d,end=%d,len=%d", shareData->data.begin, shareData->data.end, valid_len);
	 shareData->data.len = shareData->data.len - valid_len;
	 shareData->data.begin = shareData->data.end;

	 msp_mutex_leave();
	 return true;
 }


 bool msp_mem_assert()
 {
#ifdef OS_WIN32
     if (shareData == NULL)
     {
         shareData = (ShareMemData*)MapViewOfFile(shareMem, FILE_MAP_ALL_ACCESS, 0,0, MEM_SHARE_DATA);
         if (shareData == NULL)
         {
             MSP_Printf("data init failed");
             return false;
         }
     }
#else
     if(shareData==MAP_FAILED||shareData==NULL)
     {
         shareData=(ShareMemData*)mmap(0,MEM_SHARE_DATA,PROT_READ|PROT_WRITE,MAP_SHARED,shareMem,0);
         if(shareData==MAP_FAILED||shareData==NULL)
         {
             MSP_Printf("data init failed");
             return false;
         }
     }
#endif
    return true;
 }


 void  msp_mem_init(int mode)
{
    //shareMem=new QSharedMemory("Sdl_MspVRMem");
	
    if(mode==MSP_MEM_SERVER)
    {
#ifdef  OS_WIN32
        shareMem = CreateFileMapping(INVALID_HANDLE_VALUE, SHARE_NULL, PAGE_READWRITE, 0,MEM_SHARE_DATA,SHARE_MEM_NAME);
        if (shareMem == SHARE_NULL)
        {
            MSP_Printf("share mem create failed");
            return;
        }
        //shareMutex_ = CreateFileMapping(INVALID_HANDLE_VALUE, SHARE_NULL, PAGE_READWRITE, 0, sizeof(ShareMemData), "Sdl_MspVRMem");
        shareData = (ShareMemData*)MapViewOfFile(shareMem, FILE_MAP_ALL_ACCESS, 0, 0, MEM_SHARE_DATA);//shareMem->data()
#else
#ifdef OS_ANDROID
        shareMem=open(SHARE_MEM_NAME,O_CREAT | O_RDWR);
#else
        shareMem=shm_open(SHARE_MEM_NAME,O_CREAT | O_RDWR,S_IRUSR | S_IWUSR);
#endif
        if (shareMem <=SHARE_NULL)
        {
            MSP_Printf("share mem create failed");
            return;
        }
        ftruncate(shareMem, MEM_SHARE_DATA);
        shareData=(ShareMemData*)mmap(0,MEM_SHARE_DATA,PROT_READ|PROT_WRITE,MAP_SHARED,shareMem,0);
        if (shareData == MAP_FAILED)
    {
            MSP_Printf("share mem is null");
        return;
    }
#endif

        shareData->state=0;
		shareData->data.len = 0;
		shareData->data.begin = shareData->data.end = 0;
#ifdef  OS_WIN32
		//mutexMem = CreateMutex(NULL,false, "Sdl_MspMutex");
#endif
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

		pthread_mutex_init(&shareData->data.mutex_,&attr);
    }
    else if(mode==MSP_MEM_CLIENT)
    {
#ifdef OS_WIN32
#ifdef OS_WINCE
		shareMem=CreateFileMapping(INVALID_HANDLE_VALUE, SHARE_NULL, PAGE_READWRITE, 0,MEM_SHARE_DATA,SHARE_MEM_NAME);
#else
		shareMem = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, SHARE_MEM_NAME); //打开共享文件  
#endif
        if (shareMem == SHARE_NULL)
        {
            MSP_Printf("share mem file mapping failed");
            return;
        }
         shareData = (ShareMemData*)MapViewOfFile(shareMem, FILE_MAP_ALL_ACCESS, 0, 0, MEM_SHARE_DATA);//shareMem->data();
#else
#ifdef OS_ANDROID
        shareMem=open(SHARE_MEM_NAME,O_CREAT | O_RDWR);
#else
        shareMem=shm_open(SHARE_MEM_NAME,O_CREAT | O_RDWR,S_IRUSR | S_IWUSR);
#endif
        if (shareMem <=SHARE_NULL)
        {
            MSP_Printf("share mem create failed");
            return;
        }
        shareData=(ShareMemData*)mmap(0,MEM_SHARE_DATA,PROT_READ|PROT_WRITE,MAP_SHARED,shareMem,0);
        if (shareData==MAP_FAILED) {
            return;
        }
#endif

		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

		pthread_mutex_init(&shareData->data.mutex_, &attr);
    }
}

 void msp_mem_destroy()
 {
	 if (shareMem != SHARE_NULL)
	 {
#ifdef OS_WIN32
		 CloseHandle(shareMem);
#endif
	 }
		 
 }

 void msp_mem_data_init()
 {
	 if (!msp_mem_assert())
		 return;
	 shareData->data.len = 0;
	 shareData->data.begin = shareData->data.end = 0;
 }




 bool  msp_mem_state_write(int state)
{
	 if (!msp_mem_assert())
		 return false;
    shareData->state=state;
	return true;
}


 int  msp_mem_state_read()
{
	 if (!msp_mem_assert())
		 return -1;
          return shareData->state;
}

 /*
 vr parm set
*/
 void msp_vrparm_set(int type,int max_duration, int sampling_rate,
	 int bits_per_sample, int audio_type)
 {
	 if(!msp_mem_assert())
		 return;
	 shareData->vr_type = type;
	 shareData->vr_parm.max_duration = max_duration;
	 shareData->vr_parm.sampling_rate = sampling_rate;
	 shareData->vr_parm.bits_per_sample = bits_per_sample;
	 shareData->vr_parm.audio_type = audio_type;
 }

 /*
 vr parm get
 */
 bool msp_vrparm_get(int *type,int *max_duration, int *sampling_rate,
	 int *bits_per_sample, int *audio_type)
 {
	 if (!msp_mem_assert())
		 return false;
	 *type = shareData->vr_type;
	 *max_duration=shareData->vr_parm.max_duration;
	 *sampling_rate=shareData->vr_parm.sampling_rate;
	 *bits_per_sample=shareData->vr_parm.bits_per_sample;
	 *audio_type=shareData->vr_parm.audio_type;
	 return true;
 }

 bool msp_vr_is_login()
 {
	 if (!msp_mem_assert())
		 return false;

	 return shareData->login;
 }

 void msp_vr_login(bool login)
 {
	 if (!msp_mem_assert())
		 return ;

	 shareData->login=login;
 }

 bool msp_vr_is_upload()
 {
	 if (!msp_mem_assert())
		 return false;

	 return shareData->upload;
 }

 void msp_vr_upload(bool load)
 {
	 if (!msp_mem_assert())
            return;
	 shareData->upload= load;

 }
 
