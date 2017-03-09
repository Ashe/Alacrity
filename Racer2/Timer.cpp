#include "Timer.h"

void Timer::startTimer(){
	countdown_ = true;
}

void Timer::pauseTimer(){
	countdown_ = false;
}
void Timer::resetTimer(){
	countdown_ = false;
	currentTime_ = totalTime_;
}
void Timer::setTimer(float time){
	totalTime_ = time;
}


void Timer::updateTimer(float dtime){
	if (countdown_)
		currentTime_ -= dtime * 0.1;
}

float Timer::getTimer() const{
	return currentTime_;
}