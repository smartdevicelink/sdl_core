//
//  MBWebSocketHandshakeTest.m
//  avatar
//
//  Created by Eugene Nikolsky on 8/13/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <GHUnitIOS/GHUnit.h>
#import "MBServer_Private.h"


@interface MBWebSocketHandshakeTest : GHTestCase

@end


@implementation MBWebSocketHandshakeTest

#pragma mark - Privates

// Builds a dictionary with HTTP fields from an array of header fields.
// Key is field name, value is field value.
// Stop traversing the array when first non-header is found.
- (NSDictionary *)HTTPRequestDictFromArray:(NSArray *)request {
	NSMutableDictionary *requestDict = [NSMutableDictionary dictionary];
	for (NSString *header in request) {
		NSArray *headerInfo = [header componentsSeparatedByString:@": "];
		// there should be fieldName and fieldValue
		if (headerInfo.count != 2) {
			break;
		}

		[requestDict setObject:[headerInfo objectAtIndex:1] forKey:[headerInfo objectAtIndex:0]];
	}

	return [requestDict copy];
}

// Makes a fuzzy comparison of HTTP headers of two requests/responses.
- (void)compareHeaders:(NSString *)requestHeaders withCorrectHeaders:(NSString *)correctHeaders {
	// split the requests to arrays
	NSArray *requestArray = [requestHeaders componentsSeparatedByString:@"\r\n"];
	GHAssertTrue(requestArray.count > 1, @"Not enough request headers");
	NSArray *correctArray = [correctHeaders componentsSeparatedByString:@"\r\n"];
	GHAssertTrue(correctArray.count > 1, @"Not enough correct headers");

	// first compare HTTP status code
	NSString *requestStatusLine = [requestArray objectAtIndex:0];
	NSString *correctStatusLine = [correctArray objectAtIndex:0];

	NSError *error = nil;
	NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"^HTTP/\\d\\.\\d (\\d{3}) \\w+"
																		   options:0
																			 error:&error];
	GHAssertNil(error, @"regex is wrong");

	NSRange requestStatusLineRange = NSMakeRange(0, requestStatusLine.length);
	NSRange correctStatusLineRange = NSMakeRange(0, correctStatusLine.length);
	GHAssertTrue([regex numberOfMatchesInString:requestStatusLine options:0 range:requestStatusLineRange] == 1, @"Request status code not found");
	GHAssertTrue([regex numberOfMatchesInString:correctStatusLine options:0 range:correctStatusLineRange] == 1, @"Correct status code not found");
	NSString *requestStatusCode = [requestStatusLine substringWithRange:[[regex firstMatchInString:requestStatusLine options:0 range:requestStatusLineRange] rangeAtIndex:1]];
	NSString *correctStatusCode = [correctStatusLine substringWithRange:[[regex firstMatchInString:correctStatusLine options:0 range:correctStatusLineRange] rangeAtIndex:1]];
	GHAssertEqualObjects(requestStatusCode, correctStatusCode, @"HTTP status code doesn't match");

	// because the headers may be sent in different order, we build two dicts
	// from the arrays
	NSDictionary *requestDict = [self HTTPRequestDictFromArray:[requestArray subarrayWithRange:NSMakeRange(1, requestArray.count - 1)]];
	GHAssertTrue(requestDict.count > 0, @"No request headers");
	NSDictionary *correctDict = [self HTTPRequestDictFromArray:[correctArray subarrayWithRange:NSMakeRange(1, correctArray.count - 1)]];
	GHAssertTrue(correctDict.count > 0, @"No correct headers");

	// now we check that all of the correct headers are in request headers
	[correctDict enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
#pragma unused (stop)
		NSString *correctFieldName = key;
		NSString *requestFieldValue = [requestDict objectForKey:correctFieldName];
		GHAssertNotNil(requestFieldValue, @"No field found in request dict: %@", correctFieldName);

		NSString *correctFieldValue = obj;
		GHAssertEqualObjects(requestFieldValue, correctFieldValue, @"Field values don't match");
	}];

	if (requestDict.count != correctDict.count) {
		GHTestLog(@"Number of request headers (%d) != number of correct headers (%d)", requestDict.count, correctDict.count);
	}

	// at the end we check the data after the headers
	GHAssertEqualObjects([requestArray lastObject], [correctArray lastObject], @"Data doesn't match");
}

// Compares correct WebSocket protocol handshake with the composed one.
- (void)compareCorrectHandshake:(NSString *)correctHandshake withResponseForRequest:(NSString *)request withKey3:(NSString *)key3 {
	MBServer *server = [[MBServer alloc] initWithHost:nil port:0 andMessageBroker:nil];
	GHAssertNotNil(server, @"server is not created");

	NSData *requestData = [request dataUsingEncoding:NSUTF8StringEncoding];
	GHAssertTrue(requestData.length > 0, @"request is empty");

	NSData *key3Data = [key3 dataUsingEncoding:NSUTF8StringEncoding];

	NSData *handshakeData = [server composeWebSocketHandshakeResponseFromRequest:requestData andKey3:key3Data];

	if (correctHandshake != nil) {
		GHAssertTrue(handshakeData.length > 0, @"no handshake is generated");
		NSString *handshake = [[NSString alloc] initWithData:handshakeData encoding:NSUTF8StringEncoding];
		GHAssertTrue(handshake.length > 0, @"can't convert handshake data to string");

		[self compareHeaders:handshake withCorrectHeaders:correctHandshake];
	} else {
		GHAssertNil(handshakeData, @"Data is not nil, but should be");
	}
}

#pragma mark - Hybi-00 tests

// Tests the reference handshake with hybi-00 WebSocket protocol.
- (void)testHybi00Handshake {
	[self compareCorrectHandshake:@"HTTP/1.1 101 WebSocket Protocol Handshake\r\n"
									 "Upgrade: WebSocket\r\n"
									 "Connection: Upgrade\r\n"
									 "Sec-WebSocket-Origin: http://example.com\r\n"
									 "Sec-WebSocket-Location: ws://example.com/\r\n"
									 "Sec-WebSocket-Protocol: sample\r\n"
									 "\r\n"
									 "8jKS'y:G*Co,Wxa-"
		   withResponseForRequest:@"GET / HTTP/1.1\r\n"
									// modified example request from hybi-00 spec
									"Host: example.com\r\n"
									"Connection: Upgrade\r\n"
									"Sec-WebSocket-Key2: 12998 5 Y3 1  .P00\r\n"
									"Sec-WebSocket-Protocol: sample\r\n"
									"Upgrade: WebSocket\r\n"
									"Sec-WebSocket-Key1: 4 @1  46546xW%0l 1 5\r\n"
									"Origin: http://example.com\r\n"
									"\r\n"
						 withKey3:@"^n:ds[4U"];
}

// Request w/o key2 should get no response
- (void)testHybi00HandshakeWithoutKey2 {
	[self compareCorrectHandshake:nil
		   withResponseForRequest:@"GET / HTTP/1.1\r\n"
									 // modified example request from hybi-00 spec
									 // w/o Sec-WebSocket-Key2
									 "Host: example.com\r\n"
									 "Connection: Upgrade\r\n"
									 "Sec-WebSocket-Protocol: sample\r\n"
									 "Upgrade: WebSocket\r\n"
									 "Sec-WebSocket-Key1: 4 @1  46546xW%0l 1 5\r\n"
									 "Origin: http://example.com\r\n"
									 "\r\n"
						 withKey3:@"^n:ds[4U"];
}

// Request with incorrect key1 should get no response
- (void)testHybi00HandshakeWithIncorrectKey1 {
	[self compareCorrectHandshake:nil
		   withResponseForRequest:@"GET / HTTP/1.1\r\n"
									 // modified example request from hybi-00 spec
									 "Host: example.com\r\n"
									 "Connection: Upgrade\r\n"
									 "Sec-WebSocket-Protocol: sample\r\n"
									 "Sec-WebSocket-Key2: 12998 5 Y3 1  .P00\r\n"
									 "Upgrade: WebSocket\r\n"
									 // remove a space from key1
									 "Sec-WebSocket-Key1: 4 @1 46546xW%0l 1 5\r\n"
									 "Origin: http://example.com\r\n"
									 "\r\n"
						 withKey3:@"^n:ds[4U"];
}

// Request w/o key3 should get no response
- (void)testHybi00HandshakeWithoutKey3 {
	[self compareCorrectHandshake:nil
		   withResponseForRequest:@"GET / HTTP/1.1\r\n"
									 // modified example request from hybi-00 spec
									 // w/o key3
									 "Host: example.com\r\n"
									 "Connection: Upgrade\r\n"
									 "Sec-WebSocket-Key2: 12998 5 Y3 1  .P00\r\n"
									 "Sec-WebSocket-Protocol: sample\r\n"
									 "Upgrade: WebSocket\r\n"
									 "Sec-WebSocket-Key1: 4 @1  46546xW%0l 1 5\r\n"
									 "Origin: http://example.com\r\n"
									 "\r\n"
						 withKey3:nil];
}

#pragma mark - RFC6455 tests

// Tests the reference handshake with RFC6455 WebSocket protocol.
- (void)testRFC6455Handshake {
	[self compareCorrectHandshake:@"HTTP/1.1 101 Switching Protocols\r\n"
									 "Upgrade: websocket\r\n"
									 "Connection: Upgrade\r\n"
									 "Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n"
									 "Sec-WebSocket-Protocol: chat\r\n"
									 "\r\n"
		   withResponseForRequest:@"GET / HTTP/1.1\r\n"
									 // modified example request from RFC6455 spec
									 "Host: server.example.com\r\n"
									 "Upgrade: websocket\r\n"
									 "Connection: Upgrade\r\n"
									 "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
									 "Origin: http://example.com\r\n"
									 "Sec-WebSocket-Protocol: chat\r\n"
									 "Sec-WebSocket-Version: 13\r\n"
									 "\r\n"
						 withKey3:nil];
}

// Request w/o key should get no response
- (void)testRFC6455HandshakeWithoutKey {
	[self compareCorrectHandshake:nil
		   withResponseForRequest:@"GET / HTTP/1.1\r\n"
									 // modified example request from RFC6455 spec
									 // w/o key
									 "Host: server.example.com\r\n"
									 "Upgrade: websocket\r\n"
									 "Connection: Upgrade\r\n"
									 "Origin: http://example.com\r\n"
									 "Sec-WebSocket-Protocol: chat\r\n"
									 "Sec-WebSocket-Version: 13\r\n"
									 "\r\n"
						 withKey3:nil];
}

@end
