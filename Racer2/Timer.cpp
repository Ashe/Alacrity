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
}

void Timer::updateTimer(float dtime){
	if (mCountdown)
		mCurrentTime -= dtime * 0.1;
}

float Timer::getTimer() const{
	return mCurrentTime;
}