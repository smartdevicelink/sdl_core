//
//  MBClient.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 6/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MBClient.h"
#import "GCDAsyncSocket.h"

#define kServerRequestTag	142

#define kServerReplyTag		144


@interface MBClient () <GCDAsyncSocketDelegate>

@property (nonatomic, copy) NSString *host;
@property (nonatomic, assign) uint16_t port;

@property (nonatomic, retain) GCDAsyncSocket *socket;

@end

@implementation MBClient

@synthesize host = _host;
@synthesize port = _port;
@synthesize socket = _socket;

- (void)dealloc {
	DDLogVerbose(@" dealloc %@", self);
	
	[_host release], _host = nil;
	_socket.delegate = nil;
	[_socket release], _socket = nil;
	[super dealloc];
}

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port {
	if (self = [super init]) {
		_host = [host copy];
		_port = port;
		
		DDLogVerbose(@"creating MBClient: %@, %d", _host, _port);
		
		_socket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
	}
	return self;
}

#pragma mark - Methods

- (BOOL)connect {
	NSError *error = nil;
	BOOL result = [self.socket connectToHost:self.host onPort:self.port error:&error];
	if (!result) {
		DDLogError(@"%@ %@: connect error = %@", self, NSStringFromSelector(_cmd), error);
	}
	
	return result;
}

- (void)sendData:(NSData *)data {
	[self.socket writeData:data withTimeout:-1 tag:kServerRequestTag];
	[self.socket readDataWithTimeout:-1 tag:kServerReplyTag];
}

- (void)receiveData:(NSData *)data {
	DDLogError(@"MBClient receiveData: not implemented!");
}

#pragma mark - GCDAsyncSocketDelegate

- (void)socket:(GCDAsyncSocket *)sock didConnectToHost:(NSString *)host port:(uint16_t)port {
	DDLogInfo(@"socket:%@ didConnectToHost:%@ port:%d", sock, host, port);
}

- (void)socket:(GCDAsyncSocket *)sock didWriteDataWithTag:(long)tag {
	DDLogInfo(@"socket:%@ didWriteDataWithTag:%d", sock, tag);
}

- (void)socket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag {
	DDLogInfo(@"socket:%@ didReadData:%@ withTag:%d", sock, [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease], tag);
	
	switch (tag) {
		case kServerReplyTag:
			[self receiveData:data];
			break;
			
		default:
			DDLogWarn(@"readData: unknown tag = %d", tag);
			break;
	}
	
	[sock readDataWithTimeout:-1 tag:kServerReplyTag];
}

@end
