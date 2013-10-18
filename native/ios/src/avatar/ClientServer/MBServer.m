//
//  MBServer.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 6/27/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "MBServer_Private.h"
#import "GCDAsyncSocket.h"
#import "JSONKit.h"
#import "MBConst.h"
#import "MBWebSocketHelper.h"

enum {
	// the beginning of the very first request from a client
	kClientFirstRequestTag = 42,
	
	// === Native Client tags ===
	// continuation of a request from a native client
	kClientRequestContinuationTag,
	// start of a new request from a native client
	kClientNewRequestTag,
	
	// === HTTP Request tags ===
	// whole HTTP request (except for a few first bytes)
	kClientHTTPRequestTag,
	// additional Key3 in case of a Hybi-00 request
	kClientHTTPKey3RequestTag,
	
	// === WebSocket Hybi-00 tags ===
	// start byte of a request
	kClientNewWebSocketRequestTag,
	// stop byte of the request
	kClientWebSocketContinuationTag,
	
	// === WebSocket RFC6455 tags ===
	// 2 first bytes of a request, to determine if its length
	// is <= 125 or no
	kClientNewWebSocketRFC6455RequestTag,
	// the rest of the request when it's <= 125 bytes long
	kClientWebSocketRFC6455ContinuationTag,
	// continuation of the request's header in case
	// the message is > 125 bytes long
	kClientWebSocketRFC6455HeaderTag,
	// the rest of the request when it's > 125 bytes long
	kClientWebSocketRFC6455DataRequestTag,
};

#define kClientReplyTag		44

#define kWebSocketKey		@"Sec-WebSocket-Key: "
#define kWebSocketKeyLength	24
#define kWebSocketKey1		@"Sec-WebSocket-Key1: "
#define kWebSocketKey2		@"Sec-WebSocket-Key2: "
#define kWebSocketOrigin	@"Origin: "
#define kWebSocketHost		@"Host: "
#define kWebSocketProtocol	@"Sec-WebSocket-Protocol: "


@interface MBServer () <GCDAsyncSocketDelegate>

@property (nonatomic, unsafe_unretained) MBMessageBroker *messageBroker;

@property (nonatomic, copy) NSString *host;
@property (nonatomic, assign) uint16_t port;

@property (nonatomic, strong) GCDAsyncSocket *socket;
// The array stores the sockets of all connected clients
@property (nonatomic, strong) NSMutableArray *acceptedSockets;

// NSData keeping '}' symbol
@property (nonatomic, strong) NSData *closingSquareBracketData;
// The data of an incomplete request for each socket
@property (nonatomic, strong) NSMutableDictionary *savedRequestDataDict;


// Disconnects the specified socket.
- (void)disconnectSocket:(GCDAsyncSocket *)socket;
// Disconnects all client sockets.
- (void)disconnectAllClientSockets;

// Returns the socket's fd
- (NSNumber *)fileDescriptorOfSocket:(GCDAsyncSocket *)socket;
// Returns the socket with the specified fd.
- (GCDAsyncSocket *)socketWithFileDescriptor:(NSNumber *)fd;
// Returns the version of WebSocket client on the socket, or nil.
- (NSNumber *)webSocketVersionOfSocketWithFileDescriptor:(NSNumber *)fd;

@end

@implementation MBServer

@synthesize messageBroker = _messageBroker;
@synthesize host = _host;
@synthesize port = _port;
@synthesize socket = _socket;
@synthesize acceptedSockets = _acceptedSockets;
@synthesize closingSquareBracketData = _closingSquareBracketData;

- (void)dealloc {
	DDLogVerbose(@" dealloc %@", self);
	
	[_socket synchronouslySetDelegate:nil];
	[self disconnectAllClientSockets];
	[_socket disconnect];
	_socket = nil;
}

- (id)initWithHost:(NSString *)host port:(uint16_t)port andMessageBroker:(MBMessageBroker *)messageBroker {
	if (self = [super init]) {
		_host = [host copy];
		_port = port;
		_messageBroker = messageBroker;
		
		DDLogVerbose(@"creating MBServer: %@, %d, %@", _host, _port, _messageBroker);

		_closingSquareBracketData = [@"}" dataUsingEncoding:NSISOLatin1StringEncoding];
        _savedRequestDataDict = [NSMutableDictionary new];
		
		@synchronized(self.acceptedSockets) {
			_acceptedSockets = [[NSMutableArray alloc] init];
		}
		_socket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
	}
	return self;
}

- (void)disconnectAllClientSockets {
	// close all connections
	@synchronized(self.acceptedSockets) {
		DDLogVerbose(@"closing %d connections", self.acceptedSockets.count);
		for (GCDAsyncSocket *socket in self.acceptedSockets) {
			[socket synchronouslySetDelegate:nil];
			[socket disconnect];
		}
		[self.acceptedSockets removeAllObjects];
	}
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

- (void)stop {
	DDLogVerbose(@"stopping server");
	[self.socket disconnectAfterReadingAndWriting];
}

- (void)stopImmediately {
	DDLogVerbose(@"stopping server immediately");
	[self.socket synchronouslySetDelegate:nil];
	[self.socket disconnect];
}

- (int)sendData:(NSData *)data withFd:(NSNumber *)fd {
	DDLogVerbose(@"sendData: %@ fd: %@", data, fd);
	
	GCDAsyncSocket *socket = [self socketWithFileDescriptor:fd];
	
	int bytesWritten = -1;
	if (socket) {
		DDLogVerbose(@"socket with fd=%@ found: %@", fd, socket);
		
		NSNumber *webSocketVersion = [self webSocketVersionOfSocketWithFileDescriptor:fd];
		if ([webSocketVersion isEqualToNumber:kWebSocketProtocolRFC6455]) {
			DDLogVerbose(@"data to WebSocket RFC6455!");
			// insert WebSocket header in the beginning
			NSMutableData *header = [[[MBWebSocketHelper sharedHelper] prepareHeaderForData:data] mutableCopy];
			[header appendData:data];
			data = [header copy];
		} else if ([webSocketVersion isEqualToNumber:kWebSocketProtocolHybi00]) {
			DDLogVerbose(@"data to WebSocket hybi-00!");
			// enclose the message in start and stop bytes
			char startByte = '\0';
			NSMutableData *envelope = [NSMutableData dataWithBytes:&startByte length:1];
			[envelope appendData:data];
			char stopByte = '\xff';
			[envelope appendData:[NSData dataWithBytes:&stopByte length:1]];
			data = [envelope copy];
		}
		
		// assume the data has been sent
		[socket writeData:data withTimeout:-1 tag:kClientReplyTag];
		bytesWritten = data.length;
	} else {
		DDLogWarn(@"socket with fd=%@ not found", fd);
	}
	
	return bytesWritten;
}

#pragma mark - Privates

- (NSData *)savedRequestDataForSocket:(GCDAsyncSocket *)socket {
    return self.savedRequestDataDict[[self fileDescriptorOfSocket:socket]];
}

- (void)setSavedRequestData:(NSData *)data forSocket:(GCDAsyncSocket *)socket {
    id key = [self fileDescriptorOfSocket:socket];
    if (data) {
        self.savedRequestDataDict[key] = data;
    } else {
        [self.savedRequestDataDict removeObjectForKey:key];
    }
}

- (NSData *)webSocketStopByteData {
	return [NSData dataWithBytes:"\xFF" length:1];
}

- (NSNumber *)fileDescriptorOfSocket:(GCDAsyncSocket *)socket {
	__block NSNumber *fd = [NSNumber numberWithInt:-1];
	[socket performBlock:^{
		fd = [NSNumber numberWithInt:socket.socketFD];
	}];
	return fd;
}

- (GCDAsyncSocket *)socketWithFileDescriptor:(NSNumber *)fd {
	GCDAsyncSocket *socket = nil;
	__block BOOL socketFound = NO;
	int intFd = [fd intValue];
	@synchronized(self.acceptedSockets) {
		for (socket in self.acceptedSockets) {
			[socket performBlock:^{
				if (intFd == socket.socketFD) {
					socketFound = YES;
				}
			}];
			
			if (socketFound) {
				break;
			}
		}
	}
	
	return socketFound ? socket : nil;
}

- (NSNumber *)webSocketVersionOfSocketWithFileDescriptor:(NSNumber *)fd {
	GCDAsyncSocket *socket = [self socketWithFileDescriptor:fd];
	NSNumber *result = nil;
	if (socket) {
		result = socket.userData;
	}
	return result;
}

- (BOOL)isWebSocketRequest:(NSData *)request {
	NSString *stringData = [[NSString alloc] initWithData:request encoding:NSISOLatin1StringEncoding];
	return [stringData hasPrefix:@"GET "];
}

- (NSNumber *)webSocketVersionOfRequest:(NSData *)request {
	NSNumber *result = nil;
	
	NSString *stringRequest = [[NSString alloc] initWithData:request encoding:NSISOLatin1StringEncoding];
	NSRange keyFieldRange = [stringRequest rangeOfString:kWebSocketKey];
	
	if (keyFieldRange.location != NSNotFound) {
		result = kWebSocketProtocolRFC6455;
	} else {
		NSRange key1FieldRange = [stringRequest rangeOfString:kWebSocketKey1];
		NSRange key2FieldRange = [stringRequest rangeOfString:kWebSocketKey2];
		if ((key1FieldRange.location != NSNotFound) && (key2FieldRange.location != NSNotFound)) {
			result = kWebSocketProtocolHybi00;
		}
	}
	
	return result;
}

- (NSString *)valueOfFieldWithRange:(NSRange)range inRequest:(NSString *)request {
	if ((request != nil) &&
		(range.location != NSNotFound) &&
		(range.location < request.length)) {
		
		NSString *EOL = @"\r\n";
		NSRange searchRange = NSMakeRange(range.location,
										  request.length - range.location);
		NSRange EOLRange = [request rangeOfString:EOL
										  options:NSLiteralSearch
											range:searchRange];
		
		if (EOLRange.location != NSNotFound) {
			
			int rangeEnd = range.location + range.length;
			NSRange valueRange = NSMakeRange(rangeEnd,
											 EOLRange.location - rangeEnd);
			NSString *value = [request substringWithRange:valueRange];
			
			return value;
			
		}
	}
	
	return @"";
}

// Processes raw data from the socket.
- (void)processRequest:(NSData *)request fromSocket:(GCDAsyncSocket *)socket {
	// try to parse the JSON data
	// if valid, parse, response, read new message up to a "}"
	// else, save the data, read up to "}"
//	DDLogWarn(@"request = %@", request);

	NSError *error = nil;
	id jsonMessage = [request objectFromJSONDataWithParseOptions:JKParseOptionNone error:&error];
	if (error) {
        [self setSavedRequestData:request forSocket:socket];

		[socket readDataToData:self.closingSquareBracketData withTimeout:-1 tag:kClientRequestContinuationTag];
	} else {
		DDLogVerbose(@"Parsed JSON message: %@", jsonMessage);
        [self setSavedRequestData:nil forSocket:socket];

		if ([jsonMessage isKindOfClass:NSDictionary.class]) {
			NSString *jsonString = [jsonMessage JSONString];
			[self.messageBroker receiveMessage:jsonString fromSocketWithFd:[self fileDescriptorOfSocket:socket]];
		} else {
			DDLogError(@"%@: json message is not an object: %@", NSStringFromSelector(_cmd), [jsonMessage class]);
		}

		[socket readDataToData:self.closingSquareBracketData withTimeout:-1 tag:kClientNewRequestTag];
	}
}

- (void)processHTTPRequest:(NSData *)request fromSocket:(GCDAsyncSocket *)socket {
	// detect WebSocket version in the request
	NSNumber *webSocketVersion = [self webSocketVersionOfRequest:request];
	if ([webSocketVersion isEqualToNumber:kWebSocketProtocolRFC6455]) {
		DDLogInfo(@"WebSocket RFC6455 client");
		// send a handshake response
		[self processWebSocketHandshake:request withKey3:nil fromSocket:socket];
	} else if ([webSocketVersion isEqualToNumber:kWebSocketProtocolHybi00]) {
		DDLogInfo(@"WebSocket Hybi-00 client");
        [self setSavedRequestData:request forSocket:socket];
		// need to receive 8 bytes of key3
		[socket readDataToLength:8 withTimeout:-1 tag:kClientHTTPKey3RequestTag];
	} else {
		DDLogWarn(@"Unknown WebSocket client");
		[self disconnectSocket:socket];
	}
}

- (NSData *)composeWebSocketHandshakeResponseFromRequest:(NSData *)request andKey3:(NSData *)key3 {
	NSString *stringRequest = [[NSString alloc] initWithData:request encoding:NSISOLatin1StringEncoding];
	// look for a key in the request
	// there should be either Sec-WebSocket-Key: field (the newest protocol version)
	// or Sec-WebSocket-Key1: and …-Key2: fields (older version)
	NSRange keyFieldRange = [stringRequest rangeOfString:kWebSocketKey];

	// echo the WebSocket protocol
	NSRange webSocketProtocolRange = [stringRequest rangeOfString:kWebSocketProtocol];
	NSString *webSocketProtocol = [self valueOfFieldWithRange:webSocketProtocolRange inRequest:stringRequest];
	NSString *responseString = [NSString stringWithFormat:@"HTTP/1.1 101 Switching Protocols\r\nSec-WebSocket-Protocol: %@\r\nConnection: Upgrade\r\n", webSocketProtocol];
	NSMutableData *response = [NSMutableData dataWithData:[responseString dataUsingEncoding:NSUTF8StringEncoding]];

	if (keyFieldRange.location != NSNotFound) {
		NSString *responseKey = [stringRequest substringWithRange:NSMakeRange(keyFieldRange.location + keyFieldRange.length, kWebSocketKeyLength)];
		NSString *key = [[MBWebSocketHelper sharedHelper] acceptKeyFromRequestKey:responseKey];
		DDLogVerbose(@" key is %@", key);

		if (key) {
			NSString *responseEnding = [NSString stringWithFormat:@"Upgrade: websocket\r\nSec-WebSocket-Accept: %@\r\n\r\n", key];
			[response appendData:[responseEnding dataUsingEncoding:NSUTF8StringEncoding]];
		} else {
			response = nil;
		}
	} else {
		// older version
		NSRange key1FieldRange = [stringRequest rangeOfString:kWebSocketKey1];
		NSRange key2FieldRange = [stringRequest rangeOfString:kWebSocketKey2];
		if ((key1FieldRange.location != NSNotFound) && (key2FieldRange.location != NSNotFound)) {
			// now that we've found the keys, extract them to the EOL
			NSString *key1 = [self valueOfFieldWithRange:key1FieldRange inRequest:stringRequest];
			NSString *key2 = [self valueOfFieldWithRange:key2FieldRange inRequest:stringRequest];

			DDLogVerbose(@" key1 is %@, key2 is %@, key3 is %@", key1, key2, key3);

			NSData *key = [[MBWebSocketHelper sharedHelper] acceptKeyFromRequestKey1:key1 key2:key2 andKey3:key3];

			if (key) {
				NSString *responseEnding = [NSString stringWithFormat:@"Upgrade: WebSocket\r\nSec-WebSocket-Origin: %@\r\nSec-WebSocket-Location: ws://%@/\r\n\r\n",
											[self valueOfFieldWithRange:[stringRequest rangeOfString:kWebSocketOrigin] inRequest:stringRequest],	// Origin
											[self valueOfFieldWithRange:[stringRequest rangeOfString:kWebSocketHost] inRequest:stringRequest]	// Location
											];

				[response appendData:[responseEnding dataUsingEncoding:NSUTF8StringEncoding]];
				[response appendData:key];
			} else {
				response = nil;
			}
		} else {
			// neither newer nor older supported version
			response = nil;
		}
	}

	return [response copy];
}

// Responses to a WebSocket handshake request.
// Hybi-00 protocol when key3 is set.
// RFC6455 protocol when key3 is nil.
- (void)processWebSocketHandshake:(NSData *)request withKey3:(NSData *)key3 fromSocket:(GCDAsyncSocket *)socket {
	NSData *response = [self composeWebSocketHandshakeResponseFromRequest:request andKey3:key3];
	if (response) {
		// if response is available, send it and wait for requests
		[self sendData:response withFd:[self fileDescriptorOfSocket:socket]];

		// store the version of WebSocket protocol to know how to encode responses later
		NSNumber *webSocketVersion = [self webSocketVersionOfRequest:request];
		socket.userData = webSocketVersion;

		if ([webSocketVersion isEqualToNumber:kWebSocketProtocolHybi00]) {
			[socket readDataToData:[GCDAsyncSocket ZeroData] withTimeout:-1 tag:kClientNewWebSocketRequestTag];
		} else if ([webSocketVersion isEqualToNumber:kWebSocketProtocolRFC6455]) {
			// read next 2 bytes to determine how long the message is
			[socket readDataToLength:2 withTimeout:-1 tag:kClientNewWebSocketRFC6455RequestTag];
		} else {
			[self disconnectSocket:socket];
		}
	} else {
		[self disconnectSocket:socket];
	}
}

- (void)processWebSocketRequest:(NSData *)request fromSocket:(GCDAsyncSocket *)socket {
	// if the data is a 0xFF byte only, the connection should be closed
	if (request.length == 1) {
		DDLogInfo(@"Received %@, disconnecting", request);
		[self disconnectSocket:socket];
	} else {
		// strip off the trailing 0xff byte
		NSRange requestRange = NSMakeRange(0, request.length - 1);
		request = [request subdataWithRange:requestRange];
//		DDLogInfo(@"WebSocket request = %@", request);

		// pass the message to Message Broker
		NSError *error = nil;
		id jsonMessage = [request objectFromJSONDataWithParseOptions:JKParseOptionNone error:&error];
		if (!error) {
			DDLogVerbose(@"Parsed JSON message: %@", jsonMessage);

			if ([jsonMessage isKindOfClass:NSDictionary.class]) {
				NSString *jsonString = [jsonMessage JSONString];
				[self.messageBroker receiveMessage:jsonString fromSocketWithFd:[self fileDescriptorOfSocket:socket]];
			} else {
				DDLogError(@"%@: json message is not an object: %@", NSStringFromSelector(_cmd), [jsonMessage class]);
			}
		} else {
			DDLogWarn(@"JSON message parsing error: %@", error);
		}

		// wait for a new request
		[socket readDataToData:[GCDAsyncSocket ZeroData] withTimeout:-1 tag:kClientNewWebSocketRequestTag];
	}
}

- (void)processWebSocketRFC6455Request:(NSData *)request fromSocket:(GCDAsyncSocket *)socket {
	DDLogVerbose(@"request = %@", request);
	NSAssert(request.length >= 2, @"Request is too short");

	// first byte has some service bits
	unsigned char opcode = 0;
	NSRange opcodeRange = NSMakeRange(0, 1);
	[request getBytes:&opcode range:opcodeRange];
	// clear the high half-byte
	opcode &= 0x0F;
	
	// opcodes as defined by RFC 6455
	enum {
		OpcodeContinuationFrame = 0,
		OpcodeTextFrame = 1,
		OpcodeBinaryFrame = 2,
		OpcodeConnectionClose = 8,
		OpcodePing = 9,
		OpcodePong = 0xA
	};
	
	switch (opcode) {
		case OpcodeBinaryFrame:
		case OpcodeContinuationFrame:
		case OpcodeTextFrame:
			// skip the return
			break;
			
		case OpcodeConnectionClose:
			// falling through
		default:
			// all the other types are not supported
			[self disconnectSocket:socket];
			return;
	}
	
	// second byte specifies payload length
	unsigned char payloadLen = 0;
	NSRange payloadLenRange = NSMakeRange(1, 1);
	[request getBytes:&payloadLen range:payloadLenRange];

	BOOL masked = (payloadLen & 0x80) == 0x80;
	if (!masked) {
		DDLogError(@"All messages from client must be masked");
	}

	static const int maskLength = 4;
	// clear the highest bit
	payloadLen &= 0x7F;
	switch (payloadLen) {
		case 126:
			// read 2 bytes of length + 4 bytes of mask
			[socket readDataToLength:2 + maskLength withTimeout:-1 tag:kClientWebSocketRFC6455HeaderTag];
			break;

		case 127:
			// read 8 bytes of length + 4 bytes of mask
			[socket readDataToLength:8 + maskLength withTimeout:-1 tag:kClientWebSocketRFC6455HeaderTag];
			break;

		default:
			// read 4 bytes of mask + payloadLen bytes of message
			[socket readDataToLength:maskLength + payloadLen withTimeout:-1 tag:kClientWebSocketRFC6455ContinuationTag];
			break;
	}
}

- (void)parseWebSocketRFC6455Data:(NSData *)data withMask:(NSData *)mask fromSocket:(GCDAsyncSocket *)socket {
	static const int maskLength = 4;
	int firstMessageByte = 0;

	// if mask is not specified, extract it from the data
	if (!mask) {
		NSRange maskRange = NSMakeRange(0, maskLength);
		mask = [data subdataWithRange:maskRange];
		firstMessageByte += maskLength;
	}

	unsigned char maskBytes[maskLength];
	[mask getBytes:&maskBytes length:maskLength];

	NSRange messageRange = NSMakeRange(firstMessageByte, data.length - firstMessageByte);
	unsigned char messageBytes[messageRange.length];
	[data getBytes:&messageBytes range:messageRange];

	// demask the message bytes
	for (NSUInteger i = 0; i < messageRange.length; ++i) {
		messageBytes[i] ^= maskBytes[i % maskLength];
	}

	NSData *request = [NSData dataWithBytes:messageBytes length:messageRange.length];
//	DDLogInfo(@"decoded bytes = %@", request);

	// pass the message to Message Broker
	NSError *error = nil;
	id jsonMessage = [request objectFromJSONDataWithParseOptions:JKParseOptionNone error:&error];
	if (!error) {
		DDLogVerbose(@"Parsed JSON message: %@", jsonMessage);

		if ([jsonMessage isKindOfClass:NSDictionary.class]) {
			NSString *jsonString = [jsonMessage JSONString];
			[self.messageBroker receiveMessage:jsonString fromSocketWithFd:[self fileDescriptorOfSocket:socket]];
		} else {
			DDLogError(@"%@: json message is not an object: %@", NSStringFromSelector(_cmd), [jsonMessage class]);
		}
	} else {
		DDLogWarn(@"JSON message parsing error: %@", error);
	}

	// read new message
	[socket readDataToLength:2 withTimeout:-1 tag:kClientNewWebSocketRFC6455RequestTag];
}

- (void)parseWebSocketRFC6455HeaderData:(NSData *)data fromSocket:(GCDAsyncSocket *)socket {
	// header data is 2 or 8 bytes of payload length + 4 bytes of mask
	static const int maskLength = 4;
	NSRange maskRange = NSMakeRange(data.length - maskLength, maskLength);
	NSData *mask = nil;

	switch (data.length) {
		case 2 + maskLength: {
			uint16_t payloadLen = 0;
			[data getBytes:&payloadLen length:2];
			payloadLen = NSSwapBigShortToHost(payloadLen);
			DDLogVerbose(@"payloadLen = %d", payloadLen);
			[socket readDataToLength:payloadLen withTimeout:-1 tag:kClientWebSocketRFC6455DataRequestTag];

			mask = [data subdataWithRange:maskRange];
			break;
		}

		case 8 + maskLength: {
			uint64_t payloadLen = 0;
			[data getBytes:&payloadLen length:8];
			payloadLen = NSSwapBigLongLongToHost(payloadLen);
			DDLogVerbose(@"payloadLen = %lld", payloadLen);
			[socket readDataToLength:payloadLen withTimeout:-1 tag:kClientWebSocketRFC6455DataRequestTag];

			mask = [data subdataWithRange:maskRange];
			break;
		}

		default:
			DDLogWarn(@"header data is %d bytes long!", data.length);
			[self disconnectSocket:socket];
			break;
	}

	// save the mask for future decoding
    [self setSavedRequestData:mask forSocket:socket];
}

- (void)disconnectSocket:(GCDAsyncSocket *)socket {
	DDLogInfo(@"Server: disconnecting client %@", socket);
	[socket disconnectAfterWriting];
}

#pragma mark - GCDAsyncSocketDelegate

- (void)socket:(GCDAsyncSocket *)sock didAcceptNewSocket:(GCDAsyncSocket *)newSocket {
	DDLogVerbose(@"socket:%@ didAcceptNewSocket:%@", sock, newSocket);
	
	@synchronized(self.acceptedSockets) {
		[self.acceptedSockets addObject:newSocket];
	}
	[newSocket readDataToLength:4 withTimeout:-1 tag:kClientFirstRequestTag];
}

- (void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err {
	DDLogVerbose(@"socketDidDisconnect:%@ withError:%@", sock, err);
	
	if (sock != self.socket) {
		@synchronized(self.acceptedSockets) {
			DDLogVerbose(@"index of socket: %d", [self.acceptedSockets indexOfObject:sock]);
			
			[self.acceptedSockets removeObject:sock];
		}
	}
}

- (void)socket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag {
	DDLogVerbose(@"socket:%@ didReadData:%@ withTag:%ld", sock, [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding], tag);

	switch (tag) {
		case kClientFirstRequestTag: {
			// first request (4 bytes) from client
			// can contain either "{xxx" (for local clients) or
			// "GET " (for WebSocket clients)

			NSData *openingSquareBracketData = [@"{" dataUsingEncoding:NSISOLatin1StringEncoding];
			// check the first byte
			if ([[data subdataWithRange:NSMakeRange(0, 1)] isEqualToData:openingSquareBracketData]) {
				// save the data
                [self setSavedRequestData:data forSocket:sock];
				// read up to a closing square bracket
				[sock readDataToData:self.closingSquareBracketData withTimeout:-1 tag:kClientRequestContinuationTag];
			} else if ([self isWebSocketRequest:data]) {
				// an HTTP request
				DDLogVerbose(@"HTTP protocol detected! Should be a WebSocket request.");
                [self setSavedRequestData:data forSocket:sock];

				// read HTTP headers up to double CRLF
				NSMutableData *doubleCRLF = [NSMutableData dataWithData:[GCDAsyncSocket CRLFData]];
				[doubleCRLF appendData:[GCDAsyncSocket CRLFData]];
				[sock readDataToData:doubleCRLF withTimeout:-1 tag:kClientHTTPRequestTag];
			} else {
				// don't support any other type of clients
				[self disconnectSocket:sock];
			}

			break;
		}

		case kClientRequestContinuationTag: {
			NSData *fullData = data;
            NSData *savedData = [self savedRequestDataForSocket:sock];
			if (savedData) {
				// continuation of the first request, append it with the new data
				NSMutableData *tempData = [NSMutableData dataWithData:savedData];
				[tempData appendData:data];
				fullData = [tempData copy];
                [self setSavedRequestData:nil forSocket:sock];
			} else {
				DDLogWarn(@"savedRequestData is nil!");
			}

			[self processRequest:fullData fromSocket:sock];
			break;
		}

		case kClientNewRequestTag: {
			// a new request is coming

            NSData *savedData = [self savedRequestDataForSocket:sock];
			if (savedData) {
				DDLogWarn(@"savedRequestData is not nil: %@", savedData);
                [self setSavedRequestData:nil forSocket:sock];
			}

			[self processRequest:data fromSocket:sock];
			break;
		}

		case kClientHTTPRequestTag: {
			// read HTTP request
			NSData *fullData = data;
            NSData *savedData = [self savedRequestDataForSocket:sock];
			if (savedData) {
				// continuation of the first request, append it with the new data
				NSMutableData *tempData = [NSMutableData dataWithData:savedData];
				[tempData appendData:data];
				fullData = [tempData copy];
                [self setSavedRequestData:nil forSocket:sock];
			} else {
				DDLogWarn(@"savedRequestData is nil!");
			}

			// should be either Hybi-00 or RFC6455 WebSocket request
			[self processHTTPRequest:fullData fromSocket:sock];
			break;
		}

		case kClientHTTPKey3RequestTag: {
			// got key3
			NSData *request = [self savedRequestDataForSocket:sock];
			if (request) {
                [self setSavedRequestData:nil forSocket:sock];
			} else {
				DDLogWarn(@"No saved WebSocket request found!");
			}
			[self processWebSocketHandshake:request withKey3:data fromSocket:sock];
			break;
		}

		case kClientNewWebSocketRequestTag: {
			// received the start byte of a new WebSocket request
			if (data.length != 1) {
				DDLogWarn(@"Should have received a 0x00 byte, but received %@", data);
			}

			// read until the end of the message
			[sock readDataToData:[self webSocketStopByteData] withTimeout:-1 tag:kClientWebSocketContinuationTag];

			break;
		}

		case kClientWebSocketContinuationTag: {
			// received the stop byte of a WebSocket request
			[self processWebSocketRequest:data fromSocket:sock];

			break;
		}

		case kClientNewWebSocketRFC6455RequestTag: {
			// data is first 2 bytes of an RFC6455 request
			[self processWebSocketRFC6455Request:data fromSocket:sock];
			break;
		}

		case kClientWebSocketRFC6455ContinuationTag: {
			// end of RFC6455 frame
			// 4 bytes of mask + (0..125) bytes of message
			[self parseWebSocketRFC6455Data:data withMask:nil fromSocket:sock];

			break;
		}

		case kClientWebSocketRFC6455HeaderTag: {
			// continuation of RFC6455 frame
			// either 2 or 8 bytes of payload length + 4 bytes of mask
			[self parseWebSocketRFC6455HeaderData:data fromSocket:sock];
			break;
		}

		case kClientWebSocketRFC6455DataRequestTag: {
			// end of RFC6455 frame, which is > 125 bytes long
			NSData *mask = [self savedRequestDataForSocket:sock];
			if (mask) {
                [self setSavedRequestData:nil forSocket:sock];
			} else {
				DDLogError(@"mask is nil");
			}
			[self parseWebSocketRFC6455Data:data withMask:mask fromSocket:sock];
			break;
		}

		default:
			DDLogWarn(@"readData: unknown tag = %ld", tag);
			break;
	}
}

- (void)socket:(GCDAsyncSocket *)sock didWriteDataWithTag:(long)tag {
	DDLogVerbose(@"socket:%@ didWriteDataWithTag:%ld", sock, tag);
	
	switch (tag) {
		case kClientReplyTag:
			// ok, nothing to do
			break;
			
		default:
			DDLogWarn(@"writeData: unknown tag = %ld", tag);
			break;
	}
}

@end
