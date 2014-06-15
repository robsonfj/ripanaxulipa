//
//  CheatMode.h
//  Ripa na Xulipa
//
//  Created by Robson Ferreira Jacomini on 15/06/14.
//  Copyright (c) 2014 Robson Ferreira Jacomini. All rights reserved.
//

#include "Game.h"
#include "MainCharacter.h"


class CheatMode{
    string cheat;
    
public:
    CheatMode(){};
    ~CheatMode(){};
    
    void SetCheat(string cheat){this->cheat = cheat;};
    
    void Update();
};