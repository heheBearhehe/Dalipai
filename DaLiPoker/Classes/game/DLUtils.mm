//
//  DLUtils.mm
//  DaLiPoker
//
//  Created by lugan on 9/4/16.
//
//

#import "DLUtils.h"
#import <UIKit/UIKit.h>

void DLUtils::sendEmail(){
    NSMutableString *mailUrl = [[NSMutableString alloc] init];
    [mailUrl appendFormat:@"mailto:%s", DL_FEEDBACK_EMAIL_ADDRESS.c_str()];
    [mailUrl appendFormat:@"?subject=%@", [[NSString alloc] initWithUTF8String:DL_FEEDBACK_EMAIL_SUBJECT.c_str()]];
    
    NSString * encodingString = [mailUrl stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:encodingString]];
}