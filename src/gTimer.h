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

	  timeval startUser = startProfile.ru_utime;
	  timeval startSys = startProfile.ru_stime;
	  timeval endUser = endProfile.ru_utime;
	  timeval endSys = endProfile.ru_stime;

	  static const float microToMillis = 1000000.0f;
	  
 	  uSec = endUser.tv_sec - startUser.tv_sec;
	  uMillis = (endUser.tv_usec - startUser.tv_usec) / microToMillis;
	  sSec = endSys.tv_sec - startSys.tv_sec;
	  sMillis = (endSys.tv_usec - startSys.tv_usec) / microToMillis;
	}

	/** Print the most recent time measurement if it is finished.
		Else, notify that it is still running.
	*/
	friend std::ostream& operator<<(std::ostream& os, Timer& timer) {
	  if (timer.isTiming) {
		os << "TIMER RUNNING";
		return os;
	  }

	  os << std::setprecision(4) << std::fixed
		 << "User: " << (static_cast<float>(timer.uSec) + timer.uMillis) << " sec\t"
		 << "System: " << (static_cast<float>(timer.sSec) + timer.sMillis) << " sec";
	  
	  return os;
	}

	
  private:
	/** Hold beginning and end time state information. */
	struct rusage startProfile, endProfile;
	
	/** Timings extracted from #startProfile and #endProfile */
	long int uSec, sSec;
	float uMillis, sMillis;

	/** Track state of timer to notify user if misusing. */
	bool isTiming;
  };
  
}  // GUtil namespace

#endif  //__GTIMER_H__
