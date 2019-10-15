// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Peter Staar (taa@zurich.ibm.com)
//
// Time classes.

#ifndef DCA_PROFILING_EVENTS_TIME_HPP
#define DCA_PROFILING_EVENTS_TIME_HPP

#include <stdexcept>
#include <sstream>

#if !defined(_MSC_VER)
#include <sys/resource.h>
#include <sys/time.h>
#else
#include <time.h>
#include <windows.h>
#include <psapi.h>

#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64

struct timezone
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

inline int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;

  if (nullptr != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tmpres /= 10;  /*convert into microseconds*/
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  if (nullptr != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }

  return 0;
}

#define RUSAGE_SELF 0
#define	RUSAGE_THREAD 1

struct rusage {
  struct timeval ru_utime; /* user time used */
  struct timeval ru_stime; /* system time used */
  long ru_maxrss;
  long ru_ixrss; /* XXX: 0 */
  long ru_idrss; /* XXX: sum of rm_asrss */
  long ru_isrss; /* XXX: 0 */
  long ru_minflt; /* any page faults not requiring I/O */
  long ru_majflt; /* any page faults requiring I/O */
  long ru_nswap; /* swaps */
  long ru_inblock; /* block input operations */
  long ru_oublock; /* block output operations */
  long ru_msgsnd; /* messages sent */
  long ru_msgrcv; /* messages received */
  long ru_nsignals; /* signals received */
  long ru_nvcsw; /* voluntary context switches */
  long ru_nivcsw; /* involuntary " */

#define ru_last ru_nivcsw
};

inline void usage_to_timeval(FILETIME* ft, struct timeval* tv) {
  ULARGE_INTEGER time;
  time.LowPart = ft->dwLowDateTime;
  time.HighPart = ft->dwHighDateTime;

  tv->tv_sec = time.QuadPart / 10000000;
  tv->tv_usec = (time.QuadPart % 10000000) / 10;
}

inline int getrusage(int who, struct rusage* usage) {
  FILETIME creation_time, exit_time, kernel_time, user_time;
  PROCESS_MEMORY_COUNTERS pmc;

  memset(usage, 0, sizeof(struct rusage));

  if (who == RUSAGE_SELF) {
    if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &user_time)) {
      return -1;
    }

    if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
      return -1;
    }

    usage_to_timeval(&kernel_time, &usage->ru_stime);
    usage_to_timeval(&user_time, &usage->ru_utime);
    usage->ru_majflt = pmc.PageFaultCount;
    usage->ru_maxrss = pmc.PeakWorkingSetSize / 1024;
    return 0;
  }
  else if (who == RUSAGE_THREAD) {
    if (!GetThreadTimes(GetCurrentThread(), &creation_time, &exit_time, &kernel_time, &user_time)) {
      return -1;
    }
    usage_to_timeval(&kernel_time, &usage->ru_stime);
    usage_to_timeval(&user_time, &usage->ru_utime);
    return 0;
  }
  else {
    return -1;
  }
}
#endif

namespace dca {
namespace profiling {
// dca::profiling::

class TimeBase {
public:
  TimeBase(long s, long u) : sec(s), usec(u) {}

  static long oneMillion() {
    static long value(1000000);
    return value;
  }

  long sec;
  long usec;
};

class Time : public TimeBase {
public:
  typedef struct timeval TimeValueType;

  Time(long s, long u) : TimeBase(s, u) {}
  Time(TimeValueType& tim) : TimeBase(tim.tv_sec, tim.tv_usec) {}

  Time& operator=(const TimeValueType& tim) {
    sec = tim.tv_sec;
    usec = tim.tv_usec;
    return *this;
  }
};

class WallTime : public Time {
public:
  typedef struct timeval TimeValueType;

  WallTime() : Time(0, 0) {
    TimeValueType tim;
    gettimeofday(&tim, NULL);
    this->sec = tim.tv_sec;
    this->usec = tim.tv_usec;
  }
};

class ResourceTime : public Time {
public:
  typedef struct rusage UsageType;

  UsageType usage;
  int status;

  int getResourceUsage() {
    return getrusage(RUSAGE_SELF, &usage);
  }

  ResourceTime(long s, long u) : Time(s, u) {}

  ResourceTime() : Time(0, 0), status(this->getResourceUsage()) {
    if (status != 0) {
      std::ostringstream msg;
      msg << "ResourceTime could not retreive the time status = " << status << "\n";
      throw std::logic_error(msg.str());
    }
  }

  ResourceTime(const UsageType& usg) : Time(0, 0), usage(usg), status(1) {}
};

class UserTime : public ResourceTime {
public:
  typedef struct rusage UsageType;
  UserTime(long s, long u) : ResourceTime(s, u) {}
  UserTime() : ResourceTime() {
    Time& tim(*this);
    tim = this->usage.ru_utime;
  }
  UserTime(const UsageType& usge) : ResourceTime(usge) {
    Time& tim(*this);
    tim = this->usage.ru_utime;
  }
};

class SystemTime : public ResourceTime {
public:
  typedef struct rusage UsageType;
  SystemTime(long s, long u) : ResourceTime(s, u) {}
  SystemTime() : ResourceTime() {
    Time& tim(*this);
    tim = this->usage.ru_stime;
  }
  SystemTime(const UsageType& usge) : ResourceTime(usge) {
    Time& tim(*this);
    tim = this->usage.ru_stime;
  }
};

class Duration : public TimeBase {
public:
  Duration(long s, long u) : TimeBase(s, u) {
    normalize();
  }

  Duration(const Time& time1, const Time& time2) : TimeBase(0, 0) {
    this->setFrom(time1, time2);
  }

  Duration(const Duration& duration1, const Duration& duration2) : TimeBase(0, 0) {
    this->setFrom(duration1, duration2);
  }

  void setFrom(const Time& time, const Time& earlierTime) {
    this->sec = time.sec - earlierTime.sec;
    this->usec = time.usec - earlierTime.usec;
    if (this->usec < 0) {
      this->sec = this->sec - 1;
      this->usec = oneMillion() + this->usec;
    }
  }

  void normalize() {
    this->sec += this->usec / oneMillion();
    this->usec = this->usec % oneMillion();
  }

  void setFrom(const Duration& duration1, const Duration& duration2) {
    this->sec = duration1.sec + duration2.sec;
    this->usec = duration1.usec + duration2.usec;
    normalize();
  }

  Duration operator+(const Duration& other) {
    return Duration(*this, other);
  }
};

Duration operator-(const Time& time, const Time& earlierTime);

}  // profiling
}  // dca

#endif  // DCA_PROFILING_EVENTS_TIME_HPP
