//
//  MBController.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MBController.h"
#import "JSONKit.h"
#import "MBConst.h"

#pragma mark - JSON keys

#define kJsonRpcKey					@"jsonrpc"
#define kMessageIdKey				@"id"
#define kResultKey					@"result"

#define kErrorKey					@"error"
#define kErrorCodeKey				@"code"
#define kErrorMessageKey			@"message"

#pragma mark - JSON values

#define kJsonRpcVersion				@"2.0"

#define kMBPrefix					@"MB."
#define kRegisterControllerMethod	kMBPrefix @"registerComponent"
#define kUnregisterControllerMethod	kMBPrefix @"unregisterComponent"
#define kSubscribeToPropertyMethod	kMBPrefix @"subscribeTo"
#define kUnsubscribeFromPropertyMethod	kMBPrefix @"unsubscribeFrom"

#pragma mark -

#define kMessageIdMaxNumber	1000

#pragma mark -

@interface MBController () {
	NSInteger _nextMessageId;
}

@property (nonatomic, copy) NSString *name;
@property (nonatomic, assign) NSInteger baseMessageId;
@property (nonatomic, readonly) NSNumber *nextMessageId;
@property (nonatomic, retain) NSMutableDictionary *waitingResponses;

// Checks if the message conforms to the JSON-RPC protocol.
// Returns a JSON-RPC error object if an error found.
- (NSDictionary *)checkMessage:(NSDictionary *)message;

// Returns YES if the message is a notification.
- (BOOL)messageIsNotification:(NSDictionary *)message;

// Returns YES if the message is a response.
- (BOOL)messageIsResponse:(NSDictionary *)message;

@end

@implementation MBController

@synthesize name = _name;
@synthesize baseMessageId = _baseMessageId;
@synthesize waitingResponses = _waitingResponses;

- (void)dealloc {
	[_name release], _name = nil;
	[_waitingResponses release], _waitingResponses = nil;
	[super dealloc];
}

- (id)initWithHost:(NSString *)host port:(uint16_t)port andName:(NSString *)name {
	if (self = [super initWithHost:host andPort:port]) {
		_name = [name copy];
		// self.nextMessageId will be increased to 0 in the getter
		_nextMessageId = -1;
		_waitingResponses = [[NSMutableDictionary alloc] init];
	}
	return self;
}

#pragma mark - Methods

- (void)registerControllerWithStartId:(NSInteger)startId {
	DDLogVerbose(@"registerController startId = %d", startId);
	
	NSMutableDictionary *msg = [self constructMessageWithMethod:kRegisterControllerMethod andParameter:self.name];
	
	// overwrite message id set by prepareMessage:
	[msg setObject:[NSNumber numberWithInteger:startId] forKey:kMessageIdKey];
	
	[self sendJsonMessage:msg];
}

- (void)unregisterController {
	DDLogVerbose(@"unregisterController");
	
	NSMutableDictionary *msg = [self constructMessageWithMethod:kUnregisterControllerMethod andParameter:self.name];
	[self sendJsonMessage:msg];
}

- (void)subscribeToProperty:(NSString *)property {
	DDLogVerbose(@"subscribeToProperty: %@", property);
	NSAssert(property, @"property not set");
	
	NSMutableDictionary *msg = [self constructMessageWithMethod:kSubscribeToPropertyMethod andParameter:property];
	[self sendJsonMessage:msg];
}

- (void)unsubscribeFromProperty:(NSString *)property {
	DDLogVerbose(@"unsubscribeFromProperty: %@", property);
	NSAssert(property, @"property not set");
	
	NSMutableDictionary *msg = [self constructMessageWithMethod:kUnsubscribeFromPropertyMethod andParameter:property];
	[self sendJsonMessage:msg];
}


- (void)receiveData:(NSData *)data {
	[MBConst parseJSONMessagesFromData:data withProcessingBlock:^(id jsonMessage) {
		if ([jsonMessage isKindOfClass:[NSDictionary class]]) {
			NSDictionary *message = jsonMessage;
			NSDictionary *error = [self checkMessage:message];
			if (error == nil) {
				if ([self messageIsNotification:message]) {
					[self processNotification:message];
				} else if ([self messageIsResponse:message]) {
					NSNumber *messageId = [message objectForKey:kMessageIdKey];
					NSString *messageMethod = [self.waitingResponses objectForKey:messageId];
					if (messageMethod) {
						DDLogVerbose(@" message %@ is response for %@", messageId, messageMethod);
						[self.waitingResponses removeObjectForKey:messageId];
						
						if ([messageMethod isEqualToString:kRegisterControllerMethod]) {
							// init the base number for messages' ids
							NSNumber *result = [message objectForKey:kResultKey];
							DDLogVerbose(@"setting baseMessageId = %d", [result integerValue]);
							self.baseMessageId = [result integerValue];
						} else if ([messageMethod hasPrefix:kMBPrefix]) {
							// skip for now
						} else {
							[self processResponse:message withMethodName:messageMethod];
						}
					} else {
						DDLogWarn(@"%@: request with id %@ not found", NSStringFromSelector(_cmd), messageId);
					}
				} else { // request
					[self processRequest:message];
				}
			} else {
				DDLogWarn(@"%@: message contains an error: %@", NSStringFromSelector(_cmd), error);
			}
		} else {
			DDLogError(@"%@: json message is not an object: %@", NSStringFromSelector(_cmd), [jsonMessage class]);
		}
	}];
}

- (void)processNotification:(NSDictionary *)notification {
	DDLogWarn(@" [MBController] processNotification:%@", notification);
}

- (void)processResponse:(NSDictionary *)response withMethodName:(NSString *)method {
	DDLogWarn(@" [MBController] processResponse:%@ withMethodName:%@", response, method);
}

- (void)processRequest:(NSDictionary *)request {
	DDLogWarn(@" [MBController] processRequest:%@", request);
	
	// reply with Not Implemented message
	// now there is a bug in MessageBroker that sends a reply to an error response,
	// so we'll send a regular response for now
	[self sendJsonMessage:[self prepareResponse:@"Method not implemented" forMessage:request]];
/*	[self sendJsonMessage:[self prepareErrorResponseWithCode:kMBInvalidRequest
													 message:NSLocalizedString(@"Method not implemented", nil)
												  forMessage:request]];*/
}

#pragma mark - JSON

- (void)prepareMessage:(NSMutableDictionary *)message {
	NSAssert(message != nil, @"prepareMessage: message is nil");
	
	[message setObject:kJsonRpcVersion forKey:kJsonRpcKey];
	[message setObject:[self.nextMessageId stringValue] forKey:kMessageIdKey];
}

- (NSMutableDictionary *)constructMessageWithMethod:(NSString *)method andParameter:(NSString *)parameter {
	DDLogVerbose(@"constructMessage: method %@, parameter %@", method, parameter);
	
	NSMutableDictionary *message = [NSMutableDictionary dictionary];
	[self prepareMessage:message];
	
	[message setObject:method forKey:kMethodKey];
	[message setObject:parameter forKey:kParametersKey];
	
	return message;
}

- (NSMutableDictionary *)constructNotificationWithMethod:(NSString *)method andParameter:(NSString *)parameter {
	DDLogVerbose(@"constructNotification: method %@, parameter %@", method, parameter);
	
	NSMutableDictionary *notification = [NSMutableDictionary dictionaryWithCapacity:3];
	[notification setObject:kJsonRpcVersion forKey:kJsonRpcKey];
	[notification setObject:method forKey:kMethodKey];
	[notification setObject:parameter forKey:kParametersKey];
	
	return notification;
}

- (NSMutableDictionary *)prepareResponse:(NSString *)response forMessage:(NSDictionary *)message {
	DDLogVerbose(@"prepareResponse:%@ forMessage:%@", response, message);
	
	NSMutableDictionary *msg = [NSMutableDictionary dictionaryWithCapacity:3];
	
	// copy id and json-rpc version from the original message
	[msg setObject:[message objectForKey:kJsonRpcKey] forKey:kJsonRpcKey];
	[msg setObject:[message objectForKey:kMessageIdKey] forKey:kMessageIdKey];
	
	[msg setObject:response forKey:kResultKey];
	
	return msg;
}

- (NSMutableDictionary *)prepareErrorResponseWithCode:(NSInteger)errorCode message:(NSString *)errorMessage forMessage:(NSDictionary *)message {
	DDLogVerbose(@"prepareErrorResponseWithCode:%d messasge:%@ forMessage:%@", errorCode, errorMessage, message);
	
	NSMutableDictionary *msg = [NSMutableDictionary dictionaryWithCapacity:3];
	
	// copy id and json-rpc version from the original message
	[msg setObject:[message objectForKey:kJsonRpcKey] forKey:kJsonRpcKey];
	[msg setObject:[message objectForKey:kMessageIdKey] forKey:kMessageIdKey];
	
	NSDictionary *errorObject = [NSDictionary dictionaryWithObjectsAndKeys:
								 [NSNumber numberWithInteger:errorCode], kErrorCodeKey,
								 errorMessage ? errorMessage : @"", kErrorMessageKey,
								 nil];
	[msg setObject:errorObject forKey:kErrorKey];
	
	return msg;
}


- (NSString *)methodNameOfMessage:(NSDictionary *)message {
	NSString *messageMethod = [message objectForKey:kMethodKey];
	return [messageMethod substringFromIndex:([messageMethod rangeOfString:@"."].location + 1)];
}


- (void)sendJsonMessage:(NSDictionary *)message {
	DDLogInfo(@"sendJsonMessage: %@", [message JSONString]);
	
	if (![self messageIsNotification:message] && ![self messageIsResponse:message]) {
		id messageId = [message objectForKey:kMessageIdKey];
		id messageMethod = [message objectForKey:kMethodKey];
		[self.waitingResponses setObject:messageMethod forKey:messageId];
	}
	
	[self sendData:[message JSONData]];
}

- (NSDictionary *)checkMessage:(NSDictionary *)message {
	NSDictionary *errorObject = [NSDictionary dictionaryWithObjectsAndKeys:
								 [NSNumber numberWithInt:kMBInvalidRequest], kErrorCodeKey,
								 NSLocalizedString(@"Invalid MessageBroker request.", @"When received message is not JSON-RPC-conforming."), kErrorMessageKey,
								 nil];
	
	NSDictionary *error = [NSDictionary dictionaryWithObjectsAndKeys:
						   [NSNull null], kMessageIdKey,
						   kJsonRpcVersion, kJsonRpcKey,
						   errorObject, kErrorKey,
						   nil];
	
	// check that the version is correct
	NSString *jsonRpcVersion = [message objectForKey:kJsonRpcKey];
	if (![jsonRpcVersion isEqualToString:kJsonRpcVersion]) {
		return error;
	}
	
	// check that id is available
	id jsonId = [message objectForKey:kMessageIdKey];
	if (jsonId && ([jsonId isKindOfClass:[NSArray class]] || [jsonId isKindOfClass:[NSDictionary class]])) {
		return error;
	}
	
	// check that method is string, if available
	id jsonMethod = [message objectForKey:kMethodKey];
	if (jsonMethod && (![jsonMethod isKindOfClass:[NSString class]])) {
		return error;
	}
	
	return nil;
}

- (BOOL)messageIsNotification:(NSDictionary *)message {
	id jsonId = [message objectForKey:kMessageIdKey];
	return jsonId == nil;
}

- (BOOL)messageIsResponse:(NSDictionary *)message {
	id jsonResult = [message objectForKey:kResultKey];
	id jsonError = [message objectForKey:kErrorKey];
	return (jsonResult != nil) || (jsonError != nil);
}

#pragma mark - Getters & Setters

// Returns the id of next message in range
// [self.baseMessageId, self.baseMessageId + kMessageIdMaxNumber)
- (NSNumber *)nextMessageId {
	_nextMessageId = self.baseMessageId + ((_nextMessageId + 1) % kMessageIdMaxNumber);
	
	return [NSNumber numberWithInteger:_nextMessageId];
}

@end
