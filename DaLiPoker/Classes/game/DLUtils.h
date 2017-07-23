//
//  DLUtils.h
//  DaLiPoker
//
//  Created by lugan on 9/4/16.
//
//

#include <stdio.h>
#include <string>


static const std::string  DL_APP_VERSION = "0.7";

static const std::string  DL_FEEDBACK_EMAIL_ADDRESS = "xinyuclub2013@163.com";
static const std::string  DL_FEEDBACK_EMAIL_SUBJECT = "大李牌反馈";

class DLUtils
{
public:
    static void sendEmail();
    
    static double getCurrentTime();
    
    static void openVideoUrl(const std::string& url);
};
