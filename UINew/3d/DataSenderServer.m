//
//  DataSenderServer.m
//  UltraVision
//
//  Created by Easy Proger on 04.10.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#import "DataSenderServer.h"

@implementation DataSenderServer

-(BOOL)send:(NSString*)path callBackTarget:(id)_target callBackAction:(SEL)_action callBackActionFail:(SEL)_actionFail succesParrams:(id)succesParrams cookies:(NSArray*)cookies {
    action = _action;
    actionFail = _actionFail;
    target = _target ;
    _succesParrams = succesParrams ;
    
    _path = path ;
    
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init] ;
    [request setCachePolicy:NSURLRequestReloadIgnoringLocalCacheData];
    [request setHTTPShouldHandleCookies:NO];
    if (cookies) {
        NSDictionary * headers = [NSHTTPCookie requestHeaderFieldsWithCookies:cookies];
        [request setAllHTTPHeaderFields:headers];
    }
    [request setTimeoutInterval:15];
    [request setHTTPMethod:@"GET"];
    [request setURL:[NSURL URLWithString:path]];
    
    _receivedData = [[NSMutableData alloc] init];
    _loader = [[NSURLConnection alloc] initWithRequest:request delegate:self];
    return FALSE;
    
}


- (void)connection:(NSURLConnection* )connection didReceiveResponse:(NSURLResponse* )response {
    [_receivedData setLength:0];
    
    _contentLength = [response expectedContentLength];
}

- (void)connection:(NSURLConnection* )connection didReceiveData:(NSData* )data {
    
    [_receivedData appendData:data];
}

- (void)connection:(NSURLConnection* )connection didFailWithError:(NSError* )error {
    
    NSLog(@"error:%@",error);
    [_receivedData setLength:0];
    if ([target respondsToSelector:actionFail]) {
        [target performSelector:actionFail withObject:_succesParrams];
    }
}





- (void)connectionDidFinishLoading:(NSURLConnection *)connection {

    NSString* answer = [[NSString alloc] initWithData:_receivedData encoding:NSASCIIStringEncoding];
    
    if ([target respondsToSelector:action]) {
        [target performSelector:action withObject:[NSArray arrayWithObjects:answer,_succesParrams, nil]];
    }
    
    [_receivedData setLength:0];
}





@end
