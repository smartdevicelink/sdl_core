// c declare
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> // for opendir(), readdir(), closedir()
#include <sys/stat.h> // for stat()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
// c++ declare
#include <csignal>
#include <iostream>

using namespace std;

// kill_with_pid_by_name >>
#define PROC_DIRECTORY "/proc/"
#define CASE_SENSITIVE    1
#define CASE_INSENSITIVE  0
#define EXACT_MATCH       1
#define INEXACT_MATCH     0
#define MANAGE_SDL_NAME "ManageSDL"
#define SMARTDEVICELINKCORE_NAME "smartDeviceLinkCore"
// kill_with_pid_by_name <<

const char kSDL[] = "./smartDeviceLinkCore";
const char kSDLName[] = "smartDeviceLinkCore";
const char kSDLParams[] = "";
const char* const kParams[4] = {kSDLName, kSDLParams, NULL, NULL};

// kill_with_pid_by_name >>
int IsNumeric(const char* ccharptr_CharacterList)
{
    for ( ; *ccharptr_CharacterList; ccharptr_CharacterList++)
        if (*ccharptr_CharacterList < '0' || *ccharptr_CharacterList > '9')
            return 0; // false
    return 1; // true
}

//intCaseSensitive=0 is not sensitive
int strcmp_Wrapper(const char *s1, const char *s2, int intCaseSensitive)
{
    if (intCaseSensitive)
        return !strcmp(s1, s2);
    else
        return !strcasecmp(s1, s2);
}

//intCaseSensitive=0 is not sensitive
int strstr_Wrapper(const char* haystack, const char* needle, int intCaseSensitive)
{
    if (intCaseSensitive)
        return (int) strstr(haystack, needle);
    else
        return (int) strcasestr(haystack, needle);
}

bool KillPIDbyName_implements(const char* cchrptr_ProcessName, int intCaseSensitiveness, int intExactMatch, bool keepSelf = false)
{
    char chrarry_CommandLinePath[100]  ;
    char chrarry_NameOfProcess[300]  ;
    char* chrptr_StringToCompare = NULL ;
    pid_t pid_ProcessIdentifier = (pid_t) -1 ;
    struct dirent* de_DirEntity = NULL ;
    DIR* dir_proc = NULL ;

    int (*CompareFunction) (const char*, const char*, int) ;

    if (intExactMatch)
        CompareFunction = &strcmp_Wrapper;
    else
        CompareFunction = &strstr_Wrapper;


    dir_proc = opendir(PROC_DIRECTORY) ;
    if (dir_proc == NULL)
    {
        perror("Couldn't open the " PROC_DIRECTORY " directory") ;
        return false ;
    }

    // Loop while not NULL
    while ( (de_DirEntity = readdir(dir_proc)) )
    {
        if (de_DirEntity->d_type == DT_DIR)
        {
            if (IsNumeric(de_DirEntity->d_name))
            {
                strcpy(chrarry_CommandLinePath, PROC_DIRECTORY) ;
                strcat(chrarry_CommandLinePath, de_DirEntity->d_name) ;
                strcat(chrarry_CommandLinePath, "/cmdline") ;
                FILE* fd_CmdLineFile = fopen (chrarry_CommandLinePath, "rt") ;  //open the file for reading text
                if (fd_CmdLineFile)
                {
                    fscanf(fd_CmdLineFile, "%s", chrarry_NameOfProcess) ; //read from /proc/<NR>/cmdline
                    fclose(fd_CmdLineFile);  //close the file prior to exiting the routine

                    if (strrchr(chrarry_NameOfProcess, '/'))
                        chrptr_StringToCompare = strrchr(chrarry_NameOfProcess, '/') +1 ;
                    else
                        chrptr_StringToCompare = chrarry_NameOfProcess ;

                    //printf("Process name: %s\n", chrarry_NameOfProcess);
                    //this is full path as /bin/ls
                    //printf("Pure Process name: %s\n", chrptr_StringToCompare );
                    //this is process name as ls

                    //If to compare full path, set chrpth_StringToCompare=chrarry_NameOfProcess
                    if ( CompareFunction(chrptr_StringToCompare, cchrptr_ProcessName, intCaseSensitiveness) )
                    {
                        pid_ProcessIdentifier = (pid_t) atoi(de_DirEntity->d_name) ;
                        if(pid_ProcessIdentifier > 0){
                          if(!keepSelf){
                            printf("Kill with pid %d\n", pid_ProcessIdentifier);
                            kill(pid_ProcessIdentifier, SIGTERM);
                          }else{
                            if(getpid() != pid_ProcessIdentifier){
                              printf("Kill with pid %d\n", pid_ProcessIdentifier);
                              kill(pid_ProcessIdentifier, SIGTERM);
                            }
                          }
                        }
                    }
                }
            }
        }
    }
    closedir(dir_proc) ;
    return true ;
}

// Interface
pid_t KillPIDbyName_Wrapper(const char* cchrptr_ProcessName, bool keepSelf = false)
{
  return KillPIDbyName_implements(cchrptr_ProcessName, 0,0,keepSelf);//大小写不敏感
}
// kill_with_pid_by_name <<

bool Execute2(std::string file, const char * const * argv);

bool SubscribeToTerminateSignal(void (*func)(int32_t p)) {
  void (*prev_func)(int32_t p);
  prev_func = signal(SIGCHLD, func);
  return (SIG_ERR != prev_func);
}

void HandleOnTerminateSignal(int32_t params) {
  static int restart_count = 0;
  void (*prev_func)(int32_t p);
  //prev_func = signal(SIGCHLD, SIG_DFL);
  cout << "HandleOnSignal" << endl;
  cout << "The " << ++restart_count << "th time to restart the child process" << endl;
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
      cout << "Begin child process" << endl;
      execvp(file.c_str(), const_cast<char* const *>(argv));
      // Not arrivable
      cout << "End child process" << endl;
      return true;
    }
    default:{ // Parent process
      cout << "Handle parent process" << endl;
      return true;
    }
  }
}

int main(int argc, char **argv) {
  if(argc > 1){
    cout << "main argv[1] = " << argv[1] << endl;
    if(strcmp(argv[1], "start") == 0){
      cout << "Begin parent process" << endl;
      cout << "Begin kill manageSDL process" << endl;
      KillPIDbyName_Wrapper(MANAGE_SDL_NAME, true);
      cout << "Begin kill smartDeviceLinkCore process" << endl;
      KillPIDbyName_Wrapper(SMARTDEVICELINKCORE_NAME);
      Execute(kSDL, kParams);
      cout << "End parent process" << endl;
    }else{  // argv[1] = close
      cout << "Begin kill manageSDL process" << endl;
      KillPIDbyName_Wrapper(MANAGE_SDL_NAME, true);
      cout << "Begin kill smartDeviceLinkCore process" << endl;
      KillPIDbyName_Wrapper(SMARTDEVICELINKCORE_NAME);
    }
    return 0;
  }else{
    cout << "Parameter invalid!" << endl;
    return 1;
  }
}
