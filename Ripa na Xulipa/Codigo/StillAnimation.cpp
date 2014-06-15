

#include "StillAnimation.h"


StillAnimation::StillAnimation(float x, float y, float rotation, Sprite sprite, float timeLimit, bool ends){
	
	sp = sprite;
	this->rotation = rotation;
	this->timeLimit = timeLimit;
	oneTimeOnly = ends;
	
	box.x = x - sp.GetWidth()/2;
	box.y = y - sp.GetHeight()/2;
	box.w = sp.GetWidth();
	box.h = sp.GetHeight();
	
}

void StillAnimation::Update(float dt){
	sp.Update(dt);
	endTimer.Update(dt*1000);
	
}

void StillAnimation::Render(){
	
	sp.Render(box.x - Camera::pos.x, box.y - Camera::pos.y);
}


bool StillAnimation::IsDead(){
	
	if (oneTimeOnly)
		if (endTimer.Get() >= timeLimit)
			return true;
	
	return false;
}


bool StillAnimation::Is(string type){
	
	if (type == "still")
		return true;
	
	return false;
}
