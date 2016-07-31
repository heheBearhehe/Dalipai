//
//  main.cpp
//  GameTestCmd
//
//  Created by lugan on 9/13/15.
//
//

#include <iostream>
#include "def.h"
#include<math.h>

void testGame();
void testGameForKeep();
void testGameForKeep(int weight1, int weight2, int count);
void testGameForKeep2(int weight1, int weight2, int weight3, int count);
void testGame(int p1Strategy, int p2Stategy);
void testGame(AIPlayer* ai1, AIPlayer* ai2, int gameCount);

AIPlayer* createOppenentAIPlayer(int charactor);
void testGameForAllCharactor(int gameCount);
void testGameForCharactor(int p1, int p2, int gameCount);

static const int CHARACTOR_GUO     = 1;
static const int CHARACTOR_DAMI    = 2;
static const int CHARACTOR_DAXIONG = 3;
static const int CHARACTOR_DALI    = 4;
static const int CHARACTOR_YUJIE   = 5;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    initUtils();
    enableLog(false);
    
    int count = 10000;
    testGameForAllCharactor(count);
    
//    int testCharactor = 3;
//    for (int i = testCharactor; i <= 6; i++) {
//        if (testCharactor == i) {
//            continue;
//        }
//        testGameForCharactor(testCharactor, i, count);
//    }
//    
//    testCharactor = 2;
//    for (int i = testCharactor; i <= 6; i++) {
//        if (testCharactor == i) {
//            continue;
//        }
//        testGameForCharactor(testCharactor, i, count);
//    }
    
    
//    for (int i = 3; i <= 5; i++) {
//        for (int j = 1; j <= 6; j++) {
//            if(i == j){
//                continue;
//            }
//            testGameForCharactor(i, j, count);
//        }
//    }
    
//    testGameForCharactor(1, 2, count);
//    testGameForCharactor(1, 6, count);
//    testGameForCharactor(2, 6, count);
//        enableLog(true);
//    testGameForKeep();
//    testGameForKeep2(0, 0, 0, 10000);
    //    testGame(3, 0, 1);
    
    
//    testGuessUpProb();
    
    std::cout << "Finish!\n";
    
    int hhh;
    std::cin >> hhh;
    std::cout << hhh;
    

    
    
    return 0;
}


#define STRATEGY_COUNT 4


static float strategyPoints[STRATEGY_COUNT] = {0};
static float weightPoints[] = {0};

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

void testGameForKeep(){
//    int delta = 20;
//    for (int i = 0; i < 100; i+=delta) {
//        for (int j = 0; j < 100; j+=delta) {
//            int weight1 = 990000 + i * 100 + j;
//            int weight2 = 990000 + 6000;
//            testGameForKeep(weight1, weight2, 10000);
//        }
//    }
//    int delta = 200;
//    int min = 0;
//    int max = 1000;
//    for (int i = min; i <= max; i+=delta) {
//        for (int j = min; j <= max; j+=delta) {
//            testGameForKeep2(i , j, 10000);
//        }
    //    }
    
//    testGameForKeep2(2 , 50, 10000);
//    for (int i = 5; i <= 10; i++) {
//        for(int j = 5; j <= 10; j++){
//            testGameForKeep2(i * 10 , j*10, 0, 10000);
//        }
//    }
//
    for (int i = 0; i < 10; i++) {
        testGameForKeep2(50 , 90, 0, 50000);
    }
}

void testGameForKeep2(int weight1, int weight2, int weight3, int count){
    AIPlayer* ai1 = new AIPlayer();
    AIPlayer* ai2 = new AIPlayer();
    ai1->setTag(1);
    ai2->setTag(2);
    
    //    ai1->setGiveStrategy(0);
//    ai1->setStrategy(3);
//    ai1->setDetect(false);
//    ai1->setAttack(false, 0, 0);
//    ai1->setGiveMid(false);
//    ai1->setGiveStrategy(2);
    
    ai1->setGiveProb(0);
    ai1->setStrategy(3);
    ai1->setGiveStrategy(4);
    ai1->setDetect(true);
    ai1->setAttack(true, weight1, weight2);
    ai1->setGiveMid(false);
    
    ai2->setGiveProb(0);
    ai2->setStrategy(3);
    ai2->setGiveStrategy(4);
    ai2->setDetect(true);
    ai2->setAttack(true, weight1, weight2);
    ai2->setGiveMid(false);
//    ai2->setGiveStrategy(0);
//    ai2->setGiveStrategy(weight1);
//    ai2->setGiveProb(weight2);
//    ai2->setGiveStrategyOffset(weight3);
    
    
    int s1[3]={100, 200, 100};
    int s2[3]={100, 200, 100};
    
    ai1->setKeepStrategyWeight(s1);
    ai2->setKeepStrategyWeight(s2);
    
    testGame(ai1, ai2, count);
    
    delete ai1;
    delete ai2;
}

void testGameForKeep(int weight1, int weight2, int count){
    
    AIPlayer* ai1 = new AIPlayer();
    AIPlayer* ai2 = new AIPlayer();
    ai1->setStrategy(3);
    ai2->setStrategy(3);
    
    
    int s1[3]={weight1 / 10000, (weight1 / 100) % 100, weight1 % 100};
    int s2[3]={weight2 / 10000, (weight2 / 100) % 100, weight2 % 100};
    for (int i = 0; i < 3; i++) {
        if (s1[i] == 99) {
            s1[i] = 100;
        }
        if (s2[i] == 99) {
            s2[i] = 100;
        }
    }
    
    ai1->setKeepStrategyWeight(s1);
    ai2->setKeepStrategyWeight(s2);
        
    testGame(ai1, ai2, count);
    
    delete ai1;
    delete ai2;
}

void testGame(int p1Strategy, int p2Strategy){
    AIPlayer* ai1 = new AIPlayer();
    AIPlayer* ai2 = new AIPlayer();
    ai1->setStrategy(p1Strategy);
    ai2->setStrategy(p2Strategy);
    testGame(ai1, ai2, 10000);
}


void testGameForAllCharactor(int gameCount){
    for (int i = 1; i <= 5; i++) {
        for (int j = i + 1; j <= 5; j++) {
            testGameForCharactor(i, j, gameCount);
        }
    }
    
//    for (int i = 1; i <= 6; i++) {
//        for (int j = 1; j <= 6; j++) {
//            if(i == j){
//                continue;
//            }
//            testGameForCharactor(i, j, gameCount);
//        }
//    }
}

void testGameForCharactor(int p1, int p2, int gameCount){
    AIPlayer* ai1 = createOppenentAIPlayer(p1);
    ai1->setTag(1);
    AIPlayer* ai2 = createOppenentAIPlayer(p2);
    ai2->setTag(2);
    
    testGame(ai1, ai2, gameCount);
    
    delete ai1;
    delete ai2;
}

AIPlayer* createOppenentAIPlayer(int charactor){
    AIPlayer* player = new AIPlayer();
    
    int s0[3]={100, 200, 100};
    int s1[3]={1, 4, 10};
    int s2[3]={3, 9, 12};
    switch (charactor) {
        case CHARACTOR_GUO:
            player->setStrategy(2);
            player->setGiveStrategy(0);
            player->setGiveProb(0);
            player->setName(" GUO ");
            break;
            
        case CHARACTOR_DAMI:
            player->setGiveStrategy(1);
            player->setGiveProb(5);
            player->setKeepStrategyWeight(s1);
            player->setNeverGiveMid(true);
            player->setName("DAMI ");
            break;
            
        case CHARACTOR_DAXIONG:
            player->setGiveStrategy(3);
            player->setGiveProb(10);
            player->setName("BEAR ");
            player->setKeepStrategyWeight(s2);
            player->setDetect(false);
            player->setAttack(false, 0, 0);
            break;
            
        case CHARACTOR_DALI:
            player->setGiveStrategy(4);
            player->setGiveProb(30);
            player->setName("DALI ");
            player->setGiveMid(true);
            break;
            
        case CHARACTOR_YUJIE:
            player->setGiveStrategy(4);
            player->setGiveProb(0);
            player->setName("YUJIE");
            break;
            
        case 6:
            player->setGiveStrategy(0);
            player->setGiveProb(0);
            player->setName("KEEP");
            player->setKeepStrategyWeight(s0);
            break;
            
        default:
            break;
    }
    
    return player;
}

void testGame(AIPlayer* ai1, AIPlayer* ai2, int gameCount){
    
    int win = 0;
    int draw = 0;
    int lose = 0;
    int maxPts = 0;
    int minPts = 0;
    int maxWin = 0;
    int totalPts1 = 0;
    int totalPts2 = 0;
    int totalGiveCountP1 = 0;
    int totalGiveCountP2 = 0;
    
    int p2ActionCountTotal = 0;
    int p2GuessCountTotal = 0;
    int p2GuessCountCorrect = 0;
    int p2GuessCountScore = 0;
    int p2FirstPlayCount = 0;
    
    std::vector<int>* p1PtsList = new std::vector<int>();
    std::vector<int>* p2PtsList = new std::vector<int>();
    for (int i = 0; i < gameCount; i++) {
        Game* game = new Game(GAME_MODE::NORMAL, PLAY_MODE::AUTO, GAME_FIRST_PLAYER::RANDOM);
        Player* player1 = new Player();
        Player* player2 = new Player();
        
        player1->setTag(1);
        player2->setTag(2);
        game->setPlayer(player1, player2);
        game->setPlayer1ChoiceListener(ai1);
        game->setPlayer2ChoiceListener(ai2);
        
        game->start();
        game->next();
        
        p1PtsList->push_back(player1->getPoints());
        p2PtsList->push_back(player2->getPoints());
        
//        game->dumpPlayerGuessStat();
        p2ActionCountTotal += game->getP2ActionCountTotal();
        p2GuessCountTotal += game->getP2GuessCountTotal();
        p2GuessCountCorrect += game->getP2GuessCountCorrect();
        p2GuessCountScore += game->getP2GuessCountScore();
        if (!game->getIsPlayer1FirstPlay()) {
            p2FirstPlayCount++;
        }
        delete player1;
        delete player2;
        delete game;
        totalGiveCountP1 += ai1->getGiveCount();
        totalGiveCountP2 += ai2->getGiveCount();
    }
    
    int* p1Pts = new int[gameCount];
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
        
        totalPts1 += p1;
        totalPts2 += p2;
        p1Pts[i] = p1;
//        if (i > gameCount - 10) {
//            LOGF("%3d. p1=[%d]  p2=[%d]  %s", i, p1, p2, msg.c_str());
//        }
    }
    
    
//    LOGF("--------- p2 games=[%d] guess=[%d/%d/%d] guess=[%5.2f%%] guess.correct=[%5.2f%%] score=[%d]", gameCount, p2GuessCountCorrect, p2GuessCountTotal, p2ActionCountTotal, (float)p2GuessCountTotal / p2ActionCountTotal * 100,(float)p2GuessCountCorrect / p2GuessCountTotal * 100,p2GuessCountScore / gameCount);
    
//    float avg1 = totalPts1 / gameCount;
//    float sum = 0;
//    for (int i = 0; i < gameCount; i++) {
//        sum += (p1Pts[i] - avg1) * (p1Pts[i] - avg1);
//    }
//    float variance = sum/ gameCount;
//    float stdD = sqrt(variance);
    
//    LOGF("------- strategy p1=[%d] p2=[%d]", ai1->getStategy(), ai2->getStategy());
//    LOGF("- p1.weight=[%d-%d-%d]", ai1->getKeepStrategyWeight()[0], ai1->getKeepStrategyWeight()[1], ai1->getKeepStrategyWeight()[2]);
//    LOGF("- p2.weight=[%d-%d-%d]", ai2->getKeepStrategyWeight()[0], ai2->getKeepStrategyWeight()[1], ai2->getKeepStrategyWeight()[2]);
//    LOGF("total=[%d] w/d/l=[%d/%d/%d]=[%5.1f%%\t%5.1f%%\t%5.1f%%]", gameCount, win, draw, lose, (float)win * 100 / gameCount, (float)draw * 100 / gameCount, (float)lose * 100 / gameCount );
//    LOGF("max=[%d] min=[%d] maxWin=[%d] avg1=[%5.2f] avg2=[%5.2f]", maxPts, minPts, maxWin, (float)totalPts1 / gameCount, (float)totalPts2 / gameCount);
//    LOGF("------- ");
    strategyPoints[ai1->getStategy()] += (float)win * 100 / gameCount * 3 + (float)draw * 100 / gameCount;
    strategyPoints[ai2->getStategy()] += (float)lose * 100 / gameCount * 3 + (float)draw * 100 / gameCount;
    
    ai2->dumpStat();
    
    LOGF("%s-%s" \
         "\t%2d-%2d" \
//         + "\t%d\t%d"
         "\t%2d-%2d" \
         "\t%3.2f" \
         "\t%5.3f\t%5.3f" \
         "\t%5.2f\t%5.2f" \
         "\t%5.1f%%\t%5.1f%%\t%5.1f%%",
         
         ai1->getName().c_str(), ai2->getName().c_str(),
         ai1->getGiveProb(), ai1->getGiveStrategy(),
//         ai2->getAttackM(), ai2->getAttackL(),
         ai2->getGiveProb(),ai2->getGiveStrategy(),
         (float)p2FirstPlayCount / gameCount,
         (float)totalGiveCountP1 / gameCount, (float)totalGiveCountP2 / gameCount,
         (float)totalPts1 / gameCount, (float)totalPts2 / gameCount,
         (float)win * 100 / gameCount, (float)draw * 100 / gameCount, (float)lose * 100 / gameCount);
}

