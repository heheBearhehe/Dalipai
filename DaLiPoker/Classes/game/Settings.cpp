//
//  Settings.cpp
//  DaLiPoker
//
//  Created by lugan on 10/18/15.
//
//

#include "Settings.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <ostream>
#include "../model/def.h"

Settings* Settings::sInstance = NULL;

Settings* Settings::getInstance(){
    if (sInstance == NULL) {
        sInstance = new Settings();
    }
    return sInstance;
}

void Settings::destroy(){
    delete sInstance;
    sInstance = NULL;
}

Settings::Settings(){
    opponentCharacter = 0;
    myAvatar = 0;
    firstPlayer = 0;
    backgroundMusic = true;
    soundEffect = true;
    
    for(int i = 0; i < CHARACTOR_TOTAL_COUNT; i++){
        this->totalResultList[i] = new vector<GameResult *>();
    }
    
// test code
//    int oppoCharacter = 2;
//    
//    int gameCount = 100;
//    std::vector<GameResult *>* gameResultList = new std::vector<GameResult *>();
//    for (int i = 0; i < gameCount; i++) {
//        Game* game = new Game(GAME_MODE::NORMAL, PLAY_MODE::AUTO, GAME_FIRST_PLAYER::RANDOM);
//        Player* player1 = new Player();
//        Player* player2 = new Player();
//        
//        AIPlayer* ai1 = GameManager::getInstance()->createOppenentAIPlayer(game, 5);
//        AIPlayer* ai2 = GameManager::getInstance()->createOppenentAIPlayer(game, oppoCharacter);
//        
//        player1->setTag(1);
//        player2->setTag(2);
//        game->setPlayer(player1, player2);
//        game->setPlayer1ChoiceListener(ai1);
//        game->setPlayer2ChoiceListener(ai2);
//        
//        game->start();
//        game->next();
//        
//        GameResult* gameResult = new GameResult();
//        gameResult->myPoints = player1->getPoints();
//        gameResult->oppoPoints = player2->getPoints();
//        gameResult->myMaxCombo = player1->getMaxCombo();
//        gameResult->oppoMaxCombo = player2->getMaxCombo();
//        gameResult->updateResult();
//        
//        
//        gameResultList->push_back(gameResult);
//        
//        
//        delete player1;
//        delete player2;
//        delete game;
//        delete ai1;
//        delete ai2;
//        
//        this->addResult(oppoCharacter, gameResult);
//    }
//    
//    GameStat* stat = new GameStat();
//    stat->calcStat(gameResultList);
//    
//    saveStat(oppoCharacter);
}

void Settings::load(){
    auto path = cocos2d::FileUtils::getInstance()->getWritablePath();
    path.append(SETTINGS_FILENAME);
    if(!cocos2d::FileUtils::getInstance()->isFileExist(path)){
        save();
        return;
    }
    
    rapidjson::Document readdoc;
    std::string loadStr;
    
    loadStr = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
    readdoc.Parse<0>(loadStr.c_str());
    if(readdoc.HasParseError())
    {
        CCLOG("GetParseError%s\n", readdoc.GetParseError());
    }
    
    if(!readdoc.IsObject()){
        return;
    }

    this->opponentCharacter = readdoc["opponentCharacter"].GetInt();
    this->myAvatar = readdoc["myAvatar"].GetInt();
    this->firstPlayer = readdoc["firstPlayer"].GetInt();
    this->backgroundMusic = readdoc["backgroundMusic"].GetBool();
    this->soundEffect = readdoc["soundEffect"].GetBool();
    
    this->loadAllStat();
}

void Settings::save(){
    rapidjson::Document writedoc;
    writedoc.SetObject();
    rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
    
    writedoc.AddMember("opponentCharacter", this->opponentCharacter, allocator);
    writedoc.AddMember("myAvatar", this->myAvatar, allocator);
    writedoc.AddMember("firstPlayer", this->firstPlayer, allocator);
    writedoc.AddMember("backgroundMusic", this->backgroundMusic, allocator);
    writedoc.AddMember("soundEffect", this->soundEffect, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writedoc.Accept(writer);
    
    auto path = cocos2d::FileUtils::getInstance()->getWritablePath();
    path.append(SETTINGS_FILENAME);
    FILE* file = fopen(path.c_str(), "wb");
    if(file)
    {
        fputs(buffer.GetString(), file);
        fclose(file);
    }
}

void Settings::addResult(int charactor, GameResult* gameResult){
    totalResultList[charactor]->push_back(gameResult);
    saveStat(charactor);
    
    totalResultList[CHARACTOR_TOTAL]->push_back(gameResult);
    saveStat(CHARACTOR_TOTAL);
}

void Settings::loadAllStat(){
    for (int i = 0; i < CHARACTOR_TOTAL_COUNT; i++) {
        loadStat(i);
    }
}

void Settings::saveAllStat(){
    for (int i = 0; i < CHARACTOR_TOTAL_COUNT; i++) {
        saveStat(i);
    }
}

void Settings::loadStat(int charactor){
    auto path = cocos2d::FileUtils::getInstance()->getWritablePath();
    path.append(getStatFilePath(charactor));
    if(!cocos2d::FileUtils::getInstance()->isFileExist(path)){
        saveStat(charactor);
        return;
    }
    
    rapidjson::Document readdoc;
    std::string loadStr;
    
    loadStr = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
    readdoc.Parse<0>(loadStr.c_str());
    if(readdoc.HasParseError())
    {
        CCLOG("GetParseError%s\n", readdoc.GetParseError());
    }
    
    if(!readdoc.IsObject()){
        return;
    }
    
    vector<GameResult *>* resultList = totalResultList[charactor];
    for (rapidjson::SizeType i = 0; i < resultList->size(); i++){
        delete resultList->at(i);
    }
    resultList->clear();
    
    
    const rapidjson::Value&jsonResultList = readdoc["results"];
    
    assert(jsonResultList.IsArray());
    for (rapidjson::SizeType i = 0; i < jsonResultList.Size(); i++){
        const rapidjson::Value& jsonResult = jsonResultList[i];
        if(jsonResult.IsArray() && jsonResult.Size() >= 5){
            GameResult* result = new GameResult();
            result->result = jsonResult[(int)0].GetInt();
            result->myMaxCombo = jsonResult[1].GetInt();
            result->myPoints = jsonResult[2].GetInt();
            result->oppoMaxCombo = jsonResult[3].GetInt();
            result->oppoPoints = jsonResult[4].GetInt();
            
            resultList->push_back(result);
        }
    }
}

void Settings::saveStat(int charactor){
    rapidjson::Document writedoc;
    writedoc.SetObject();
    rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
    
    vector<GameResult *>* resultList = totalResultList[charactor];
    rapidjson::Value statList(rapidjson::kArrayType);
    
    for(int i = 0; i < resultList->size(); i++){
        GameResult* gameResult = resultList->at(i);
        gameResult->updateResult();
        
        rapidjson::Value infoList(rapidjson::kArrayType);
        infoList.PushBack(gameResult->result, allocator);
        infoList.PushBack(gameResult->myMaxCombo, allocator);
        infoList.PushBack(gameResult->myPoints, allocator);
        infoList.PushBack(gameResult->oppoMaxCombo, allocator);
        infoList.PushBack(gameResult->oppoPoints, allocator);
        
        statList.PushBack(infoList, allocator);
    }
    
    writedoc.AddMember("results", statList, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writedoc.Accept(writer);

    auto path = cocos2d::FileUtils::getInstance()->getWritablePath();
    path.append(getStatFilePath(charactor));
    FILE* file = fopen(path.c_str(), "wb");
    if(file)
    {
        fputs(buffer.GetString(), file);
        fclose(file);
    }
}

std::string Settings::getStatFilePath(int charactor){
    std::ostringstream os;
    os << "stat_" << charactor << ".json";
    return os.str();
}

GameStat& Settings::getGameStat(int charactor){
    vector<GameResult *>* resultList = totalResultList[charactor];
    GameStat& stat = totalStatForCharacter[charactor];
    stat.calcStat(resultList);
    return stat;
}



