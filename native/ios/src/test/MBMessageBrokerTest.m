//
//  MBMessageBrokerTest.m
//  avatar
//
//  Created by Eugene Nikolsky on 8/10/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <GHUnitIOS/GHUnit.h>
#import <OCMock/OCMock.h>
#import <JSONKit.h>
#import "MBMessageBroker.h"
#import "MBTestConst.h"


#define kParameterComponentName		@"componentName"

#define kMBPrefix					@"MB."
#define kRegisterControllerMethod	kMBPrefix @"registerComponent"
#define kUnregisterControllerMethod	kMBPrefix @"unregisterComponent"


@interface MBMessageBrokerTest : GHAsyncTestCase

@property (nonatomic, assign) int sendDataCallCount;

@end


@implementation MBMessageBrokerTest

@synthesize sendDataCallCount = _sendDataCallCount;

// Tests the singleton
- (void)testSingleton {
	MBMessageBroker *messageBroker = [MBMessageBroker sharedMessageBroker];
	GHAssertNotNil(messageBroker, @"message broker is not available");
}

// Tests responses for register and unregister messages sent to MessageBroker
- (void)testMessageBroker {
	[self prepare];
	
	self.sendDataCallCount = 0;

	// mock object that emulates sender
	id senderMock = [OCMockObject mockForProtocol:@protocol(MBMessageBrokerSender)];
	[[senderMock stub] sendData:[OCMArg checkWithBlock:^BOOL(id value) {
		++self.sendDataCallCount;
		
		switch (self.sendDataCallCount) {
			case 1: {
				// expect response for register message
				NSData *data = value;
				GHAssertNotNil(data, @"value is not NSData");
				
				NSError *error = nil;
				NSDictionary *returnMessage = [data objectFromJSONDataWithParseOptions:JKParseOptionNone error:&error];
				GHAssertNotNil(returnMessage, @"return message is nil");
				GHAssertNil(error, @"json parsing failed");
				
				NSDictionary *correctMessage = [NSDictionary dictionaryWithObjectsAndKeys:
												[NSNumber numberWithInt:1], kMessageIdKey,
												@"2.0", kJsonRpcKey,
												[NSNumber numberWithInt:1000], kResultKey,
												nil];
				
				NSComparisonResult (^compareStrings)(id, id) = ^(id obj1, id obj2) {
					NSString *string1 = obj1;
					NSString *string2 = obj2;
					return [string1 compare:string2];
				};
				// we have to compare sorted keys of the dictionaries, because
				// result id is different on every test re-run
				return [[[returnMessage allKeys] sortedArrayUsingComparator:compareStrings] isEqualToArray:
						[[correctMessage allKeys] sortedArrayUsingComparator:compareStrings]];
			}
				
			case 2: {
				// expect response for unregister message
				// we have to use a block here to compare the dictionaries, and not plain data,
				// because their formats may differ
				NSData *data = value;
				GHAssertNotNil(data, @"value is not NSData");
				
				NSError *error = nil;
				NSDictionary *returnMessage = [data objectFromJSONDataWithParseOptions:JKParseOptionNone error:&error];
				GHAssertNotNil(returnMessage, @"return message is nil");
				GHAssertNil(error, @"json parsing failed");
				
				NSDictionary *correctMessage = [NSDictionary dictionaryWithObjectsAndKeys:
												[NSNumber numberWithInt:2], kMessageIdKey,
												@"2.0", kJsonRpcKey,
												@"OK", kResultKey,
												nil];
				return [returnMessage isEqualToDictionary:correctMessage];
			}
		}
		
		return YES;
	}] withFd:[NSNumber numberWithInt:1]];
	
	MBMessageBroker *messageBroker = [MBMessageBroker sharedMessageBroker];
	[messageBroker startWithSender:senderMock inDispatchQueue:dispatch_get_global_queue(0, 0)];

	double delayInSeconds = .1;
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
	dispatch_after(popTime, dispatch_get_current_queue(), ^(void){
		[messageBroker stop];

		[self notify:kGHUnitWaitStatusSuccess forSelector:_cmd];
	});

	// register component message
	NSDictionary *message = [NSDictionary dictionaryWithObjectsAndKeys:
							 [NSNumber numberWithInt:1], kMessageIdKey,
							 @"2.0", kJsonRpcKey,
							 kRegisterControllerMethod, kMethodKey,
							 [NSDictionary dictionaryWithObject:@"Test" forKey:kParameterComponentName], kParametersKey,
							 nil];

	[messageBroker receiveMessage:[message JSONString] fromSocketWithFd:[NSNumber numberWithInt:1]];

	// unregister component message
	message = [NSDictionary dictionaryWithObjectsAndKeys:
			   [NSNumber numberWithInt:2], kMessageIdKey,
			   @"2.0", kJsonRpcKey,
			   kUnregisterControllerMethod, kMethodKey,
			   [NSDictionary dictionaryWithObject:@"Test" forKey:kParameterComponentName], kParametersKey,
			   nil];
	[messageBroker receiveMessage:[message JSONString] fromSocketWithFd:[NSNumber numberWithInt:1]];

	[self waitForStatus:kGHUnitWaitStatusSuccess timeout:1.];
	
	GHAssertEquals(self.sendDataCallCount, 2, @"sendData wasn't called twice");
}

@end
