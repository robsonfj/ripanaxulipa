

#include "Road.h"

std::vector<RoadSegment*> Road::segmentos;


Road::Road(string arq, int segcount): spRoad(arq, 1, 0){
    
    this->segcount = segcount;
    speed = 0.2;
    roadwidth = 3;
    segheight = 1;
    
    for (int i = 0; i < segcount; i++) {
        float z_pt1 = i*segheight;
        float z_pt2 = (i+1)*segheight;
        segmentos.emplace_back(new RoadSegment(z_pt1, z_pt2, speed, roadwidth));
    }
    
}

void Road::Update(float dt){
    
    //	se existir menos segmentos que segcount, adiciona mais
    if (segmentos.size() < segcount) {
        float z_pt1 = segmentos.back()->GetZ_World2();
        float z_pt2 = segmentos.back()->GetZ_World2()+1;
        segmentos.emplace_back(new RoadSegment(z_pt1, z_pt2, speed, roadwidth));
    }
    
    for (int i = 0; i < segmentos.size(); i++) {
        segmentos[i]->Update(dt);
        
        if (segmentos[i]->IsDead())
            segmentos.erase(segmentos.begin()+ i);
    }
    
    //  so e executada uma vez
    initScene();
    
    obsTimer.Update(dt);
    if (obsTimer.Get() > 3+rand()%7) {
        AddObstacles();
        obsTimer.Restart();
    }
    
    objTimer.Update(dt);
    if (objTimer.Get() > 1) {
        AddRoadObjs(segcount/2);
        objTimer.Restart();
    }
    
}

void Road::Render(){
    
    //	atribui a textura
    glBindTexture(GL_TEXTURE_2D, spRoad.GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    
    //	tipo de primitiva com quatro vertices
    for (int i = 0; i< segcount; i++) {
        segmentos[i]->Render(spRoad);
    }
    glEnd();
}


void Road::initScene(){
    
    while (pos <= segcount/2) {
//		inicializa a cena os objetos e predios
        AddRoadObjs(pos);
        pos += 5;
    }
}

void Road::AddRoadObjs(int pos){
    
    //        Game::GetCurrentState().AddObject(new Objetos("poste","arquivos/img/roadobj/poste.png", pos, 0.5, 1, 5));
    Game::GetCurrentState().AddObject(new Objetos("poste","arquivos/img/roadobj/posteinv.png", pos, -0.5, 1, 5));
    
    //        construcao aleatoria de objetos
    switch (rand()%6) {
        case 0:
            
            Game::GetCurrentState().AddObject(new Buildings(pos, (pos)+3, false, 2+rand()%3));
            Game::GetCurrentState().AddObject(new Buildings(pos, (pos)+3, true, 2+rand()%3));
            break;
            
        case 1:
            Game::GetCurrentState().AddObject(new Buildings(pos, (pos)+3, true, 2+rand()%3));
            Game::GetCurrentState().AddObject(new Objetos("arvore","arquivos/img/roadobj/tree.png", pos, 1.5, 1, 1));
            break;
            
        case 2:
            Game::GetCurrentState().AddObject(new Buildings(pos, (pos)+3, false, 2+rand()%3));
            Game::GetCurrentState().AddObject(new Buildings(pos, (pos)+3, true, 2+rand()%3));
            break;
            
        case 3:
            Game::GetCurrentState().AddObject(new Buildings(pos, (pos)+3, false, 2+rand()%3));
            Game::GetCurrentState().AddObject(new Objetos("arvore","arquivos/img/roadobj/tree.png", pos, -1.5, 1, 1));
            break;
            
        case 4:
            Game::GetCurrentState().AddObject(new Objetos("arvore","arquivos/img/roadobj/tree.png", pos, 1.5, 1, 1));
            Game::GetCurrentState().AddObject(new Objetos("arvore","arquivos/img/roadobj/tree.png", pos, -1.5, 1, 1));
            break;
            
        default:
            break;
    }
    
    
}

void Road::AddObstacles(){
    //  posicoes das 3 lanes para objetos do jogo
    float xpos[3] = {0.2, 0.6, 0.85};
    //  randpos ira ficar com posicoes aleatorias e diferentes de 0 a 2
    //   para escolher o xpos
    int i = 0, randpos[3]= {-1, -1, -1};
//    posicao aleatoria do segmento a partir da metade
    float randseg[3] = {1, 1.15, 1.05};
    int temp = rand()%3;
    while (i < 3) {
        //      so adiciona se for diferente da posicao anterior
        for (int j = 0; j < 3; j++) {
            if (randpos[j]== temp){
                temp = rand()%3;
                j = 0;
            }
        }
        randpos[i] = temp;
        i++;
    }
    
    if (papercount < 60) {
     Game::GetCurrentState().AddObject(new Objetos("paper", "arquivos/img/obstacles/paper.png", segcount/2, xpos[randpos[2]], 0, 1));
        papercount ++;
    }
    
//     construcao aleatoria de objetos
    switch (0/*rand()%3*/) {
        case 0:
            Game::GetCurrentState().AddObject(new Objetos("obstaculo", "arquivos/img/obstacles/workerstop.png", randseg[randpos[0]]*segcount/2, xpos[randpos[0]], 0, 0.25));
            Game::GetCurrentState().AddObject(new Objetos("obstaculo", "arquivos/img/obstacles/workerstop.png", randseg[randpos[1]]*segcount/2, xpos[randpos[1]], 0, 0.25));
            break;
            
        case 1:
            Game::GetCurrentState().AddObject(new ObjPespectiva("arquivos/img/obstacles/hole.png", 10, xpos[1], 0, 1));
            
            break;
            
        case 2:
            Game::GetCurrentState().AddObject(new ObjPespectiva("arquivos/img/obstacles/sewage.png", 10, xpos[1], 0.5, 0.5));
            break;
            
        case 3:
            
            break;
            
        case 4:
            
            break;
            
        default:
            break;
    }
    
    
}


