#include "Timer.h"

void Timer::startTimer(){
	mCountdown = true;
}

void Timer::pauseTimer(){
	mCountdown = false;
}
void Timer::resetTimer(){
	mCountdown = false;
	mCurrentTime = mTotalTime;
}
void Timer::setTimer(float time){
	mTotalTime = time;
	resetTimer();
}

void Timer::updateTimer(float dtime){
	if (mCountdown && mCurrentTime > 0)
		mCurrentTime -= dtime;
	if (mCurrentTime < 0)
		mCurrentTime = 0.0;
}

float Timer::getTimer() const{
	return mCurrentTime;
}