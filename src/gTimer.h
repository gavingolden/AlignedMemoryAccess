#ifndef __GTIMER_H__
#define __GTIMER_H__

/**
   Copyright (c) 2015 Gavin Golden gavinegolden@gmail.com.
   All rights reserved.
*/

#include <sys/time.h>
#include <sys/resource.h>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace GUtil {

  /** A simple timer class for tracking system and user time
	  of the entire calling process.
  */
  class Timer {
  public:
	Timer() { isTiming = false; }

	/** Call to mark the beginning of a time measurement.
		Can't be called if timer has already been started.
	 */
	void start() {
	  if (isTiming) { throw std::runtime_error("Timer is already started!"); }
	  getrusage(RUSAGE_SELF, (rusage*)&startProfile);
	  isTiming = true;
	}

	/** Call to mark the end of a time measurement.
		Can't be called if the timer was never started.
	*/
	void end() {
	  if (!isTiming) { throw std::runtime_error("Timer was not running!"); }
	  getrusage(RUSAGE_SELF,(rusage*)&endProfile);
	  isTiming = false;
	}

	/** Print the most recent time measurement if it is finished.
		Else, notify that it is still running.
	*/
	friend std::ostream& operator<<(std::ostream& os, Timer& timer) {
	  if (timer.isTiming) {
		os << "TIMER RUNNING";
		return os;
	  }
	  timer.startUser = timer.startProfile.ru_utime;
	  timer.startSys = timer.startProfile.ru_stime;
	  timer.endUser = timer.endProfile.ru_utime;
	  timer.endSys = timer.endProfile.ru_stime;

	  timer.printTime(os);
	  return os;
	}

  private:
	/** Internal convenience method for printing to output. */
	void printTime(std::ostream& os) {
	  float microConversion = 1000000.0f;
	  
	  long int uSec = endUser.tv_sec - startUser.tv_sec;
	  float uMillis = (endUser.tv_usec - startUser.tv_usec) / microConversion;
	  long int sSec = endSys.tv_sec - startSys.tv_sec;
	  float sMillis = (endSys.tv_usec - startSys.tv_usec) / microConversion;

	  os << std::setprecision(4) << std::fixed
		 << "User: " << (static_cast<float>(uSec) + uMillis) << " sec\t"
		 << "System: " << (static_cast<float>(sSec) + sMillis) << " sec";
	  // Zero out timeval structs
	  startUser = endUser = startSys = endSys = (struct timeval){0, 0};
	}

	/** Hold beginning and end time state information. */
	struct rusage startProfile, endProfile;
	
	/** Hold information extracted from #startProfile and #endProfile */
	struct timeval startUser, endUser, startSys, endSys;

	/** Track state of timer to notify user if misusing. */
	bool isTiming;
  };
  
}  // GUtil namespace

#endif  //__GTIMER_H__
