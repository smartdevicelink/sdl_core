//
//  MBClient.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 6/27/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "MBClient.h"
#import "GCDAsyncSocket.h"

#define kServerRequestTag	142

#define kServerReplyTag		144


@interface MBClient () <GCDAsyncSocketDelegate>

@property (nonatomic, copy) NSString *host;
@property (nonatomic, assign) uint16_t port;

@property (nonatomic, strong) GCDAsyncSocket *socket;

@end

@implementation MBClient

@synthesize host = _host;
@synthesize port = _port;
@synthesize socket = _socket;

- (void)dealloc {
	DDLogVerbose(@" dealloc %@", self);
	
	[_socket synchronouslySetDelegate:nil];
}

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port {
	if (self = [super init]) {
		_host = [host copy];
		_port = port;
		
		DDLogVerbose(@"creating MBClient: %@, %d", _host, _port);
		
		_socket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_queue_create("MBClients", 0)];
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

- (void)disconnect {
	[self.socket disconnect];
}

- (void)didConnect {
	// do nothing
}

- (void)sendData:(NSData *)data {
	if (self.socket.isDisconnected) {
		[self connect];
	}
	
	[self.socket writeData:data withTimeout:-1 tag:kServerRequestTag];
	[self.socket readDataWithTimeout:-1 tag:kServerReplyTag];
}

- (void)receiveData:(NSData *)data {
#pragma unused (data)
	DDLogError(@"MBClient receiveData: not implemented!");
}

#pragma mark - GCDAsyncSocketDelegate

- (void)socket:(GCDAsyncSocket *)sock didConnectToHost:(NSString *)host port:(uint16_t)port {
	DDLogVerbose(@"socket:%@ didConnectToHost:%@ port:%d", sock, host, port);
	
	[self didConnect];
}

- (void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err {
	DDLogVerbose(@"client socketDidDisconnect:%@ withError:%@", sock, err);
}

- (void)socket:(GCDAsyncSocket *)sock didWriteDataWithTag:(long)tag {
	DDLogVerbose(@"socket:%@ didWriteDataWithTag:%ld", sock, tag);
}

- (void)socket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag {
	DDLogVerbose(@"socket:%@ didReadData:%@ withTag:%ld", sock, [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding], tag);
	
	switch (tag) {
		case kServerReplyTag:
			[self receiveData:data];
			break;
			
		default:
			DDLogWarn(@"readData: unknown tag = %ld", tag);
			break;
	}
	
	[sock readDataWithTimeout:-1 tag:kServerReplyTag];
}

@end
