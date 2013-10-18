//
//  MBControllerRate.m
//  avatar
//
//  Created by Eugene Nikolsky on 2013-09-04.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "MBControllerRate.h"

#import "MBConst.h"

#define kControllerName				@"Rate"

#define kMethodRateApp              @"rateApp"


@interface MBControllerRate ()

- (NSString *)processRateAppRequest:(NSDictionary *)request
                              error:(NSDictionary **)error;

@end

@implementation MBControllerRate

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port {
    return [super initWithHost:host port:port andName:kControllerName];
}

#pragma mark - Requests

- (void)processRequest:(NSDictionary *)request {
	NSString *method = [self methodNameOfMessage:request];
	id response = nil;
	NSDictionary *error = nil;
    
    if ([method isEqualToString:kMethodRateApp]) {
        response = [self processRateAppRequest:request error:&error];
    }
	
    if (error) {
        [self sendJsonMessage:error];
    } else if (response) {
        [self sendJsonMessage:[self prepareResponse:response forMessage:request]];
    } else {
        DDLogWarn(@"%@: no response for request %@", kControllerName, request);
        [super processRequest:request];
    }
}

#pragma mark - Internal Methods

- (NSString *)processRateAppRequest:(NSDictionary *)request
                              error:(NSDictionary **)error {
    if ([self.delegate respondsToSelector:@selector(rateAppFromControllerRate:)]) {
        __block MBControllerRateResult result;
        dispatch_sync(dispatch_get_main_queue(), ^{
            result = [self.delegate rateAppFromControllerRate:self];
        });
        
        switch (result) {
            case MBControllerRateResultOK:
                return @"OK";
                break;
                
            case MBControllerRateResultNoNetwork:
                *error = [[self prepareErrorResponseWithCode:kMBRateNoNetwork
                                                     message:@"No internets"
                                                  forMessage:request] copy];
                break;
                
            case MBControllerRateResultStoreNotAvailable:
                *error = [[self prepareErrorResponseWithCode:kMBRateStoreNotAvailable
                                                     message:@"No connection to AppStore"
                                                  forMessage:request] copy];
                break;
                
            case MBControllerRateResultUnknownAppID:
                *error = [[self prepareErrorResponseWithCode:kMBRequestNotHandled
                                                     message:@"AppID is not specified"
                                                  forMessage:request] copy];
                break;
        }
    } else {
        *error = [[self prepareErrorResponseWithCode:kMBRequestNotHandled
                                             message:@"No delegate for this request"
                                          forMessage:request] copy];
    }
    
    return nil;
}

@end
