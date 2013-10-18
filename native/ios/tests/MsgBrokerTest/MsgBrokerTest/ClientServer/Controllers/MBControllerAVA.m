//
//  MBControllerAVA.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MBControllerAVA.h"

#define kControllerName		@"AVA"

#define kPhonePrefix		@"Phone."
#define kMakeCallMethod		kPhonePrefix @"makeCall"

#define kPhoneBookChangedMethod	@"onPhoneBookChanged"


@implementation MBControllerAVA

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port {
	if (self = [super initWithHost:host port:port andName:kControllerName]) {
		
	}
	return self;
}

#pragma mark - Methods

- (void)makeCall:(NSString *)phoneNumber {
	DDLogVerbose(@"makeCall: %@", phoneNumber);
	
	NSMutableDictionary *message = [self constructMessageWithMethod:kMakeCallMethod andParameter:phoneNumber];
	[self sendJsonMessage:message];
}

#pragma mark - Processing data

- (void)processNotification:(NSDictionary *)notification {
	if ([[self methodNameOfMessage:notification] isEqualToString:kPhoneBookChangedMethod]) {
		DDLogVerbose(@" >> Phone book apparently changed!");
	}
}

@end
