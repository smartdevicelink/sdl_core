//
//  MBController.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/2/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "MBController_Private.h"
#import "JSONKit.h"
#import "MBConst.h"

@interface MBController () {
	NSInteger _nextMessageId;
}

@property (nonatomic, copy) NSString *name;
@property (nonatomic, assign) NSInteger baseMessageId;
@property (nonatomic, strong) NSMutableDictionary *waitingResponses;

@end

@implementation MBController

@synthesize name = _name;
@synthesize baseMessageId = _baseMessageId;
@synthesize waitingResponses = _waitingResponses;

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

- (void)registerControllerWithStartId:(NSNumber *)startId {
	DDLogVerbose(@"registerController startId = %@", startId);
	
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:self.name forKey:kParameterComponentName];
	NSMutableDictionary *msg = [self constructMessageWithMethod:kRegisterControllerMethod andParameter:parameters];
	
	// overwrite message id set by prepareMessage:
	[msg setObject:startId forKey:kMessageIdKey];
	
	[self sendJsonMessage:msg];
}

- (void)unregisterController {
	DDLogVerbose(@"unregisterController");
	
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:self.name forKey:kParameterComponentName];
	NSMutableDictionary *msg = [self constructMessageWithMethod:kUnregisterControllerMethod andParameter:parameters];
	[self sendJsonMessage:msg];
}

- (void)subscribeToProperty:(NSString *)property {
	DDLogVerbose(@"subscribeToProperty: %@", property);
	NSAssert(property, @"property not set");
	
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:property forKey:kParameterPropertyName];
	NSMutableDictionary *msg = [self constructMessageWithMethod:kSubscribeToPropertyMethod andParameter:parameters];
	[self sendJsonMessage:msg];
}

- (void)unsubscribeFromProperty:(NSString *)property {
	DDLogVerbose(@"unsubscribeFromProperty: %@", property);
	NSAssert(property, @"property not set");
	
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:property forKey:kParameterPropertyName];
	NSMutableDictionary *msg = [self constructMessageWithMethod:kUnsubscribeFromPropertyMethod andParameter:parameters];
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

							[self didRegisterController];
						} else if ([messageMethod hasPrefix:kUnregisterControllerMethod]) {
							DDLogVerbose(@"unregistering done, disconnecting.");
							[self disconnect];
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

- (void)didRegisterController {
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
	[self sendJsonMessage:[self prepareErrorResponseWithCode:kMBInvalidRequest
													 message:NSLocalizedString(@"Method not implemented", nil)
												  forMessage:request]];
}

#pragma mark - JSON

- (void)prepareMessage:(NSMutableDictionary *)message {
	NSAssert(message != nil, @"prepareMessage: message is nil");
	
	[message setObject:kJsonRpcVersion forKey:kJsonRpcKey];
	[message setObject:self.nextMessageId forKey:kMessageIdKey];
}

- (NSMutableDictionary *)constructMessageWithMethod:(NSString *)method andParameter:(NSDictionary *)parameter {
	DDLogVerbose(@"constructMessage: method %@, parameter %@", method, parameter);
	
	NSMutableDictionary *message = [NSMutableDictionary dictionary];
	[self prepareMessage:message];
	
	[message setObject:method forKey:kMethodKey];
	if (parameter) {
		[message setObject:parameter forKey:kParametersKey];
	}
	
	return message;
}

- (NSMutableDictionary *)constructNotificationWithMethod:(NSString *)method andParameter:(NSDictionary *)parameter {
	DDLogVerbose(@"constructNotification: method %@, parameter %@", method, parameter);
	
	NSMutableDictionary *notification = [NSMutableDictionary dictionaryWithCapacity:3];
	[notification setObject:kJsonRpcVersion forKey:kJsonRpcKey];
	[notification setObject:method forKey:kMethodKey];
	if (parameter) {
		[notification setObject:parameter forKey:kParametersKey];
	}
	
	return notification;
}

- (NSMutableDictionary *)prepareResponse:(id)response forMessage:(NSDictionary *)message {
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
	NSUInteger lastDotIndex = [messageMethod rangeOfString:@"." options:NSBackwardsSearch].location;
	if (lastDotIndex != NSNotFound) {
		// last dot found => extract the name to the end
		return [messageMethod substringFromIndex:lastDotIndex + 1];
	}
	// not found => return the whole string
	return messageMethod;
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
	
	// check that id is a number, if available
	id jsonId = [message objectForKey:kMessageIdKey];
	if (jsonId && (![jsonId isKindOfClass:[NSNumber class]])) {
		return error;
	}
	
	// check that method is string, if available
	id jsonMethod = [message objectForKey:kMethodKey];
	if (jsonMethod && (![jsonMethod isKindOfClass:[NSString class]])) {
		return error;
	}
	
	// check that parameters is object, if available
	id jsonParameters = [message objectForKey:kParametersKey];
	if (jsonParameters && (![jsonParameters isKindOfClass:[NSDictionary class]])) {
		return error;
	}
	
	// check that error is object, if available
	id jsonError = [message objectForKey:kErrorKey];
	if (jsonError && (![jsonError isKindOfClass:[NSDictionary class]])) {
		return error;
	}
	
	// response is invalid if it contains both result and error keys
	if ([message objectForKey:kResultKey] && jsonError) {
		return error;
	}
	
	// check that the message doesn't have any unexpected fields
	NSArray *supportedKeys = @[kJsonRpcKey, kMessageIdKey, kMethodKey,
							kParametersKey, kResultKey, kErrorKey];
	for (NSString *key in [message allKeys]) {
		if ([supportedKeys indexOfObject:key] == NSNotFound) {
			return error;
		}
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
	BOOL containsRequiredField = (jsonResult || jsonError);
	BOOL containsBothRequiredFields = (jsonResult && jsonError);
	return containsRequiredField && !containsBothRequiredFields;
}

#pragma mark - Overridden methods

- (void)didConnect {
	// during the normal flow, the controller has connected to the server
	// and this method is called. registerControllerWithStartId: hasn't been
	// called yet, so we don't have that number.
	// use a random number, which should be different each time with an app launch
	[self registerControllerWithStartId:@(arc4random())];
}

#pragma mark - Getters & Setters

// Returns the id of next message in range
// [self.baseMessageId, self.baseMessageId + kMessageIdMaxNumber)
- (NSNumber *)nextMessageId {
	_nextMessageId = self.baseMessageId + ((_nextMessageId + 1) % kMessageIdMaxNumber);
	
	return [NSNumber numberWithInteger:_nextMessageId];
}

@end
