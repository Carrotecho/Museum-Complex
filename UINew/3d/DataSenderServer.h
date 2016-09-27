//
//  DataSenderServer.h
//  UltraVision
//
//  Created by Easy Proger on 04.10.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DataSenderServer : NSObject<NSURLConnectionDelegate> {
    id target;
    SEL action;
    SEL actionFail;
    
    long long _contentLength;
    NSURLRequest* _urlRequest;
    NSURLConnection* _loader;
    id _succesParrams;
    NSString * _path;
    BOOL fileExists;
}
@property(strong,nonatomic) NSMutableData* receivedData;


-(BOOL)send:(NSString*)path callBackTarget:(id)_target callBackAction:(SEL)_action callBackActionFail:(SEL)_actionFail succesParrams:(id)succesParrams cookies:(NSArray*)cookies;

@end
