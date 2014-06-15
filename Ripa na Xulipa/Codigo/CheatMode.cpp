//
//  CheatMode.cpp
//  Ripa na Xulipa
//
//  Created by Robson Ferreira Jacomini on 15/06/14.
//  Copyright (c) 2014 Robson Ferreira Jacomini. All rights reserved.
//

#include "CheatMode.h"

void CheatMode::Update(){
    
    
    if (cheat == "infintang") {
        MainCharacter::player->SetIntang(0.6);
    }
    
    if (cheat == "nointang") {
        MainCharacter::player->SetIntang(1);
    }
    
    
    
}
