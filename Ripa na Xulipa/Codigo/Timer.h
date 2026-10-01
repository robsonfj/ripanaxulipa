
#ifndef __IDJ__Timer__
#define __IDJ__Timer__

#include <iostream>

class Timer {

private:
	float time = 0;
	
public:
	Timer() {};
	void Update(float dt) {time += dt;};
	void Restart() {time = 0;};
	float Get() {return time;};
	void Set(float time) {this->time = time;};
	
};

#endif /* defined(__IDJ__Timer__) */
