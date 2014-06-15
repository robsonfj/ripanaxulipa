//
//  EndState.cpp
//  Avenida Paulista
//
//  Created by Robson Ferreira Jacomini on 12/06/14.
//
//

#include "EndState.h"
#include "StageState.h"

EndState::EndState(bool win){
    std::stringstream s;
    rectRed = *new ColorRect(0, 0, 1024, 600);
    
	txNota = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Nota: SR", padraoCor, 512, 150);
	if (win) {
		
        s.str("");
        if (MainCharacter::papers == 0) {
            s << "Nota: SR";
        }
        else {
            if (MainCharacter::papers > 0 && MainCharacter::papers < 18) {
                s << "Nota: II";
            }
            else {
                if (MainCharacter::papers >= 18 && MainCharacter::papers < 30) {
                    s << "Nota: MI";
                }
                else {
                    if (MainCharacter::papers >= 30 && MainCharacter::papers < 42) {
                        s << "Nota: MM";
                    }
                    else {
                        if (MainCharacter::papers >= 42 && MainCharacter::papers < 54) {
                            s << "Nota: MS";
                        }
                        else {
                            if (MainCharacter::papers >= 54) {
                                s << "Nota: SS";
                            }
                        }
                    }
                }
            }
        }
		txNota.SetText(s.str());

		txResultado = *new Text(FONTE, 100, Text::TEXT_BLENDED, "Win!", padraoCor, 512, 280);
        
	}
	else {
		txResultado = *new Text(FONTE, 100, Text::TEXT_BLENDED, "Lose!", padraoCor, 512, 280);
	}
    
    txScore = *new Text(FONTE, 60, Text::TEXT_BLENDED, "Points", padraoCor, 150, 250);
    txHighscore = *new Text(FONTE, 60, Text::TEXT_BLENDED, "High Score", padraoCor, 850, 250);
    
    txNscore = *new Text(FONTE, 50, Text::TEXT_BLENDED, "0", padraoCor, 150, 320);
    txNhighscore = *new Text(FONTE, 50, Text::TEXT_BLENDED, "0", padraoCor, 800, 320);
    
    txContinue = *new Text(FONTE, 50, Text::TEXT_BLENDED,"Menu", padraoCor, 512, 380);
    txRepeat = *new Text(FONTE, 50, Text::TEXT_BLENDED,"Repeat?", padraoCor, 512, 450);
    
}

void EndState::Input(){
    
//  Se a tecla for ESC, setar a flag para deletar esse estado
    if((InputManager::GetInstance().KeyPress(ESCAPE_KEY))){
        requestDelete = true;
    }
    
//	se condicao de saida for atendido
    if(InputManager::GetInstance().ShouldQuit())
        requestQuit = true;
    
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
    if (InputManager::GetInstance().IsMouseInside(txContinue.box)) {
        
        txContinue.SetColor(padraoCorSelect);
        if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
            requestDelete = true;
        }
    }
    else
        txContinue.SetColor(padraoCor);
    
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
    if (InputManager::GetInstance().IsMouseInside(txRepeat.box)) {
        
        txRepeat.SetColor(padraoCorSelect);
        if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
            requestDelete = true;
            Game::GetInstance().Push(new StageState);
        }
    }
    else
        txRepeat.SetColor(padraoCor);
    
}


void EndState::Update(float dt) {
    std::stringstream s;
    Input();
    
    s.str("");
    s <<MainCharacter::plpoints;
    txNscore.SetText(s.str());
    s.str("");
    s <<Game::GetInstance().GetHighScore();
    txNhighscore.SetText(s.str());
}


void EndState::Render(){
    
    rectRed.Render(1);// passa 1 para definir a cor vermelha
	txNota.Render();
	txResultado.Render();
    txScore.Render();
    txHighscore.Render();
    txNscore.Render();
    txNhighscore.Render();
    txContinue.Render();
    txRepeat.Render();
    
}