//
//  main.cpp
//  GameTestCmd
//
//  Created by lugan on 9/13/15.
//
//

#include <iostream>
#include "def.h"


void testGame();
void testGame(int p1Strategy, int p2Stategy);
void testGame(int p1Strategy, int p2Strategy, int gameCount);

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    initUtils();
    enableLog(false);
    
//    testGame();
    
    enableLog(true);
    testGame(3, 0, 1);
    return 0;
}

#define STRATEGY_COUNT 4


static float strategyPoints[STRATEGY_COUNT] = {0};

void testGame(){
    int strategyCount = STRATEGY_COUNT;
    
//    for (int i = 0; i < strategyCount; i++) {
//        testGame(i, i);
//    }
    
    for (int i = 0; i < strategyCount; i++) {
        testGame(3, i);
        
    }
    
//    for (int i = 0; i < strategyCount; i++) {
//        for (int j = i + 1; j < strategyCount; j++) {
//            testGame(i, j);
//        }
//    }
    
    for (int i = 0; i < strategyCount; i++) {
        LOGF("s%d.pts=[%5.1f]", i, strategyPoints[i]);
    }
    
}

void testGame(int p1Strategy, int p2Strategy){
    testGame(p1Strategy, p2Strategy, 10000);
}

void testGame(int p1Strategy, int p2Strategy, int gameCount){
    
    int win = 0;
    int draw = 0;
    int lose = 0;
    int maxPts = 0;
    int minPts = 0;
    int maxWin = 0;
    
    std::vector<int>* p1PtsList = new std::vector<int>();
    std::vector<int>* p2PtsList = new std::vector<int>();
    for (int i = 0; i < gameCount; i++) {
        Game* game = new Game(GAME_MODE::NORMAL);
        Player* player1 = new Player();
        Player* player2 = new Player();
        AIPlayer* ai1 = new AIPlayer(game);
        ai1->setStrategy(p1Strategy);
        AIPlayer* ai2 = new AIPlayer(game);
        ai2->setStrategy(p2Strategy);
        
        player1->setTag(1);
        player2->setTag(2);
        game->setPlayer(player1, player2);
        game->setPlayer1ChoiceListener(ai1);
        game->setPlayer2ChoiceListener(ai2);
        
        game->start();
        game->next();
        
        p1PtsList->push_back(player1->getPoints());
        p2PtsList->push_back(player2->getPoints());
        
        delete ai1;
        delete ai2;
        delete player1;
        delete player2;
        delete game;
    }
    
    for (int i = 0; i < gameCount; i++) {
        string msg;
        int p1 = p1PtsList->at(i);
        int p2 = p2PtsList->at(i);
        
        if(p1 > p2){
            win++;
            msg = "win";
        }else if(p1 < p2){
            lose++;
            msg = "lose";
        }else{
            draw++;
            msg = "draw";
        }
        
        if (std::max(p1, p2) > maxPts) {
            maxPts = std::max(p1, p2);
        }
        if (std::min(p1, p2) < minPts) {
            minPts = std::min(p1, p2);
        }
        if (abs(p1 - p2) > maxWin) {
            maxWin = abs(p1 - p2);
        }
        
//        if (i > gameCount - 10) {
//            LOGF("%3d. p1=[%d]  p2=[%d]  %s", i, p1, p2, msg.c_str());
//        }
    }
    
    LOGF("--- strategy p1=[%d] p2=[%d]", p1Strategy, p2Strategy);
    LOGF("total=[%d] w/d/l=[%d/%d/%d]=[%5.1f%%\t%5.1f%%\t%5.1f%%]", gameCount, win, draw, lose, (float)win * 100 / gameCount, (float)draw * 100 / gameCount, (float)lose * 100 / gameCount );
    LOGF("max=[%d] min=[%d] maxWin=[%d]", maxPts, minPts, maxWin);
    
    strategyPoints[p1Strategy] += (float)win * 100 / gameCount * 3 + (float)draw * 100 / gameCount;
    strategyPoints[p2Strategy] += (float)lose * 100 / gameCount * 3 + (float)draw * 100 / gameCount;
    
}