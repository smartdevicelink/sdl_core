//
//  MBWebSocketHelper.m
//  avatar
//
//  Created by Eugene Nikolsky on 7/9/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "MBWebSocketHelper.h"
#import <MessageBroker/websocket_handler.hpp>
#import <CommonCrypto/CommonDigest.h>

static MBWebSocketHelper *instance;


@interface MBWebSocketHelper ()

@property (nonatomic, assign) NsMessageBroker::CWebSocketHandler *handler;

@end


@implementation MBWebSocketHelper

@synthesize handler = _handler;

- (void)dealloc {
	delete _handler, _handler = nil;
}

+ (void)initialize {
	static BOOL initialized = NO;
	if (!initialized) {
		initialized = YES;
		instance = [[MBWebSocketHelper alloc] init];
	}
}

+ (MBWebSocketHelper *)sharedHelper {
	return instance;
}

- (id)init {
	if (self = [super init]) {
		_handler = new NsMessageBroker::CWebSocketHandler();
	}
	return self;
}

#pragma mark - Methods

- (NSString *)acceptKeyFromRequestKey:(NSString *)key {
	std::string wsKey = [key UTF8String];
	self.handler->handshake_0405(wsKey);
	return [NSString stringWithCString:wsKey.c_str() encoding:NSUTF8StringEncoding];
}

- (NSData *)acceptKeyFromRequestKey1:(NSString *)key1 key2:(NSString *)key2 andKey3:(NSData *)key3 {
	std::string wsKey1 = [key1 UTF8String];
	std::string wsKey2 = [key2 UTF8String];
	int key3Length = key3.length;
	char key3Bytes[key3Length];
	[key3 getBytes:&key3Bytes length:key3Length];
	NsMessageBroker::rawBytes wsKey3(&key3Bytes[0], &key3Bytes[key3Length]);
	
	NsMessageBroker::rawBytes wsAcceptKey = self.handler->handshake_hybi00(wsKey1, wsKey2, wsKey3);

	// if the result accept key is empty, we return nil
	NSData *acceptKey = (wsAcceptKey.size() > 0) ? [NSData dataWithBytes:&wsAcceptKey[0] length:wsAcceptKey.size()] : nil;
	
	return acceptKey;
}

- (NSData *)parseData:(NSData *)data {
	unsigned int bufferLength = data.length;
	char buffer[bufferLength];
	[data getBytes:&buffer length:bufferLength];
	self.handler->parseWebSocketData(buffer, bufferLength);
	return [NSData dataWithBytes:&buffer length:bufferLength];
}

- (NSData *)prepareHeaderForData:(NSData *)data {
	unsigned int bufferLength = data.length;
	static const int maxHeaderLength = 10;
	char header[maxHeaderLength];
	memset(&header, 0x00, maxHeaderLength);
	unsigned int headerLength = self.handler->prepareWebSocketDataHeader(header, bufferLength);
	return [NSData dataWithBytes:&header length:headerLength];
}

@end
