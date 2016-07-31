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

