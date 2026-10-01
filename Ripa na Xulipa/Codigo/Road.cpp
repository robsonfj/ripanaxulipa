
#include "Road.h"
#include "LoadState.h"
#include "MainCharacter.h"

std::vector<RoadSegment*> Road::segmentos;
int Load(void* ptr);


Road::Road(string arq, int segcount) : spRoad(arq, 1, 0) {
    
    this->segcount = segcount;
    speed = 7;
    roadwidth = 3;
    segheight = 1;
    
    for (int i = 1; i < segcount; i++) {
        float z_pt1 = i * segheight;
        float z_pt2 = (i + 1) * segheight;
        segmentos.emplace_back(new RoadSegment(z_pt1, z_pt2, roadwidth));
    }
    
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			obsMatrix[i][j] = NULL;
		}
	}
    
    initScena();
    stopload = true;

}


void Road::initThread() {

    stopload = false;
    SDL_CreateThread(Load, "LoadThread", this);
    
}


void Road::Update(float dt) {
    speedTimer.Update(dt);
    paperTimer.Update(dt);
	powerTimer.Update(dt);
    materialTimer.Update(dt);
    obsTimer.Update(dt * 1000);
    objTimer.Update(dt);
    
	if (stopload) {
		initThread();
	}
    
    if (speedTimer.Get() > 15 && speed < 12) {
        speed += 0.5;
        cout << speed;
        speedTimer.Restart();
    }
    
//	Se existir menos segmentos que segcount, adiciona mais
    if (segmentos.size() < segcount) {
        float z_pt1 = segmentos.back()->GetZ_World2();
        float z_pt2 = segmentos.back()->GetZ_World2() + 1;
        segmentos.emplace_back(new RoadSegment(z_pt1, z_pt2, roadwidth));
    }
   
    for (int i = 0; i < segmentos.size(); i++) {
        segmentos[i]->Update(dt*speed);
        if (segmentos[i]->IsDead()) {
            segmentos.erase(segmentos.begin() + i);
            i--;
        }
    }
    
    
	for (int i = 0; i < roadObjcts.size(); i++) {
		roadObjcts[i]->Update(dt);
	}
    
}


int Load(void* ptr) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        cout << SDL_GetError() << std::endl;
        exit(1);
    }
    
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF);
    
    Mix_Init(MIX_INIT_FLAC|MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_MOD);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    
    TTF_Init();
    
    SDL_GL_MakeCurrent(Game::GetInstance().window, Game::GetInstance().threadctx);
    Road* load = (Road*) ptr;
    while (!load->stopload) {
        glFlush();
        if (load->obsTimer.Get() > 700 + 100 * rand() % 3) {
            load->AddObstacles();
            load->obsTimer.Restart();
        }
        
        if (load->objTimer.Get() > 1 + rand() % 2) {
            load->AddRoadObjs(load->segcount / 2);
            load->objTimer.Restart();
        }
        SDL_Delay(50);
    }
    glFinish();
    SDL_GL_MakeCurrent(Game::GetInstance().window, NULL);
    return 0;

}


void Road::Render() {

	glFlush();
//	Atribui a textura
    glBindTexture(GL_TEXTURE_2D, *spRoad.GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBegin(GL_QUADS);
    
//	Tipo de primitiva com quatro vertices
	for (int i = 0; i < segmentos.size(); i++) {
		segmentos[i]->Render(spRoad);
	}
    
    glEnd();
    glFinish();
	for (int i = roadObjcts.size() - 1; i >= 0; i--) {
		roadObjcts[i]->Render();
	}

}


void Road::initScena() {
    
    while (pos <= segcount / 2) {
//		Inicializa a cena os objetos e predios
        AddRoadObjs(pos);
        pos += 5;
//      Informa a porcentagem para a tela e carregamento
        LoadState::percent += 2;
    }

}


void Road::AddRoadObjs(int pos) {

	std::stringstream randtree, randpred1, randpred2;
    
    roadObjcts.emplace_back(new Objetos("poste","arquivos/img/states/stage/roadobj/posteinv.png", pos, -0.5, 1, 5));
    
	randtree<<"arquivos/img/states/stage/roadobj/tree_"<< 1+rand()%3<<".png";
    
    int tam = 1+rand()%2;
    int num1, num2;
    if (tam ==1) {
        num1 = 7;
        num2 = 6;
    }
    else{
        num1 = 4;
        num2 = 3;
    }
    
    randpred1<<"arquivos/img/states/stage/roadobj/predios/"<<tam<<"_"<<1+rand()%num1<<".png";
    randpred2<<"arquivos/img/states/stage/roadobj/predios/"<<tam<<"-"<<1+rand()%num2<<".png";
    
//   construcao aleatoria de objetos
    switch (rand()%6) {
        case 0:
        case 1:
            roadObjcts.emplace_back(new Buildings(randpred1.str(), randpred2.str(), pos, (pos)+3, false, 2));
            randpred1.str("");
            randpred2.str("");
            tam = 1+rand()%2;
            if (tam ==1) {
                num1 = 7;
                num2 = 6;
            }
            else{
                num1 = 4;
                num2 = 3;
            }
            randpred1<<"arquivos/img/states/stage/roadobj/predios/"<<tam<<"_"<<1+rand()%num1<<".png";
            randpred2<<"arquivos/img/states/stage/roadobj/predios/"<<tam<<"-"<<1+rand()%num2<<".png";
            
            roadObjcts.emplace_back(new Buildings(randpred1.str(), randpred2.str(), pos, (pos)+3, true, 2));
            break;
            
        case 2:
            roadObjcts.emplace_back(new Buildings(randpred1.str(), randpred2.str(), pos, (pos)+3, true, 2));
            roadObjcts.emplace_back(new Objetos("arvore", randtree.str(), pos, 1.5, 0.9, 1.5));
            break;
            
        case 3:
            roadObjcts.emplace_back(new Buildings(randpred1.str(), randpred2.str(), pos, (pos)+3, false, 2));
            roadObjcts.emplace_back(new Objetos("arvore", randtree.str(), pos, -1.8, 0.9, 1.5));
            break;
            
        case 4:
            roadObjcts.emplace_back(new Objetos("arvore", randtree.str(), pos, 1.5, 0.9, 1.5));
            roadObjcts.emplace_back(new Objetos("arvore", randtree.str(), pos, -1.8, 0.9, 1.5));
            break;
            
        default:
            break;
    }
    
    randtree.str("");
    randpred1.str("");
    randpred2.str("");
    
}


void Road::AddObstacles() {

	std::stringstream randobs, randpower, randmat;

//  Posicoes das 3 lanes para objetos do jogo
    float xpos[3] = {0.3, 0.65, 0.85};
//  randpos ficara com posicoes aleatorias e diferentes de 0 a 2
//  Para escolher o xpos
    int i = 0, randpos[3]= {-1, -1, -1};
//  Posicao aleatoria do segmento a partir da metade
    int temp = rand() % 3;
    
    while (i < 3) {
//      So adiciona se for diferente da posicao anterior
        for (int j = 0; j < 3; j++) {
            if (randpos[j]== temp) {
                temp = rand()%3;
                j = -1;
            }
        }
        randpos[i] = temp;
        i++;
    }
    
    i = 0;
    srand(time(NULL));
    int preencher = 1 + rand() % 2;
    bool obsused = false;
    while (preencher > 0) {
        if (obsMatrix[2][randpos[i]] == NULL) {
            preencher -= 1;
            switch (rand() % 10) {
                case 0:
				case 1:
				case 2:
				case 3: //50 porcento de chance de ser um obstaculo
				case 4:
                    switch (rand() % 2) {
                        case 0:
                            
                            randobs<<"arquivos/img/states/stage/obstacles/obs"<<std::to_string(1+rand()%8)<<".png";
                            for (int j = 0; j<used.size(); j++)
                                if (randobs.str() == used[j]){
                                    obsused = true;
                                    break;
                                }
                            if (!obsused) {
                                obsMatrix[2][randpos[i]] = new Objetos("obstaculo", randobs.str(), segcount / 2, xpos[randpos[i]], 1, 1);
                                used.emplace_back(randobs.str());
                                break;
                            }
                            preencher += 1;
                            break;
                        case 1:
                            for (int j = 0; j<used.size(); j++)
                                if (used[j] == "arquivos/img/states/stage/obstacles/hole1.png"){
                                    obsused = true;
                                    break;
                                }
                            if (!obsused){
                            obsMatrix[2][randpos[i]] =new ObjPespectiva("arquivos/img/states/stage/obstacles/hole1.png", segcount/2, xpos[randpos[i]], 1, 2);
                                used.emplace_back("arquivos/img/states/stage/obstacles/hole1.png");
                                break;
                            }
                            preencher += 1;
                            break;
                        default:
                            break;
                    }
                    break;
                case 5:
				case 6: // 30 porcento de chance
				case 7:
                    if (papercount < 100 && paperTimer.Get() > 1) {
                        switch (rand() % 10) {
                            case 0:
							case 1:
                            case 2:
                            case 3:
                                
                                obsMatrix[2][randpos[i]] = new Objetos("paper", "arquivos/img/states/stage/obstacles/paper.png", segcount/2, xpos[randpos[i]], 1, 1);
                                papercount+=1;
                                paperTimer.Restart();
                                break;
							case 4:
							case 5:
							case 6:
							case 7:
                                for (int i = 0; i < 3; i++) {
                                    if (obsMatrix[2][i]) {
                                        if (obsMatrix[2][i]->jumpOver == true) {
                                            obsMatrix[1][i] = new Objetos("paper", "arquivos/img/states/stage/obstacles/paper.png", segcount/2, xpos[i], 3, 1);
                                            papercount+=1;
                                            paperTimer.Restart();
                                            preencher += 1;
                                            break;
                                        }
                                    }
                                    else{
                                        obsMatrix[1][i] = new Objetos("paper", "arquivos/img/states/stage/obstacles/paper.png", segcount/2, xpos[i], 3, 1);
                                        papercount+=1;
                                        paperTimer.Restart();
                                        break;
                                    }
                                }
                                break;
                                
                            case 8:
                            case 9:
                                if (materialTimer.Get()>15) {
                                    randmat<<"arquivos/img/states/stage/materiais/mat"<<std::to_string(1+rand()%6)<<".png";
                                    obsMatrix[2][i] = new Objetos("material", randmat.str(), segcount / 2, xpos[randpos[i]], 1, 1);
                                    materialTimer.Restart();
                                }
                                break;
                                
                            default:
                                break;
                        }
                        break;
                    }
                    preencher += 1;
                    break;
                case 8:
				case 9: //20 porcento de chance
                    if (powerTimer.Get()>10) {
                        randpower<<"arquivos/img/states/stage/powerups/powerup"<<std::to_string(1+rand()%3)<<".png";
                        obsMatrix[2][randpos[i]] =new PowerUp(randpower.str(), segcount/2, xpos[randpos[i]], 1, 1.5);
                        powerTimer.Restart();
					}
					preencher +=1;
                    break;
                default:
                    break;
            }
        }
        obsused = false;
		if (used.size() > 8) {
			used.clear();
		}
        
        randmat.str("");
		randpower.str("");
        randobs.str("");
        i++;
		if (i > 2) {
			i = 0;
		}
    }
    
    for (int i = 0; i < 3; i++) {
        obsMatrix[0][i] = NULL;
    }
    
//  Adiciona todos os objetos
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (obsMatrix[i][j] != NULL) {
				Game::GetCurrentState().AddObject(obsMatrix[i][j]);
				obsMatrix[i][j] = NULL;
			}
		}
	}
    
}


Road::~Road() {

	stopload = true;
    segmentos.clear();
    roadObjcts.clear();
    
}