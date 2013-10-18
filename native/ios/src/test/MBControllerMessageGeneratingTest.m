//
//  MBControllerMessageGeneratingTest.m
//  avatar
//
//  Created by Eugene Nikolsky on 8/8/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <GHUnitIOS/GHUnit.h>
#import "MBController.h"
#import "MBTestConst.h"


// The test case tests the MBController's methods for generating
// JSON messages.
@interface MBControllerMessageGeneratingTest : GHTestCase

@property (nonatomic, strong) MBController *controller;

// Checks whether the message contains json-rpc and id keys.
- (void)checkRequiredFieldsInMessage:(NSDictionary *)message;
// Checks whether the extracted method name from message equals to the correct one.
- (void)compareMethodName:(NSString *)methodName withExtractedFromFullName:(NSString *)fullName;

@end


@implementation MBControllerMessageGeneratingTest

#pragma mark - Setup & teardown

// Creates an MBController object before the test case.
- (void)setUpClass {
	self.controller = [[MBController alloc] initWithHost:@"" port:0 andName:@""];
}

// Destroys the object after all tests.
- (void)tearDownClass {
	self.controller = nil;
}

#pragma mark - Internals

- (void)checkRequiredFieldsInMessage:(NSDictionary *)message {
	GHAssertTrue(message.count > 0, @"Message should not be empty");
	GHAssertNotNil([message objectForKey:kJsonRpcKey], @"Key jsonrpc not found");
	GHAssertNotNil([message objectForKey:kMessageIdKey], @"Key id not found");
}

- (void)compareMethodName:(NSString *)methodName withExtractedFromFullName:(NSString *)fullName {
	NSDictionary *message = [NSDictionary dictionaryWithObjectsAndKeys:
							 @"2.0", kJsonRpcKey,
							 [NSNumber numberWithInt:42], kMessageIdKey,
							 fullName, kMethodKey,
							 nil];
	NSString *name = [self.controller methodNameOfMessage:message];
	GHAssertEqualObjects(name, methodName, @"Method name is wrong");
}

#pragma mark - Tests

// Tests that the prepared message contains the right fields.
- (void)testPrepareMessage {
	NSMutableDictionary *message = [NSMutableDictionary dictionary];
	[self.controller prepareMessage:message];
	[self checkRequiredFieldsInMessage:message];
}

// Two successive messages should have consecutive ids.
- (void)testConsecutiveMessageIds {
	NSMutableDictionary *message1 = [NSMutableDictionary dictionary];
	[self.controller prepareMessage:message1];
	NSMutableDictionary *message2 = [NSMutableDictionary dictionary];
	[self.controller prepareMessage:message2];

	NSUInteger message1Id = [[message1 objectForKey:kMessageIdKey] unsignedIntegerValue];
	NSUInteger message2Id = [[message2 objectForKey:kMessageIdKey] unsignedIntegerValue];

	GHAssertTrue(message2Id == (message1Id + 1), @"Consecutive messages should have consecutive ids");
}

// Tests the convenience constructor of a JSON-RPC message with
// a method and a parameter.
- (void)testConstructMessageWithMethodAndParam {
	NSString *methodName = @"Test.construct";
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:@"world" forKey:@"hello"];

	NSMutableDictionary *message = [self.controller constructMessageWithMethod:methodName andParameter:parameters];
	[self checkRequiredFieldsInMessage:message];

	GHAssertEqualObjects([message objectForKey:kMethodKey], methodName, @"Method name doesn't match");
	GHAssertEqualObjects([message objectForKey:kParametersKey], parameters, @"Parameters don't match");
}

// Tests the convenience constructor of a JSON-RPC message with
// a method and no parameters.
- (void)testConstructMessageWithMethodAndNoParam {
	NSString *methodName = @"Test.construct";

	NSMutableDictionary *message = [self.controller constructMessageWithMethod:methodName andParameter:nil];
	[self checkRequiredFieldsInMessage:message];

	GHAssertEqualObjects([message objectForKey:kMethodKey], methodName, @"Method name doesn't match");
	GHAssertNil([message objectForKey:kParametersKey], @"Parameters should be nil");
}

// Tests the convenience constructor of a JSON-RPC notification with
// a method and a parameter. A notification should not have an id.
- (void)testConstructNotificationWithMethodAndParam {
	NSString *methodName = @"Test.notify";
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:@"world" forKey:@"hello"];

	NSMutableDictionary *message = [self.controller constructNotificationWithMethod:methodName andParameter:parameters];

	GHAssertTrue(message.count > 0, @"Message should not be empty");
	GHAssertNotNil([message objectForKey:kJsonRpcKey], @"Key jsonrpc not found");
	GHAssertNil([message objectForKey:kMessageIdKey], @"Notification should not have id");

	GHAssertEqualObjects([message objectForKey:kMethodKey], methodName, @"Method name doesn't match");
	GHAssertEqualObjects([message objectForKey:kParametersKey], parameters, @"Parameters don't match");
}

// Tests the convenience constructor of a JSON-RPC notification with
// a method and no parameters. A notification should not have an id.
- (void)testConstructNotificationWithMethodAndNoParam {
	NSString *methodName = @"Test.notify";

	NSMutableDictionary *message = [self.controller constructNotificationWithMethod:methodName andParameter:nil];

	GHAssertTrue(message.count > 0, @"Message should not be empty");
	GHAssertNotNil([message objectForKey:kJsonRpcKey], @"Key jsonrpc not found");
	GHAssertNil([message objectForKey:kMessageIdKey], @"Notification should not have id");

	GHAssertEqualObjects([message objectForKey:kMethodKey], methodName, @"Method name doesn't match");
	GHAssertNil([message objectForKey:kParametersKey], @"Parameters should be nil");
}

// Response for message should have its id and json-rpc version.
- (void)testResponseForMessage {
	NSDictionary *originalMessage = [NSDictionary dictionaryWithObjectsAndKeys:
									 @"2.0", kJsonRpcKey,
									 [NSNumber numberWithInt:42], kMessageIdKey,
									 @"unknown", kMethodKey,
									 nil];
	NSString *response = @"OK";

	NSMutableDictionary *message = [self.controller prepareResponse:response forMessage:originalMessage];
	[self checkRequiredFieldsInMessage:message];

	GHAssertEqualObjects([message objectForKey:kJsonRpcKey], [originalMessage objectForKey:kJsonRpcKey], @"JSON-RPC version should match");
	GHAssertEqualObjects([message objectForKey:kMessageIdKey], [originalMessage objectForKey:kMessageIdKey], @"Message id should match");
	GHAssertEqualObjects([message objectForKey:kResultKey], response, @"Response doesn't match");
	GHAssertNil([message objectForKey:kErrorKey], @"Error must not be here");
}

// Error response for message should have its id. Should have error dict, and not result dict.
- (void)testErrorResponseForMessage {
	NSDictionary *originalMessage = [NSDictionary dictionaryWithObjectsAndKeys:
									 @"2.0", kJsonRpcKey,
									 [NSNumber numberWithInt:42], kMessageIdKey,
									 @"unknown", kMethodKey,
									 nil];
	NSInteger errorCode = 1001;
	NSString *errorMessage = @"Not implemented";

	NSMutableDictionary *message = [self.controller prepareErrorResponseWithCode:errorCode message:errorMessage forMessage:originalMessage];
	[self checkRequiredFieldsInMessage:message];

	GHAssertEqualObjects([message objectForKey:kJsonRpcKey], [originalMessage objectForKey:kJsonRpcKey], @"JSON-RPC version should match");
	GHAssertEqualObjects([message objectForKey:kMessageIdKey], [originalMessage objectForKey:kMessageIdKey], @"Message id should match");
	GHAssertNil([message objectForKey:kResultKey], @"Result must not be here");

	NSDictionary *errorDict = [message objectForKey:kErrorKey];
	GHAssertNotNil(errorDict, @"Error object not found");
	GHAssertEqualObjects([errorDict objectForKey:kErrorCodeKey], [NSNumber numberWithInteger:errorCode], @"Error code is wrong");
	GHAssertEqualObjects([errorDict objectForKey:kErrorMessageKey], errorMessage, @"Error message is wrong");
}

// For message with "Test.object.unknown" method name should be "unknown"
- (void)testGetMethodNameFromHierarchicalFullName {
	[self compareMethodName:@"unknown" withExtractedFromFullName:@"Test.object.unknown"];
}

// For message with "Test.unknown" method name should be "unknown"
- (void)testGetMethodNameFromFullName {
	[self compareMethodName:@"unknown" withExtractedFromFullName:@"Test.unknown"];
}

// For message with "unknown" method name should be "unknown"
- (void)testGetMethodNameFromMethodName {
	[self compareMethodName:@"unknown" withExtractedFromFullName:@"unknown"];
}

// For message with empty method name should be empty
- (void)testGetMethodNameFromEmptyName {
	[self compareMethodName:@"" withExtractedFromFullName:@""];
}

// For message without method name should be nil
- (void)testGetMethodNameWithoutMethodName {
	NSDictionary *message = [NSDictionary dictionaryWithObjectsAndKeys:
							 @"2.0", kJsonRpcKey,
							 [NSNumber numberWithInt:42], kMessageIdKey,
							 nil];
	NSString *methodName = [self.controller methodNameOfMessage:message];
	GHAssertNil(methodName, @"Method name should be nil");
}

@end
