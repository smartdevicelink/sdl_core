//
//  MBController.h
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MBClient.h"

/* A message broker controller that handles one part of functionality. */
@interface MBController : MBClient

// Constructor that accepts the name of the controller.
// When instantiating children of the class use the
// initWithHost:andPort: constructor.
// name will be specified by the child class.
- (id)initWithHost:(NSString *)host port:(uint16_t)port andName:(NSString *)name;

// Registers the controller on the server.
// startId should be unique within the application
- (void)registerControllerWithStartId:(NSInteger)startId;

// Unregisters the controller.
- (void)unregisterController;

// Subscribes the controller to the property changing events.
- (void)subscribeToProperty:(NSString *)property;

// Unsubscribes the controller from the property changing events.
- (void)unsubscribeFromProperty:(NSString *)property;


// Adds standard fields to conform to the JSON-RPC standard.
// message should not be nil.
- (void)prepareMessage:(NSMutableDictionary *)message;

// Convenience constructor of a JSON-RPC message with a method and one parameter.
- (NSMutableDictionary *)constructMessageWithMethod:(NSString *)method andParameter:(NSString *)parameter;

// Convenience constructor of a JSON-RPC notification with a method and one parameter.
- (NSMutableDictionary *)constructNotificationWithMethod:(NSString *)method andParameter:(NSString *)parameter;

// Prepares a response for the messasge.
- (NSMutableDictionary *)prepareResponse:(NSString *)response forMessage:(NSDictionary *)message;

// Prepares an error response for the message.
- (NSMutableDictionary *)prepareErrorResponseWithCode:(NSInteger)errorCode message:(NSString *)errorMessage forMessage:(NSDictionary *)message;


// Returns the method's name (w/o the controller's name) of the message.
// E.g., "makeCall", not "Phone.makeCall"
- (NSString *)methodNameOfMessage:(NSDictionary *)message;


// Sends the JSON message to the server.
- (void)sendJsonMessage:(NSDictionary *)message;

// Processes notification sent to the controller.
// Should be overridden in child classes.
- (void)processNotification:(NSDictionary *)notification;

// Processes response sent to the controller.
// method is name of the method that sent the request.
// Should be overridden in child classes.
- (void)processResponse:(NSDictionary *)response withMethodName:(NSString *)method;

// Processes request to the controller.
// Should be overridden in child classes.
- (void)processRequest:(NSDictionary *)request;

@end
