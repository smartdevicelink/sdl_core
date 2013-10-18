//
//  MBServerTest.m
//  avatar
//
//  Created by Eugene Nikolsky on 8/9/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <GHUnitIOS/GHUnit.h>
#import <GCDAsyncSocket.h>
#import <OCMock/OCMock.h>
#import <JSONKit.h>
#import "MBServer_Private.h"
#import "MBClient.h"
#import "MBTestConst.h"

#define kHost	@"127.1"


@interface MBServerTest : GHTestCase

@property (nonatomic, strong) MBServer *server;

@end

@implementation MBServerTest

- (void)setUpClass {
	self.server = [[MBServer alloc] initWithHost:nil port:0 andMessageBroker:0];
}

- (void)tearDownClass {
	self.server = nil;
}

#pragma mark - -[MBServer valueOfFieldWithRange:inRequest:] Tests

// Tests the base, correct case of getting a value of field in request.
- (void)testValueOfFieldInRequest {
	NSString *request = @"HTTP / 1.1\r\nHost: example.com\r\n\r\n";
	NSRange range = [request rangeOfString:@"Host: "];
	NSString *result = [self.server valueOfFieldWithRange:range
												inRequest:request];
	GHAssertNotNil(result, @"The value must not be nil");
	GHAssertEqualObjects(result, @"example.com", @"The value is incorrect");
}

// Tests that the method returns an empty string when range.location ==
// NSNotFound
- (void)testValueOfFieldInRequestWithRangeNotFound {
	NSString *request = @"HTTP / 1.1\r\nHost: example.com\r\n\r\n";
	NSRange range = NSMakeRange(NSNotFound, 0);
	NSString *result = [self.server valueOfFieldWithRange:range
												inRequest:request];
	GHAssertNotNil(result, @"The value must not be nil");
	GHAssertEqualObjects(result, @"",
						 @"The value must be an empty string when range not found");
}

// Tests that the method returns an empty string when range.location is beyond
// the request string
- (void)testValueOfFieldInRequestWithRangeBeyondRequest {
	NSString *request = @"HTTP / 1.1\r\nHost: example.com\r\n\r\n";
	NSRange range = NSMakeRange(request.length, 1);
	NSString *result = [self.server valueOfFieldWithRange:range
												inRequest:request];
	GHAssertNotNil(result, @"The value must not be nil");
	GHAssertEqualObjects(result, @"",
						 @"The value must be an empty string when range is beyond the request");
}

// Tests that the method returns an empty string when (range.location +
// range.length) is beyond the request string
- (void)testValueOfFieldInRequestWithRangeLengthBeyondRequest {
	NSString *request = @"HTTP / 1.1\r\nHost: example.com\r\n\r\n";
	NSRange range = NSMakeRange(request.length - 1, 2);
	NSString *result = [self.server valueOfFieldWithRange:range
												inRequest:request];
	GHAssertNotNil(result, @"The value must not be nil");
	GHAssertEqualObjects(result, @"",
						 @"The value must be an empty string when range is beyond the request");
}

// Tests that the method returns an empty string when the value string doesn't
// have CRLF.
- (void)testValueOfFieldInRequestWoTrailingCRLF {
	NSString *request = @"HTTP / 1.1\r\nHost: example.com";
	NSRange range = [request rangeOfString:@"Host: "];
	NSString *result = [self.server valueOfFieldWithRange:range
												inRequest:request];
	GHAssertNotNil(result, @"The value must not be nil");
	GHAssertEqualObjects(result, @"", @"The value is incorrect");
}

#pragma mark - -[MBServer isWebSocketRequest:] Tests

// "GET / …" is a WebSocket request
- (void)testIsWebSocketRequestGET {
	NSString *requestString = @"GET / HTTP/1.1\r\n";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	GHAssertTrue([self.server isWebSocketRequest:request],
				 @"GET is a WebSocket request");
}

// " GET / …" is an invalid HTTP request
- (void)testIsWebSocketRequestGETWithLeadingSpace {
	NSString *requestString = @" GET / HTTP/1.1\r\n";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	GHAssertFalse([self.server isWebSocketRequest:request],
				 @"GET with leading whitespace is invalid HTTP request");
}

// "POST / …" is an invalid WebSocket request
- (void)testIsWebSocketRequestPOST {
	NSString *requestString = @"POST / HTTP/1.1\r\n";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	GHAssertFalse([self.server isWebSocketRequest:request],
				 @"POST is not a WebSocket request");
}

// Our internal protocol is not a WebSocket request
- (void)testIsWebSocketRequestInternalProtocol {
	NSString *requestString = @"{\"jsonrpc\": \"2.0\", \"method\": \"notify\"}";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	GHAssertFalse([self.server isWebSocketRequest:request],
				  @"Internal protocol is not a WebSocket request");
}

#pragma mark - -[MBServer webSocketVersionOfRequest:] Tests

// Tests the method with a correct Hybi-00 request
- (void)testWebSocketVersionHybi00 {
	NSString *requestString =
		@"GET / HTTP/1.1\r\n"
		// modified example request from hybi-00 spec
		"Host: example.com\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Key2: 12998 5 Y3 1  .P00\r\n"
		"Sec-WebSocket-Protocol: sample\r\n"
		"Upgrade: WebSocket\r\n"
		"Sec-WebSocket-Key1: 4 @1  46546xW%0l 1 5\r\n"
		"Origin: http://example.com\r\n"
		"\r\n";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	NSNumber *version = [self.server webSocketVersionOfRequest:request];
	GHAssertNotNil(version, @"Version must be not nil");
	GHAssertEqualObjects(version, kWebSocketProtocolHybi00,
						 @"This is a Hybi-00 request");
}

// Tests the method with an incorrect Hybi-00 request, w/o Key 1
- (void)testWebSocketVersionHybi00MissingKey1 {
	NSString *requestString =
		@"GET / HTTP/1.1\r\n"
		"Host: example.com\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Protocol: sample\r\n"
		"Upgrade: WebSocket\r\n"
		"Sec-WebSocket-Key2: 4 @1  46546xW%0l 1 5\r\n"
		"Origin: http://example.com\r\n"
		"\r\n";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	NSNumber *version = [self.server webSocketVersionOfRequest:request];
	GHAssertNil(version, @"This is an invalid Hybi-00 request");
}

// Tests the method with an incorrect Hybi-00 request, w/o Key 2
- (void)testWebSocketVersionHybi00MissingKey2 {
	NSString *requestString =
		@"GET / HTTP/1.1\r\n"
		"Host: example.com\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Protocol: sample\r\n"
		"Upgrade: WebSocket\r\n"
		"Sec-WebSocket-Key1: 4 @1  46546xW%0l 1 5\r\n"
		"Origin: http://example.com\r\n"
		"\r\n";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	NSNumber *version = [self.server webSocketVersionOfRequest:request];
	GHAssertNil(version, @"This is an invalid Hybi-00 request");
}

// Tests the method with an incorrect Hybi-00 request, w/o Key 1 and Key 2
- (void)testWebSocketVersionHybi00MissingKey1AndKey2 {
	NSString *requestString =
		@"GET / HTTP/1.1\r\n"
		"Host: example.com\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Protocol: sample\r\n"
		"Upgrade: WebSocket\r\n"
		"Origin: http://example.com\r\n"
		"\r\n";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	NSNumber *version = [self.server webSocketVersionOfRequest:request];
	GHAssertNil(version, @"This is an invalid Hybi-00 request");
}

// Tests the method with a correct RFC6455 request
- (void)testWebSocketVersionRFC6455 {
	NSString *requestString =
		@"GET / HTTP/1.1\r\n"
		// modified example request from RFC6455 spec
		"Host: server.example.com\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
		"Origin: http://example.com\r\n"
		"Sec-WebSocket-Protocol: chat\r\n"
		"Sec-WebSocket-Version: 13\r\n"
		"\r\n";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	NSNumber *version = [self.server webSocketVersionOfRequest:request];
	GHAssertNotNil(version, @"Version must be not nil");
	GHAssertEqualObjects(version, kWebSocketProtocolRFC6455,
						 @"This is an RFC 6455 request");
}

// Tests the method with an incorrect RFC6455 request, w/o Key
- (void)testWebSocketVersionRFC6455MissingKey {
	NSString *requestString =
		@"GET / HTTP/1.1\r\n"
		"Host: server.example.com\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Origin: http://example.com\r\n"
		"Sec-WebSocket-Protocol: chat\r\n"
		"Sec-WebSocket-Version: 13\r\n"
		"\r\n";
	NSData *request = [requestString dataUsingEncoding:NSUTF8StringEncoding];
	NSNumber *version = [self.server webSocketVersionOfRequest:request];
	GHAssertNil(version, @"This is an invalid RFC 6455 request");
}

// The method should return Nil on empty request
- (void)testWebSocketVersionOfEmptyRequest {
	NSData *request = [NSData data];
	NSNumber *version = [self.server webSocketVersionOfRequest:request];
	GHAssertNil(version, @"Should return Nil on empty request");
}

// The method should return Nil on Nil request
- (void)testWebSocketVersionOfNilRequest {
	NSData *request = nil;
	NSNumber *version = [self.server webSocketVersionOfRequest:request];
	GHAssertNil(version, @"Should return Nil on Nil request");
}

@end


@interface MBServerStartupTest : GHAsyncTestCase <GCDAsyncSocketDelegate>

@property (nonatomic, strong) MBServer *server;
@property (nonatomic, strong) GCDAsyncSocket *clientSocket;

@end


@implementation MBServerStartupTest

@synthesize server = _server;
@synthesize clientSocket = _clientSocket;

// Tests if the server starts up and listens to connections.
- (void)testServerStartup {
	[self prepare];

	static const uint16_t port = 8000;
	// initialize the server
	self.server = [[MBServer alloc] initWithHost:kHost port:port andMessageBroker:nil];
	GHAssertNotNil(self.server, @"Server is not created");

	BOOL result = [self.server listen];
	GHAssertTrue(result, @"Listen failed");

	// initialize the client
	self.clientSocket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_global_queue(0, 0)];
	NSError *error = nil;
	result = [self.clientSocket connectToHost:kHost onPort:port withTimeout:.5 error:&error];
	GHAssertTrue(result, @"Connect to host failed");
	GHAssertNil(error, @"Connect error");

	// wait for results
	[self waitForStatus:kGHUnitWaitStatusSuccess timeout:1.];
	
	// cleanup
	[self.server stopImmediately];
	self.server = nil;
	
	[self.clientSocket synchronouslySetDelegate:nil];
	[self.clientSocket disconnect];
	self.clientSocket = nil;
}

#pragma mark - GCDAsyncSocketDelegate

- (void)socket:(GCDAsyncSocket *)sock didConnectToHost:(NSString *)host port:(uint16_t)port {
#pragma unused (host, port)
	GHAssertEquals(sock, self.clientSocket, @"didConnect from unexpected socket");
	
	dispatch_async(dispatch_get_main_queue(), ^{
		[self notify:kGHUnitWaitStatusSuccess forSelector:@selector(testServerStartup)];
	});
}

- (void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err {
#pragma unused (sock, err)
	[self notify:kGHUnitWaitStatusFailure forSelector:@selector(testServerStartup)];
}

@end



@interface MBServerParseMessagesTest : GHAsyncTestCase <GCDAsyncSocketDelegate>

@property (nonatomic, strong) id mbMock;
@property (nonatomic, strong) MBServer *server;
@property (nonatomic, strong) GCDAsyncSocket *clientSocket;
@property (nonatomic, assign) int receiveMessageCallCount;

@end


@implementation MBServerParseMessagesTest

@synthesize mbMock = _mbMock;
@synthesize server = _server;
@synthesize clientSocket = _clientSocket;
@synthesize receiveMessageCallCount = _receiveMessageCallCount;

// Tests that the server accepts two joined messages and forwards them
// to a message broker (a mock here)
- (void)testParseTwoMessages {
	[self prepare];
	
	self.receiveMessageCallCount = 0;

	self.mbMock = [OCMockObject mockForClass:[MBMessageBroker class]];
	[[[self.mbMock stub] andDo:^(NSInvocation *invocation) {
#pragma unused (invocation)
		++self.receiveMessageCallCount;
		
		if (self.receiveMessageCallCount == 2) {
			[self notify:kGHUnitWaitStatusSuccess forSelector:@selector(testParseTwoMessages)];
		}
	}] receiveMessage:[OCMArg any] fromSocketWithFd:[OCMArg any]];

	static const uint16_t port = 8001;
	// initialize the server
	self.server = [[MBServer alloc] initWithHost:kHost port:port andMessageBroker:self.mbMock];
	GHAssertNotNil(self.server, @"Server is not created");

	BOOL result = [self.server listen];
	GHAssertTrue(result, @"Listen failed");

	// initialize the client
	self.clientSocket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_global_queue(0, 0)];
	NSError *error = nil;
	result = [self.clientSocket connectToHost:kHost onPort:port withTimeout:.5 error:&error];
	GHAssertTrue(result, @"Connect to host failed");
	GHAssertNil(error, @"Connect error");

	// wait for results
	[self waitForStatus:kGHUnitWaitStatusSuccess timeout:3.];
	
	// cleanup
	[self.server stopImmediately];
	self.server = nil;
	
	[self.clientSocket synchronouslySetDelegate:nil];
	[self.clientSocket disconnect];
	self.clientSocket = nil;
}

#pragma mark - GCDAsyncSocketDelegate

- (void)socket:(GCDAsyncSocket *)sock didConnectToHost:(NSString *)host port:(uint16_t)port {
#pragma unused (host, port)
	NSDictionary *message = [NSDictionary dictionaryWithObjectsAndKeys:
							 @"2.0", kJsonRpcKey,
							 [NSNumber numberWithInt:42], kMessageIdKey,
							 @"unknown", kMethodKey,
							 nil];
	NSMutableData *data = [NSMutableData dataWithData:[message JSONData]];
	[data appendData:[message JSONData]];
	[sock writeData:[data copy] withTimeout:.5 tag:20];
}

- (void)socket:(GCDAsyncSocket *)sock didWriteDataWithTag:(long)tag {
#pragma unused (tag)
	GHAssertEquals(sock, self.clientSocket, @"didWriteData from unexpected socket");
}

- (void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err {
#pragma unused (sock, err)
	[self notify:kGHUnitWaitStatusFailure forSelector:@selector(testParseTwoMessages)];
}

@end
