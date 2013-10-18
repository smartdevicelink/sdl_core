//
//  MBControllerTest.m
//  avatar
//
//  Created by Eugene Nikolsky on 4/23/13.
//  Copyright (c) 2013 Luxoft. All rights reserved.
//

#import <GHUnitIOS/GHUnit.h>
#import <OCMock.h>
#import "MBController_Private.h"
#import "MBConst.h"


// The test case tests the MBController interface.
@interface MBControllerTest : GHTestCase

@property (nonatomic, strong) MBController *controller;

// Checks that the error message is populated correctly.
- (void)checkErrorDict:(NSDictionary *)dict;

@end


@implementation MBControllerTest

- (void)setUpClass {
	self.controller = [[MBController alloc]
					   initWithHost:@"" port:0 andName:@""];
}

- (void)tearDownClass {
	self.controller = nil;
}

#pragma mark - Tests

// Tests that the generated message id correctly wraps back to the start value
// after reaching the max id.
- (void)testMessageIdLoopingOverMaxId {
	NSNumber *firstMsgId = self.controller.nextMessageId;
	GHAssertNotNil(firstMsgId, @"firstMsgId should not be nil");
	
	NSNumber *msgId = nil;
	for (int i = 0; i < kMessageIdMaxNumber; ++i) {
		msgId = self.controller.nextMessageId;
		GHAssertNotNil(msgId, @"msgId should not be nil");
	}
	
	GHAssertEqualObjects(firstMsgId, msgId,
						 @"msgId should wrap back to the beginning");
}

// Tests that didConnect registers the controller with different start id
// each time.
- (void)testConnectWithDifferentId {
	id mock = [OCMockObject partialMockForObject:self.controller];
	
	static const int MAX_CALL_NUMBER = 3;
	NSMutableArray *startIds = [NSMutableArray
								arrayWithCapacity:MAX_CALL_NUMBER];
	
	BOOL (^startIdCheckBlock)(id) = ^BOOL(id obj) {
		NSNumber *startId = (NSNumber *)obj;
		GHAssertNotNil(startId, @"startId should be set");
		
		// check that startId doesn't appear in the array yet
		NSUInteger index = [startIds indexOfObject:startId];
		if (index != NSNotFound) {
			return NO;
		}
		
		[startIds addObject:startId];
		
		return YES;
	};
	
	// register N check blocks
	for (int i = 0; i < MAX_CALL_NUMBER; ++i) {
		[[mock expect] registerControllerWithStartId:
		 [OCMArg checkWithBlock:startIdCheckBlock]];
	}
	
	// simulate N connects
	for (int i = 0; i < MAX_CALL_NUMBER; ++i) {
		[self.controller didConnect];
	}
	
	[mock verify];
}

// Tests that the MBController's -processRequest: method sends back a
// Not Implemented error.
- (void)testProcessRequestShouldSendError {
	id mock = [OCMockObject partialMockForObject:self.controller];
	[[mock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		[self checkErrorDict:msg];
		
		return YES;
	}]];
	
	NSMutableDictionary *msg = [NSMutableDictionary dictionary];
	[self.controller prepareMessage:msg];
	[self.controller processRequest:msg];
	
	[mock verify];
}

// Tests that -registerControllerWithStartId: method sends the required message.
- (void)testRegisterControllerMessage {
	static NSString *kControllerName = @"TestController";
	self.controller = [[MBController alloc]
					   initWithHost:nil port:0 andName:kControllerName];
	
	static const int startId = 42;
	id mock = [OCMockObject partialMockForObject:self.controller];
	
	[[mock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		GHAssertEqualObjects(msg[kMessageIdKey], @(startId), @"Wrong message id");
		
		GHAssertEqualObjects(msg[kMethodKey], kRegisterControllerMethod,
							 @"Wrong method name");
		NSDictionary *correctParams = @{kParameterComponentName: kControllerName};
		GHAssertEqualObjects(msg[kParametersKey], correctParams,
							 @"Wrong parameters");
		
		return YES;
	}]];
	[self.controller registerControllerWithStartId:@(startId)];
	
	[mock verify];
}

// Tests that -unregisterController method sends the required message.
- (void)testUnregisterControllerMessage {
	static NSString *kControllerName = @"TestController";
	self.controller = [[MBController alloc]
					   initWithHost:nil port:0 andName:kControllerName];
	
	id mock = [OCMockObject partialMockForObject:self.controller];
	
	[[mock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		GHAssertEqualObjects(msg[kMethodKey], kUnregisterControllerMethod,
							 @"Wrong method name");
		NSDictionary *correctParams = @{kParameterComponentName: kControllerName};
		GHAssertEqualObjects(msg[kParametersKey], correctParams,
							 @"Wrong parameters");
		
		return YES;
	}]];
	[self.controller unregisterController];
	
	[mock verify];
}

// Tests that -subscribeToProperty: method sends the required message.
- (void)testSubscribeToPropertyMessage {
	static NSString *propertyName = @"test.onChanged";
	
	id mock = [OCMockObject partialMockForObject:self.controller];
	
	[[mock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		GHAssertEqualObjects(msg[kMethodKey], kSubscribeToPropertyMethod,
							 @"Wrong method name");
		NSDictionary *correctParams = @{kParameterPropertyName: propertyName};
		GHAssertEqualObjects(msg[kParametersKey], correctParams,
							 @"Wrong parameters");
		
		return YES;
	}]];
	[self.controller subscribeToProperty:propertyName];
	
	[mock verify];
}

// Tests that -unsubscribeFromProperty: method sends the required message.
- (void)testUnsubscribeFromPropertyMessage {
	static NSString *propertyName = @"test.onChanged";
	
	id mock = [OCMockObject partialMockForObject:self.controller];
	
	[[mock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		GHAssertEqualObjects(msg[kMethodKey], kUnsubscribeFromPropertyMethod,
							 @"Wrong method name");
		NSDictionary *correctParams = @{kParameterPropertyName: propertyName};
		GHAssertEqualObjects(msg[kParametersKey], correctParams,
							 @"Wrong parameters");
		
		return YES;
	}]];
	[self.controller unsubscribeFromProperty:propertyName];
	
	[mock verify];
}

// Tests that -messageIsNotification: method returns YES on a notification.
// A message is a notification if it doesn't have a message id.
- (void)testMessageIsNotificationWithoutId {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion,
					   kMethodKey: @"notification"};
	GHAssertTrue([self.controller messageIsNotification:msg],
				 @"It is a notification");
}

// Tests that -messageIsNotification: method returns NO on a request.
- (void)testMessageIsNotificationWithId {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
					   kMethodKey: @"notification"};
	GHAssertFalse([self.controller messageIsNotification:msg],
				 @"It is not a notification");
}

// Tests that -messageIsResponse: method returns YES on a response with the
// result key.
- (void)testMessageIsResponseWithResultKey {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
					   kResultKey: @"OK"};
	GHAssertTrue([self.controller messageIsResponse:msg],
				 @"It is a response with result key");
}

// Tests that -messageIsResponse: method returns YES on a response with the
// error key.
- (void)testMessageIsResponseWithErrorKey {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
					   kErrorKey: @{kErrorCodeKey: @-1}};
	GHAssertTrue([self.controller messageIsResponse:msg],
				 @"It is a response with error key");
}

// -messageIsResponse: should return NO when a message has neither result key
// nor error key.
- (void)testMessageIsResponseMinimal {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42};
	GHAssertFalse([self.controller messageIsResponse:msg],
				  @"Message is not a response w/o result or error key");
}

// -messageIsResponse: should return NO when a message has both result key and
// error key.
// JSON-RPC 2.0 specifies: "Either the result member or error member MUST be
// included, but both members MUST NOT be included."
- (void)testMessageIsResponseWithResultAndErrorKeys {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
					   kErrorKey: @{kErrorCodeKey: @-1}, kResultKey: @"OK"};
	GHAssertFalse([self.controller messageIsResponse:msg],
				  @"Response is invalid with both result and error keys");
}

#pragma mark - -checkMessage: tests

// Tests that -checkMessage; method passes a message with JSON-RPC version only.
- (void)testCheckMessageMinimal {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNil(result, @"Minimal message should pass");
}

// Tests that -checkMessage; method passes a message with 4 standard keys.
- (void)testCheckMessageFull {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
					   kMethodKey: @"method", kParametersKey: @{@"param": @1}};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNil(result, @"Message with 4 standard keys should pass");
}

// -checkMessage: should pass a response with result key.
- (void)testCheckMessageResponseWithResult {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
					   kResultKey: @"OK"};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNil(result, @"Response with result key should pass");
}

// -checkMessage: should pass a response with error key.
- (void)testCheckMessageResponseWithError {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
					   kErrorKey: @{kErrorCodeKey: @-1}};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNil(result, @"Response with error key should pass");
}

// Tests that -checkMessage: passes a message with result as number.
- (void)testCheckMessageResultIsNumber {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kResultKey: @42};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNil(result, @"Message where result is a number should pass");
}

// Tests that -checkMessage: passes a message with result as array.
- (void)testCheckMessageResultIsArray {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kResultKey: @[@"OK"]};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNil(result, @"Message where result is an array should pass");
}

// Tests that -checkMessage: passes a message with result as dictionary.
- (void)testCheckMessageResultIsDictionary {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kResultKey: @{}};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNil(result, @"Message where result is a dictionary should pass");
}

// Tests that -checkMessage: passes a message with result as null.
- (void)testCheckMessageResultIsNull {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kResultKey: [NSNull null]};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNil(result, @"Message where result is a null should pass");
}

// Tests that -checkMessage: method fails an empty message.
- (void)testCheckMessageEmpty {
	NSDictionary *result = [self.controller checkMessage:@{}];
	GHAssertNotNil(result, @"Empty message should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with unexpected fields.
- (void)testCheckMessageWithExtraFields {
	NSMutableDictionary *msg = [NSMutableDictionary dictionary];
	[self.controller prepareMessage:msg];
	msg[@"extraField"] = @"extraValue";
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message with unexpected fields should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with unsupported JSON-RPC version.
- (void)testCheckMessageWithUnsupportedJsonRpcVersion {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: @"3.0"};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message with unsupported JSON-RPC version should not pass");
	
	[self checkErrorDict:result];
}

// -checkMessage: should fail a response with both error and result keys.
- (void)testCheckMessageInvalidResponse {
	NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
					   kErrorKey: @{kErrorCodeKey: @-1}, kResultKey: @"OK"};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Response is invalid with both result and error keys");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with id as array.
- (void)testCheckMessageIdIsArray {
	NSDictionary *msg = @{kMessageIdKey: @[@3000], kJsonRpcKey: kJsonRpcVersion};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where id is an array should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with id a dictionary.
- (void)testCheckMessageIdIsDictionary {
	NSDictionary *msg = @{kMessageIdKey: @{}, kJsonRpcKey: kJsonRpcVersion};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where id is a dictionary should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with id as null.
- (void)testCheckMessageIdIsNull {
	NSDictionary *msg = @{kMessageIdKey: [NSNull null], kJsonRpcKey: kJsonRpcVersion};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where id is a null should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with method as array.
- (void)testCheckMessageMethodIsArray {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kMethodKey: @[@"method"]};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where method is an array should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with method as dictionary.
- (void)testCheckMessageMethodIsDictionary {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kMethodKey: @{}};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where method is a dictionary should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with method as null.
- (void)testCheckMessageMethodIsNull {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kMethodKey: [NSNull null]};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where method is a null should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with parameters as array.
- (void)testCheckMessageParametersIsArray {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kMethodKey: @"method", kParametersKey: @[@"param"]};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where parameters is an array should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with parameters as string.
- (void)testCheckMessageParametersIsString {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kMethodKey: @"method", kParametersKey: @"param"};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where parameters is a string should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with parameters as null.
- (void)testCheckMessageParametersIsNull {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kMethodKey: @"method", kParametersKey: [NSNull null]};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where parameters is a null should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with error as array.
- (void)testCheckMessageErrorIsArray {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kErrorKey: @[@"Bad"]};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where error is an array should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with error as string.
- (void)testCheckMessageErrorIsString {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kErrorKey: @"Bad"};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where error is a string should not pass");
	
	[self checkErrorDict:result];
}

// Tests that -checkMessage: fails a message with error as null.
- (void)testCheckMessageErrorIsNull {
	NSDictionary *msg = @{kMessageIdKey: @3000, kJsonRpcKey: kJsonRpcVersion,
					   kErrorKey: [NSNull null]};
	NSDictionary *result = [self.controller checkMessage:msg];
	GHAssertNotNil(result, @"Message where error is a null should not pass");
	
	[self checkErrorDict:result];
}

#pragma mark - Private methods

- (void)checkErrorDict:(NSDictionary *)dict {
	NSDictionary *errorDict = [dict objectForKey:kErrorKey];
	GHAssertNotNil(errorDict, @"Error dictionary not found");
	
	GHAssertEquals(errorDict.count, 2u, @"Error dictionary should contain 2 pairs");
	
	NSNumber *errorCode = [errorDict objectForKey:kErrorCodeKey];
	GHAssertEqualObjects(errorCode, @(kMBInvalidRequest),
						 @"Error code should be InvalidRequest");
	NSString *errorMessage = [errorDict objectForKey:kErrorMessageKey];
	GHAssertNotNil(errorMessage, @"Error message should not be nil");
	GHAssertGreaterThan(errorMessage.length, 0u,
						@"Error message should not be empty");
}

@end
