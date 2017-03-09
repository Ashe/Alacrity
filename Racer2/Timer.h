#ifndef TIMER_H
#define TIMER_H

using namespace std;

class Timer
{
public:
	//constructors
	Timer(float time = 30.0f) : totalTime_(time), currentTime_(totalTime_), countdown_(false) {
	}
	~Timer();
	//update functions
	void startTimer();				//start countdown of timer
	void pauseTimer();				//pause countdown of timer
	void resetTimer();				//reset current time of timer to max time
	void setTimer(float);			//change starting time on timer
	void updateTimer(float);		//timer updates current time counting down
	//query functions
	float getTimer() const;			//return value of current time
private:
	bool countdown_;				//controls whether timer should countdown or not
	float totalTime_;				//starting time for timer
	float currentTime_;				//current time of timer
}
#endif

