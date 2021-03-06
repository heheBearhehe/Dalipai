//
//  DLUtils.mm
//  DaLiPoker
//
//  Created by lugan on 9/4/16.
//
//

#import "DLUtils.h"

#ifdef __APPLE__
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

void DLUtils::sendEmail(){
    
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
    
}

#endif
