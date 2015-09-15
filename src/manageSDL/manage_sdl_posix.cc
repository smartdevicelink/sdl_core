#include <stdio.h>  
#include <stdlib.h>
#include <sys/types.h>  
#include <csignal>
#include <unistd.h>
#include <vector>
#include <string.h>  
#include <iostream>

using namespace std;

int32_t kSDL_Pid=0;
int32_t kMsp_Pid=0;
pthread_t  _thread1,_thread2;

// kill_with_pid_by_name >>
#define PS_COMMAND "ps ax"
#define MANAGE_SDL_NAME "ManageSDL"
#define SMARTDEVICELINKCORE_NAME "smartDeviceLinkCore"
#ifdef MODIFY_FUNCTION_SIGN
#define SDLMSPVR_NAME         "SDLMspVR.app/Contents/MacOS/SDLMspVR"
#endif

// kill_with_pid_by_name <<

const char kSDL[] = "./smartDeviceLinkCore";
const char kSDLName[] = "smartDeviceLinkCore";
const char kSDLParams[] = "";

#ifdef MODIFY_FUNCTION_SIGN
const char kMSPVR[]="SDLMspVR.app/Contents/MacOS/SDLMspVR";
const char kMSPVRName[]="SDLMspVR.app/Contents/MacOS/SDLMspVR";
const char kMSPVRParams[]="";
#endif

const char* const kParams[4] = {kSDLName, kSDLParams, NULL, NULL};
#ifdef MODIFY_FUNCTION_SIGN
const char* const kMspParams[4] = {kMSPVRName, kMSPVRParams, NULL, NULL};
#endif

void toTokens(char *str, char *delim, std::vector<std::string> &tokens);

// kill_with_pid_by_name >>
void toTokens(char *str, char *delim, std::vector<std::string> &tokens){
  char sstr[4096];
  memset(sstr, 0, sizeof(sstr));
  memcpy(sstr, str, strlen(str));
  char *p = NULL;
  p = strtok(sstr, delim);
  while(p != NULL){
    //printf("the character is :%s\n",p);
    tokens.push_back(p);
    p = strtok(NULL,delim);
  } 
}

bool kill_process_by_name(const char *name, bool keepSelf = false)
{  
  FILE *fp;  
  char buf[4096];
  pid_t pid;
   
  if ((fp = popen(PS_COMMAND, "r")) == NULL)
    return false;  
   
  while (fgets(buf, sizeof(buf), fp)) {
    //printf("buf=%s\n", buf);
    std::vector<std::string> tokens;
    toTokens(buf, " ", tokens);
    if (tokens.size() > 0){
      pid = atoi(tokens[0].c_str());
      if(pid > 0){
        if(strstr(tokens[4].c_str(), name)){
          if(!keepSelf){
            printf("Kill with pid %d\n", pid);
            kill(pid, SIGTERM);
          }else{
            if(getpid() != pid){
              printf("Kill with pid %d\n", pid);
              kill(pid, SIGTERM);
            }
          }
        }
      }
      
    }
  } 
  pclose(fp);  
  return true;  
}
// kill_with_pid_by_name <<

bool Execute2(std::string file, const char * const * argv);

bool SubscribeToTerminateSignal(void (*func)(int32_t p)) {
  void (*prev_func)(int32_t p);
  prev_func = signal(SIGCHLD, func);
  if(SIG_ERR != prev_func){
    return true;
  }else{
    printf("prev_func == SIG_ERR, p = %p\n", prev_func);
    return false;
  }
}

void HandleOnTerminateSignal(int32_t params) {
  
  void (*prev_func)(int32_t p);
  //prev_func = signal(SIGCHLD, SIG_DFL);
  cout << "HandleOnSignal "<<params<< endl;
    static int restart_count = 0;
    cout << "The " << ++restart_count << "th time to restart process "<<"smartDeviceLinkCore" << endl;

    Execute2(kSDL, kParams);
    
}



bool Execute(std::string file, const char * const * argv) {
  pid_t pid_flow = fork();
  switch(pid_flow){
    case -1:{ // Error
      return false;
    }
    case 0:{  // Child process
      cout << "Begin child process" << endl;
      execvp(file.c_str(), const_cast<char* const *>(argv));
      // Not arrivable
      cout << "End child process" << endl;
      return true;
    }
    default:{ // Parent process
      SubscribeToTerminateSignal(&HandleOnTerminateSignal);
      while(1){
        //sleep(1000);
        pause();
      }
      return true;
    }
  }
}

bool Execute2(std::string file, const char * const * argv) {
  pid_t pid_flow= fork();
  switch(pid_flow){
    case -1:{ // Error
      return false;
    }
    case 0:{  // Child process
      cout << "Begin child process" <<file.c_str()<< endl;
      execvp(file.c_str(), const_cast<char* const *>(argv));
      // Not arrivable
      cout << "End child process" << endl;
      return true;
    }
    default:{ // Parent process
      cout << "Handle parent process" << file.c_str()<<endl;
      return true;
    }
  }
}

#ifdef MODIFY_FUNCTION_SIGN

bool Execute3(std::string file, const char * const * argv,int32_t *pid) {
    pid_t pid_flow= fork();
    switch(pid_flow){
        case -1:{ // Error
            return false;
        }
        case 0:{  // Child process
            cout << "Begin child process "<<file.c_str() << endl;
            execvp(file.c_str(), const_cast<char* const *>(argv));
            // Not arrivable
            cout << "End child process "<<file.c_str() << endl;
            return true;
        }
        default:{ // Parent process
            cout<<file.c_str()<<" "<<pid_flow<<endl;
            *pid=pid_flow;
//            SubscribeToTerminateSignal(func);
            return true;
        }
    }
}
#endif

int main(int argc, char **argv) {
  if(argc > 1){
    //cout << "main argv[1] = " << argv[1] << endl;
    if(strcmp(argv[1], "start") == 0){
      cout << "Begin parent process" << endl;
      cout << "Begin kill manageSDL process" << endl;
      kill_process_by_name(MANAGE_SDL_NAME, true);
      cout << "Begin kill smartDeviceLinkCore process" << endl;
      kill_process_by_name(SMARTDEVICELINKCORE_NAME);
#ifdef MODIFY_FUNCTION_SIGN
      cout<<"Begin kill SDLMspVR  process"<<endl;
      kill_process_by_name(SDLMSPVR_NAME,true);
#endif
#ifdef MODIFY_FUNCTION_SIGN
        Execute3(kSDL, kParams, &kSDL_Pid);
        Execute3(kMSPVR, kMspParams, &kMsp_Pid);
        while (true) {
            pid_t wid=wait(NULL);
            if (wid==kSDL_Pid) {
                Execute3(kSDL, kParams, &kSDL_Pid);
                continue;
            }
            else if (wid==kMsp_Pid){
                Execute3(kMSPVR, kMspParams, &kMsp_Pid);
                continue;
            }
            else
                continue;
        }
#else
        Execute(kSDL, kParams);
#endif
        cout << "End parent process" << endl;
    }else{  // argv[1] = close
      cout << "Begin kill manageSDL process" << endl;
      kill_process_by_name(MANAGE_SDL_NAME, true);
      cout << "Begin kill smartDeviceLinkCore process" << endl;
      kill_process_by_name(SMARTDEVICELINKCORE_NAME);
#ifdef MODIFY_FUNCTION_SIGN
      cout<<"Begin kill SDLMspVR  process"<<endl;
      kill_process_by_name(SDLMSPVR_NAME,true);
#endif
    }
    return 0;
  }else{
    cout << "Parameter invalid!" << endl;
    return 1;
  }
}
