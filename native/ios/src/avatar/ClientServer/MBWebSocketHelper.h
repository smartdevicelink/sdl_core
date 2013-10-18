//
//  MBWebSocketHelper.h
//  avatar
//
//  Created by Eugene Nikolsky on 7/9/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <Foundation/Foundation.h>

// Class that helps with dealing with WebSocket protocol.
// In particular, it generates accept key from request key(s),
// parses and prepares WebSocket headers.
// It's a singleton.
@interface MBWebSocketHelper : NSObject

+ (MBWebSocketHelper *)sharedHelper;

// Generates and returns accept key from one key (newer version
// of WebSocket protocol).
- (NSString *)acceptKeyFromRequestKey:(NSString *)key;
// Generates and returns accept key from three keys (older version
// of WebSocket protocol).
- (NSData *)acceptKeyFromRequestKey1:(NSString *)key1 key2:(NSString *)key2 andKey3:(NSData *)key3;

// Parses WebSocket incoming data.
- (NSData *)parseData:(NSData *)data;
// Generates WebSocket data header for the outgoing data.
- (NSData *)prepareHeaderForData:(NSData *)data;

@end
