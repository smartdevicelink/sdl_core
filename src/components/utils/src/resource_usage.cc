#include "utils/resource_usage.h"
#if defined(__QNXNTO__)
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/procfs.h>
#include <sys/stat.h>
#include <sys/trace.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <sys/resource.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include "utils/file_system.h"

namespace utils {

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

const char* Resources::proc = "/proc/";

ResourseUsage* Resources::getCurrentResourseUsage() {
  PidStats pid_stats;
  if (false == getProcInfo(pid_stats)) {
    LOG4CXX_ERROR(logger_, "Failed to get cpu proc info");
    return NULL;
  }
  MemInfo mem_info;
  if (false == getMemInfo(mem_info)) {
    LOG4CXX_ERROR(logger_, "Failed to get memory info");
    return NULL;
  }
  ResourseUsage* usage = new ResourseUsage();
  usage->utime = pid_stats.utime;
  usage->stime = pid_stats.stime;
  usage->memory = static_cast<long long int>(mem_info);
  return usage;
}

bool Resources::readMyProcFile(std::string& output) {
  std::string filename = getMyProcPath();
  if (false == file_system::FileExists(filename)) {
    return false;
  }
  if (false == file_system::ReadFile(filename,output)) {
    return false;
  }
  return true;
}

bool Resources::getProcInfo(Resources::PidStats& output) {
#if defined(OS_LINUX)
  std::string proc_buf;
  if (false == readMyProcFile(proc_buf)) {
    return false;
  }
  uint32_t num_succes = sscanf(proc_buf.c_str(),
         "%d" //pid
         " %*s"//com
         " %c" //state
         " %d" //ppid
         " %d" //pgrp
         " %d" //session
         " %d" //tty_nr
         " %d" //tpgid
         " %u" //flags
         " %lu" //minflt
         " %lu" //cminflt
         " %lu" //majflt
         " %lu" //cmajflt
         " %lu" //utime
         " %lu" //stime
         " %ld" //cutime
         " %ld" //cstime
         " %ld" //priority
         " %ld" //nice
         " %ld" //num_threads
         " %ld" //itrealvalue
         " %llu" //starttime
         " %lu" //vsize
         " %ld" //rss
         " %lu" //rsslim
         " %lu" //startcode
         " %lu" //endcode
         " %lu" //startstack
         " %lu" //kstkesp
         " %lu" //kstkip
         " %lu" //signal
         " %lu" //blocked
         " %lu" //sigignore
         " %lu" //sigcatch
         " %lu" //wchan
         " %lu" //nswap
         " %lu" //cnswap
         " %d" //exit_signal
         " %d" //processor
         " %u" //rt_priority
         " %u" //policy
         " %llu" //delayacct_blkio_ticks
         " %lu" //guest_time
         " %ld" //cguest_time
         ,&(output.pid), &(output.state), &(output.ppid), &(output.pgrp), &(output.session),
         &(output.tty_nr), &(output.tpgid), &(output.flags), &(output.minflt), &(output.cminflt),
         &(output.majflt), &(output.cmajflt), &(output.utime), &(output.stime), &(output.cutime),
         &(output.cstime), &(output.priority), &( output.nice), &(output.num_threads), &(output.itrealvalue),
         &(output.starttime), &(output.vsize), &(output.rss), &(output.rsslim), &(output.startcode),
         &(output.endcode), &(output.startstack), &(output.kstkesp), &(output.kstkeip), &(output.signal),
         &(output.blocked), &(output.sigignore), &(output.sigcatch), &(output.wchan), &(output.nswap),
         &(output.cnswap), &(output.exit_signal), &(output.processor), &(output.rt_priority), &(output.policy),
         &(output.delayacct_blkio_ticks), &(output.guest_time), &(output.cguest_time)
  );
  if(num_succes != 43) { // 43 is number of iteams in Resources::PidStats
    LOG4CXX_ERROR(logger_, "Couldn't parse all iteams in /proc/PID/stat file");
    return false;
  }
  return true;
#elif defined(__QNXNTO__)
  int fd = open(getMyProcPath().c_str(), O_RDONLY);
  if (0 >= fd) {
    LOG4CXX_ERROR(logger_, "Failed open process proc file : " << getMyProcPath() <<
                  "; error no : " << strerror( errno ) );
    return false;
  }
  devctl(fd, DCMD_PROC_INFO, &output, sizeof(output), 0);
  close(fd);
  return true;
#endif
}

bool Resources::getMemInfo(Resources::MemInfo &output) {
  #if defined(OS_LINUX)
  Resources::PidStats pid_stat;
  if (false == getProcInfo(pid_stat)) {
    LOG4CXX_ERROR(logger_, "Failed to get proc info");
    return false;
  }
  output = pid_stat.rss;
  return true;
#elif defined(__QNXNTO__)
  std::string proc_path = getMyProcPath();
  std::string as = proc_path + std::string("/as");
  struct stat st;
  struct _dir* proc_dir = 0;
  struct dirent* proc_entry = 0;
  if (0 == (proc_dir = opendir(proc))) {
    LOG4CXX_ERROR(logger_, "Unable to access to " << proc);
    return false;
  }
 // output = 0;
  if (0 == (proc_entry = readdir(proc_dir))) {
    LOG4CXX_ERROR(logger_, "Unable to read : " << proc_dir);
    return false;
  }

  if (-1 == stat(as.c_str(), &st) || 0 == st.st_size) {
     LOG4CXX_ERROR(logger_, "Unable to stat : " << as.c_str());
     return false;
  }
  output = st.st_size;
  closedir(proc_dir);
  return true;
#endif
}

std::string Resources::getMyProcPath() {
  pid_t my_pid = getpid();
  std::stringstream stream;
  stream << proc << my_pid;
#if defined(OS_LINUX)
  stream<<"/stat";
#endif
  std::string filename;
  stream >> filename;
  return filename;
}

} // namespace utils
