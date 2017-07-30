//
//  DLUtils.mm
//  DaLiPoker
//
//  Created by lugan on 9/4/16.
//
//


#ifdef __APPLE__
#import "DLUtils.h"
#import <UIKit/UIKit.h>

void DLUtils::sendEmail(){
    NSMutableString *mailUrl = [[NSMutableString alloc] init];
    [mailUrl appendFormat:@"mailto:%s", DL_FEEDBACK_EMAIL_ADDRESS.c_str()];
    [mailUrl appendFormat:@"?subject=%@", [[NSString alloc] initWithUTF8String:DL_FEEDBACK_EMAIL_SUBJECT.c_str()]];
    
    NSString * encodingString = [mailUrl stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:encodingString]];
}

double DLUtils::getCurrentTime(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000;
}


void DLUtils::openVideoUrl(const std::string& url){
    if (url.length() == 0) {
        return;
    }
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
}

#else
#include "DLUtils.h"
#include "jni/JniHelper.h"

inline jstring util_cstr2jstr(JNIEnv* env, const char* str) {
    int size = strlen(str);
    jbyteArray array = env->NewByteArray(size);
    env->SetByteArrayRegion(array, 0, size, (const jbyte*) str);
    jstring strEncode = env->NewStringUTF("UTF-8");
    jclass cls = env->FindClass("java/lang/String");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "([BLjava/lang/String;)V");
    jstring jstr = (jstring) env->NewObject(cls, ctor, array, strEncode);
    env->DeleteLocalRef(cls);
    env->DeleteLocalRef(array);
    env->DeleteLocalRef(strEncode);
    return jstr;
}

void DLUtils::sendEmail(){
    cocos2d::JniMethodInfo info;
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(info,"com/xinyu/game/dalipoker/Utils","sendEmail","(Ljava/lang/String;Ljava/lang/String;)V");
    if(ret){
        jstring address = util_cstr2jstr(info.env, DL_FEEDBACK_EMAIL_ADDRESS.c_str());
        jstring subject = util_cstr2jstr(info.env, DL_FEEDBACK_EMAIL_SUBJECT.c_str());
        info.env->CallStaticVoidMethod(info.classID,info.methodID, address, subject);
        info.env->DeleteLocalRef(address);
        info.env->DeleteLocalRef(subject);
    }
}

double DLUtils::getCurrentTime(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000;
}

void DLUtils::openVideoUrl(const std::string& url){
    if (url.length() == 0) {
        return;
    }

    cocos2d::JniMethodInfo info;
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(info,"com/xinyu/game/dalipoker/Utils","openUrl","(Ljava/lang/String;)V");
    if(ret){
        jstring jstr = util_cstr2jstr(info.env, url.c_str());
        info.env->CallStaticVoidMethod(info.classID,info.methodID, jstr);
        info.env->DeleteLocalRef(jstr);
    }
}

#endif
