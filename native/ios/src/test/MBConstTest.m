//
//  MBConstTest.m
//  avatar
//
//  Created by Eugene Nikolsky on 8/7/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <GHUnitIOS/GHUnit.h>
#import <JSONKit.h>
#import "MBTestConst.h"


// The test case tests MBConst's method to split&parse JSON messages from a string
@interface MBConstTest : GHTestCase

@end


@implementation MBConstTest

// Parses an empty message. The parses should not return anything.
- (void)testParseEmptyMessage {
	NSData *originalMessageData = [NSData data];

	__block int blockCallNumber = 0;
	[MBConst parseJSONMessagesFromData:originalMessageData
				   withProcessingBlock:^(id message) {
#pragma unused (message)
					   GHFail(@"Should not be here");
					   ++blockCallNumber;
				   }];

	GHAssertEquals(blockCallNumber, 0, @"There should be no messages");
}

// Parses half a message. The parser should not return anything.
- (void)testParseHalfAMessage {
	NSString *originalMessage = @"{\"jsonrpc\": \"2.0\", \"id\": 1";
	NSData *originalMessageData = [originalMessage dataUsingEncoding:NSUTF8StringEncoding];

	__block int blockCallNumber = 0;
	[MBConst parseJSONMessagesFromData:originalMessageData
				   withProcessingBlock:^(id message) {
#pragma unused (message)
					   GHFail(@"Should not be here");
					   ++blockCallNumber;
				   }];

	GHAssertEquals(blockCallNumber, 0, @"There should be no messages");
}

// Parses one message. The parser should return one message.
- (void)testParseOneMessage {
	NSDictionary *originalMessage = [NSDictionary dictionaryWithObjectsAndKeys:
									 @"2.0", kJsonRpcKey,
									 [NSNumber numberWithInt:1], kMessageIdKey,
									 @"unknown", kMethodKey,
									 nil];
	NSData *originalMessageData = [originalMessage JSONData];

	__block int blockCallNumber = 0;
	[MBConst parseJSONMessagesFromData:originalMessageData
				   withProcessingBlock:^(id message) {
					   GHAssertEqualObjects(message, originalMessage, @"Parsed message doesn't match");
					   ++blockCallNumber;
				   }];

	GHAssertEquals(blockCallNumber, 1, @"There should be one message");
}

// Parses one message and a half. The parser should return one message.
- (void)testParseOneMessageAndAHalf {
	NSDictionary *originalMessage = [NSDictionary dictionaryWithObjectsAndKeys:
									 @"2.0", kJsonRpcKey,
									 [NSNumber numberWithInt:1], kMessageIdKey,
									 @"unknown", kMethodKey,
									 nil];
	NSMutableData *originalMessageData = [NSMutableData dataWithData:[originalMessage JSONData]];
	[originalMessageData appendData:[@"{\"jsonrpc\": \"2.0\", \"id\": 3, " dataUsingEncoding:NSUTF8StringEncoding]];

	__block int blockCallNumber = 0;
	[MBConst parseJSONMessagesFromData:originalMessageData
				   withProcessingBlock:^(id message) {
					   GHAssertEqualObjects(message, originalMessage, @"Parsed message doesn't match");
					   ++blockCallNumber;
				   }];

	GHAssertEquals(blockCallNumber, 1, @"There should be one message");
}

// Parses two JSON messages in a row. The parser should return two messages.
- (void)testParseTwoMessages {
	NSArray *originalMessages = [NSArray arrayWithObjects:
								 [NSDictionary dictionaryWithObjectsAndKeys:
								  @"2.0", kJsonRpcKey,
								  [NSNumber numberWithInt:1], kMessageIdKey,
								  @"unknown", kMethodKey,
								  nil],
								 [NSDictionary dictionaryWithObjectsAndKeys:
								  @"2.0", kJsonRpcKey,
								  [NSNumber numberWithInt:8], kMessageIdKey,
								  @"unknown", kMethodKey,
								  nil],
								 nil];
	NSMutableData *originalMessageData = [NSMutableData dataWithData:[[originalMessages objectAtIndex:0] JSONData]];
	[originalMessageData appendData:[[originalMessages objectAtIndex:1] JSONData]];

	__block int blockCallNumber = 0;
	[MBConst parseJSONMessagesFromData:[originalMessageData copy]
				   withProcessingBlock:^(id message) {
					   GHAssertEqualObjects(message, [originalMessages objectAtIndex:blockCallNumber], @"Parsed message doesn't match");
					   ++blockCallNumber;
				   }];

	GHAssertEquals(blockCallNumber, 2, @"There should be two messages");
}

// Parses two JSON messages in a row with an LF between them. The parser should return two messages.
// It's a synthetic test, required because MessageBroker and WebSocket clients
// insert an LF at the end of a string.
- (void)testParseTwoMessagesWithLFInBetween {
	NSArray *originalMessages = [NSArray arrayWithObjects:
								 [NSDictionary dictionaryWithObjectsAndKeys:
								  @"2.0", kJsonRpcKey,
								  [NSNumber numberWithInt:1], kMessageIdKey,
								  @"unknown", kMethodKey,
								  nil],
								 [NSDictionary dictionaryWithObjectsAndKeys:
								  @"2.0", kJsonRpcKey,
								  [NSNumber numberWithInt:8], kMessageIdKey,
								  @"unknown", kMethodKey,
								  nil],
								 nil];
	NSMutableData *originalMessageData = [NSMutableData dataWithData:[[originalMessages objectAtIndex:0] JSONData]];
	[originalMessageData appendData:[@"\x0a" dataUsingEncoding:NSISOLatin1StringEncoding]];
	[originalMessageData appendData:[[originalMessages objectAtIndex:1] JSONData]];

	__block int blockCallNumber = 0;
	[MBConst parseJSONMessagesFromData:[originalMessageData copy]
				   withProcessingBlock:^(id message) {
					   GHAssertEqualObjects(message, [originalMessages objectAtIndex:blockCallNumber], @"Parsed message doesn't match");
					   ++blockCallNumber;
				   }];

	GHAssertEquals(blockCallNumber, 2, @"There should be two messages");
}

@end
