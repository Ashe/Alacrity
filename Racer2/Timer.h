#ifndef TIMER_H
#define TIMER_H

using namespace std;

class Timer
{
public:
	//constructors
	Timer(float time = 30.0f) : mTotalTime(time), mCurrentTime(mTotalTime), mCountdown(false) {
	}
	~Timer(){}
	//update functions
	void startTimer();				//start countdown of timer
	void pauseTimer();				//pause countdown of timer
	void resetTimer();				//reset current time of timer to max time
	void setTimer(float);			//change starting time on timer
	void updateTimer(float);		//timer updates current time counting down
	//query functions
	float getTimer() const;			//return value of current time
private:
	bool mCountdown;				//controls whether timer should countdown or not
	float mTotalTime;				//starting time for timer
	float mCurrentTime;				//current time of timer
};
#endif

