//
//  MBServer_Private.h
//  avatar
//
//  Created by Eugene Nikolsky on 8/13/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "MBServer.h"

// these constants are stored in socket's userData
// to identify protocol version
#define kWebSocketProtocolHybi00	[NSNumber numberWithInt:1]
#define kWebSocketProtocolRFC6455	[NSNumber numberWithInt:2]


// Private methods of MBServer class.
// Imported by the implementation file and tests files.
@interface MBServer (Private)

// Constructs and returns handshake response for WebSocket request
// Hybi-00 protocol when key3 is set.
// RFC6455 protocol when key3 is nil.
// May return nil if incorrect values are passed in the request.
- (NSData *)composeWebSocketHandshakeResponseFromRequest:(NSData *)request andKey3:(NSData *)key3;

// Returns the value of the field specified by range. It's looked up in the
// specified HTTP request as a string. If range is not found, an empty string is
// returned.
//
// E.g.:
//  request = "HTTP / 1.1\r\nHost: example.com\r\n\r\n"
//  range = {12, 6}  (which maps to "Host: ")
//  result -> "example.com"
- (NSString *)valueOfFieldWithRange:(NSRange)range inRequest:(NSString *)request;

// Returns YES if the request is likely to be from a WebSocket client
- (BOOL)isWebSocketRequest:(NSData *)request;

// Detects the WebSocket protocol version. Returns kWebSocketProtocolHybi00, or
// kWebSocketProtocolRFC6455, or nil if error.
- (NSNumber *)webSocketVersionOfRequest:(NSData *)request;

@end
