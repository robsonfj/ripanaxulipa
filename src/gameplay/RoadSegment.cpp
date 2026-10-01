// RoadSegment.cpp - fatia da pista com projecao perspectiva mundo->tela.

#include "RoadSegment.h"
#include "Game.h"


RoadSegment::RoadSegment(float z_worldpt1, float z_worldpt2, float roadWidth) {
	
	x_world = -4;
	y_world = 1;
	this->z_worldpt1 = z_worldpt1;
	this->z_worldpt2 = z_worldpt2;
	this->roadWidth = roadWidth * 2;

}


void RoadSegment::Update(float speed) {

	double halfScreenWidth = Game::GetInstance().GetWindowWidth() / 2;

//  Ajustado para que a rua fique no horizonte da imagem, que nao esta exatamente no meio
    double halfScreenHeight = Game::GetInstance().GetWindowHeight() * 0.59;
	
	x_world = -4- Camera::pos.x;;
	y_world = 1-Camera::pos.y;;
	
//	Seta a velocidade da pista
    z_worldpt1 -= speed ;
    z_worldpt2 -= speed;
    
//	Epsilon em vez de >= 0: z proximo de zero gera inf/NaN na
//	projecao perspectiva (quads gigantes ou lixo no driver).
    const float zMin = 0.1f;
    if (z_worldpt1 > zMin && z_worldpt2 > zMin) {
//	Transformacoes dos pontos do "Mundo" para os pontos na tela
        screenRect1.x = halfScreenWidth + (1 + (x_world / z_worldpt1) * halfScreenWidth);
        screenRect2.x = halfScreenWidth + (1 + (x_world / z_worldpt2) * halfScreenWidth);
        
        screenRect1.y = halfScreenHeight+ (1 + (y_world / z_worldpt1) * halfScreenHeight * 2);
        screenRect2.y = halfScreenHeight+ (1 + (y_world / z_worldpt2) * halfScreenHeight * 2);
        
        screenRect1.w = 1 + (roadWidth / z_worldpt1) * halfScreenWidth;
        screenRect2.w = 1 + (roadWidth / z_worldpt2) * halfScreenWidth;
    }

}


void RoadSegment::Render(Sprite spRoad) {

	float escala = 1.0f;
	if (screenRect1.w != 0) {
		escala = screenRect2.w / screenRect1.w;
	}
	
//	Ponto superior esquerdo
	glTexCoord4f(0, 0, 0, escala);
	glVertex2f(screenRect2.x - screenRect2.w, screenRect2.y);
	
//  Ponto superior direito
	glTexCoord4f(escala * spRoad.GetTexU(), 0, 0, escala);
	glVertex2f( screenRect2.x + screenRect2.w, screenRect2.y);
	
//  Ponto inferior direito
	glTexCoord4f(spRoad.GetTexU(), spRoad.GetTexV(), 0, 1);
	glVertex2f( screenRect1.x + screenRect1.w, screenRect1.y);
	
//  Ponto inferior esquerdo
	glTexCoord4f(0, spRoad.GetTexV(), 0, 1);
	glVertex2f(screenRect1.x - screenRect1.w, screenRect1.y);
	
}


bool RoadSegment::IsDead() {
	
	if (z_worldpt2 < 1) {
		return true;
	}    
	return false;

}
