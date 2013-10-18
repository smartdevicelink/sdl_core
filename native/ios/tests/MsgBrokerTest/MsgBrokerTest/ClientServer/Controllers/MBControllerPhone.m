//
//  MBControllerPhone.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MBControllerPhone.h"
#import "MBConst.h"

#define kControllerName		@"Phone"

#define kMakeCallMethod		@"makeCall"

#define kPhoneBookChangedMethod		kControllerName @".onPhoneBookChanged"

#define kResultOK			@"OK"


@interface MBControllerPhone ()

- (NSString *)makeCall:(NSString *)phoneNumber;

@end

@implementation MBControllerPhone

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port {
	if (self = [super initWithHost:host port:port andName:kControllerName]) {
		
	}
	return self;
}

#pragma mark - Notifications

- (void)onPhoneBookChanged {
	NSMutableDictionary *notification = [self constructNotificationWithMethod:kPhoneBookChangedMethod andParameter:@""];
	[self sendJsonMessage:notification];
}

#pragma mark - Processing

- (void)processRequest:(NSDictionary *)request {
	NSString *response = nil;
	
	if ([[self methodNameOfMessage:request] isEqualToString:kMakeCallMethod]) {
		response = [self makeCall:[request objectForKey:kParametersKey]];
	}
	
	if (response) {
		[self sendJsonMessage:[self prepareResponse:response forMessage:request]];
	}
}

#pragma mark - Privates

- (NSString *)makeCall:(NSString *)phoneNumber {
	NSAssert(phoneNumber, @"makeCall: phoneNumber is nil");
	DDLogInfo(@"Do you really want to call Ghostbusters?");
	
	return kResultOK;
}

@end
