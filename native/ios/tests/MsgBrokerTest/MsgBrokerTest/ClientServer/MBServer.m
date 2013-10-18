//
//  MBServer.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 6/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MBServer.h"
#import "GCDAsyncSocket.h"
#import "JSONKit.h"
#import "MBConst.h"

#define kClientRequestTag	42

#define kClientReplyTag		44


@interface MBServer () <GCDAsyncSocketDelegate>

@property (nonatomic, assign) MBMessageBroker *messageBroker;

@property (nonatomic, copy) NSString *host;
@property (nonatomic, assign) uint16_t port;

@property (nonatomic, retain) GCDAsyncSocket *socket;
// The array stores the sockets of all connected clients
@property (nonatomic, retain) NSMutableArray *acceptedSockets;

// Returns the socket's fd
- (int)fileDescriptorOfSocket:(GCDAsyncSocket *)socket;

- (void)processClientRequest:(NSData *)data withFd:(int)fd;

@end

@implementation MBServer

@synthesize messageBroker = _messageBroker;
@synthesize host = _host;
@synthesize port = _port;
@synthesize socket = _socket;
@synthesize acceptedSockets = _acceptedSockets;

- (void)dealloc {
	DDLogVerbose(@" dealloc %@", self);
	
	// close all connections
	DDLogVerbose(@"closing %d connections", self.acceptedSockets.count);
	for (GCDAsyncSocket *socket in self.acceptedSockets) {
		socket.delegate = nil;
		[socket disconnect];
	}
	
	[_host release], _host = nil;
	_socket.delegate = nil;
	[_socket release], _socket = nil;
	[_acceptedSockets release], _acceptedSockets = nil;
	[super dealloc];
}

- (id)initWithHost:(NSString *)host port:(uint16_t)port andMessageBroker:(MBMessageBroker *)messageBroker {
	if (self = [super init]) {
		_host = [host copy];
		_port = port;
		_messageBroker = messageBroker;
		
		DDLogVerbose(@"creating MBServer: %@, %d, %@", _host, _port, _messageBroker);
		
		_acceptedSockets = [[NSMutableArray alloc] init];
		_socket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
	}
	return self;
}

#pragma mark - Methods

- (BOOL)listen {
	NSError *error = nil;
	BOOL result = [self.socket acceptOnPort:self.port error:&error];
	if (!result) {
		DDLogError(@"%@ %@: accept error = %@", self, NSStringFromSelector(_cmd), error);
	}
	
	return result;
}

- (int)sendString:(NSString *)string withFd:(int)fd {
	DDLogInfo(@"sendString: %@ fd: %d", string, fd);
	
	// look for the socket with the fd
	GCDAsyncSocket *socket = nil;
	__block BOOL socketFound = NO;
	for (socket in self.acceptedSockets) {
		[socket performBlock:^{
			if (fd == socket.socketFD) {
				socketFound = YES;
			}
		}];
		
		if (socketFound) {
			break;
		}
	}
	
	int bytesWritten = -1;
	if (socketFound) {
		DDLogVerbose(@"socket with fd=%d found: %@", fd, socket);
		
		NSData *data = [string dataUsingEncoding:NSUTF8StringEncoding];
		// assume the data has been sent
		[socket writeData:data withTimeout:-1 tag:kClientReplyTag];
		bytesWritten = data.length;
	} else {
		DDLogWarn(@"socket with fd=%d not found", fd);
	}
	
	return bytesWritten;
}

#pragma mark - Privates

- (int)fileDescriptorOfSocket:(GCDAsyncSocket *)socket {
	__block int fd = -1;
	[socket performBlock:^{
		fd = socket.socketFD;
	}];
	return fd;
}

- (void)processClientRequest:(NSData *)data withFd:(int)fd {
	[MBConst parseJSONMessagesFromData:data withProcessingBlock:^(id message) {
		if ([message isKindOfClass:[NSDictionary class]]) {
			NSString *jsonString = [message JSONString];
			[self.messageBroker receiveMessage:jsonString fromSocketWithFd:fd];
		} else {
			DDLogError(@"%@: json message is not an object: %@", NSStringFromSelector(_cmd), [message class]);
		}
	}];
}

#pragma mark - GCDAsyncSocketDelegate

- (void)socket:(GCDAsyncSocket *)sock didAcceptNewSocket:(GCDAsyncSocket *)newSocket {
	DDLogInfo(@"socket:%@ didAcceptNewSocket:%@", sock, newSocket);
	
	[self.acceptedSockets addObject:newSocket];
	[newSocket readDataWithTimeout:-1 tag:kClientRequestTag];
}

- (void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err {
	DDLogInfo(@"socketDidDisconnect:%@ withError:%@", sock, err);
	DDLogVerbose(@"index of socket: ", [self.acceptedSockets indexOfObject:sock]);
	
	[self.acceptedSockets removeObject:sock];
}

- (void)socket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag {
	DDLogVerbose(@"socket:%@ didReadData:%@ withTag:%d", sock, [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease], tag);
	
	switch (tag) {
		case kClientRequestTag:
			[self processClientRequest:data withFd:[self fileDescriptorOfSocket:sock]];
			break;
			
		default:
			DDLogWarn(@"readData: unknown tag = %d", tag);
			break;
	}
	
	[sock readDataWithTimeout:-1 tag:kClientRequestTag];
}

- (void)socket:(GCDAsyncSocket *)sock didWriteDataWithTag:(long)tag {
	DDLogVerbose(@"socket:%@ didWriteDataWithTag:%d", sock, tag);
	
	switch (tag) {
		case kClientReplyTag:
			// ok, nothing to do
			break;
			
		default:
			DDLogWarn(@"writeData: unknown tag = %d", tag);
			break;
	}
}

@end
