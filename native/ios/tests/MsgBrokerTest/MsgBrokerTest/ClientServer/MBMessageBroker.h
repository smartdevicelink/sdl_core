//
//  MBMessageBroker.h
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol MBMessageBrokerSender;

// Wrapper around C++'s MessageBroker class.
// It's a singleton.
@interface MBMessageBroker : NSObject

// Returns the singleton instance.
+ (MBMessageBroker *)sharedMessageBroker;

// Starts the MessageBroker.
// sender is an object that will send the messages to the clients.
// dispatchQueue is a queue where the message broker will handle the messages.
- (void)startWithSender:(id<MBMessageBrokerSender>)sender inDispatchQueue:(dispatch_queue_t)dispatchQueue;

// Stops the MessageBroker.
- (void)stop;

// Handle the message received from a socket.
- (void)receiveMessage:(NSString *)message fromSocketWithFd:(int)fd;

@end



// Counterpart of C++'s CSender interface.
// Used by MessageBroker to send data to clients.
@protocol MBMessageBrokerSender <NSObject>

@required
// Sends data to a client with the specified fd
// Returns number of bytes sent or -1 on error
- (int)sendString:(NSString *)string withFd:(int)fd;

@end
