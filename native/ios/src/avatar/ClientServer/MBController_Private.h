//
//  MBController_Private.h
//  avatar
//
//  Created by Eugene Nikolsky on 4/23/13.
//  Copyright (c) 2013 Luxoft. All rights reserved.
//

#import "MBController.h"

#pragma mark - JSON keys

#define kJsonRpcKey					@"jsonrpc"
#define kMessageIdKey				@"id"
#define kResultKey					@"result"

#define kErrorKey					@"error"
#define kErrorCodeKey				@"code"
#define kErrorMessageKey			@"message"

#define kParameterComponentName		@"componentName"
#define kParameterPropertyName		@"propertyName"

#pragma mark - JSON values

#define kJsonRpcVersion				@"2.0"

#define kMBPrefix					@"MB."
#define kRegisterControllerMethod	kMBPrefix @"registerComponent"
#define kUnregisterControllerMethod	kMBPrefix @"unregisterComponent"
#define kSubscribeToPropertyMethod	kMBPrefix @"subscribeTo"
#define kUnsubscribeFromPropertyMethod	kMBPrefix @"unsubscribeFrom"


#pragma mark -

#define kMessageIdMaxNumber	1000


// Private methods of MBController interface.
// Imported by the implementation file and test files.
@interface MBController (Private)

@property (unsafe_unretained, nonatomic, readonly) NSNumber *nextMessageId;

// Checks if the message conforms to the JSON-RPC protocol.
// Returns a JSON-RPC error object if an error found.
- (NSDictionary *)checkMessage:(NSDictionary *)message;

// Returns YES if the message is a notification.
- (BOOL)messageIsNotification:(NSDictionary *)message;

// Returns YES if the message is a response.
- (BOOL)messageIsResponse:(NSDictionary *)message;

@end
