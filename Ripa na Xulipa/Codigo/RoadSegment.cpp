

#include "RoadSegment.h"
#include "Game.h"

RoadSegment::RoadSegment(float z_worldpt1, float z_worldpt2, float speed, float roadWidth){
	
	x_world = 0 - Camera::pos.x;
	y_world = 1 - Camera::pos.y;
	this->z_worldpt1 = z_worldpt1;
	this->z_worldpt2 = z_worldpt2;
	this->roadWidth = roadWidth;
	this->speed = speed;
}

void RoadSegment::Update(float dt) {
	double halfScreenWidth = Game::GetInstance().GetWindowWidth()/2;
//  ajustado para que a rua fique no horizonte da imagem, q nao esta exatamente no meio
	double halfScreenHeight = Game::GetInstance().GetWindowHeight()*0.6;
	
	x_world = -2 - Camera::pos.x;
	y_world = 1 - Camera::pos.y;
	
//	seta a velocidade da pista
	z_worldpt1 -= speed;
	z_worldpt2 -= speed;

//	transformacoes dos pontos do "Mundo" para os pontos na tela
	screenRect1.x = halfScreenWidth + ((x_world/z_worldpt1) * halfScreenWidth);
	screenRect2.x = halfScreenWidth + ((x_world/z_worldpt2) * halfScreenWidth);
	
	screenRect1.y = halfScreenHeight + ((y_world/z_worldpt1) * halfScreenHeight);
	screenRect2.y = halfScreenHeight + ((y_world/z_worldpt2) * halfScreenHeight);
	
	screenRect1.w = (roadWidth/z_worldpt1)* halfScreenWidth;
	screenRect2.w = (roadWidth/z_worldpt2)* halfScreenWidth;
	
}

void RoadSegment::Render(Sprite spRoad){
	float escala = screenRect2.w/screenRect1.w;
	
//	ponto superior esquerdo
	glTexCoord4f(0, escala, 0, escala);
	glVertex2f(screenRect2.x - screenRect2.w, screenRect2.y);
	
//  ponto superior direito
	glTexCoord4f(escala, escala, 0, escala);
	glVertex2f( screenRect2.x + screenRect2.w, screenRect2.y);
	
//  ponto inferior direito
	glTexCoord4f(1, escala, 0, 1);
	glVertex2f( screenRect1.x + screenRect1.w, screenRect1.y);
	
//  ponto inferior esquerdo
	glTexCoord4f(0, escala, 0, 1);
	glVertex2f(screenRect1.x - screenRect1.w, screenRect1.y);
	
}

bool RoadSegment::IsDead(){
	
	if((screenRect2.y >= Game::GetInstance().GetWindowHeight())||(screenRect2.y < Game::GetInstance().GetWindowHeight()/2))
		return true;
	
	return false;

}
