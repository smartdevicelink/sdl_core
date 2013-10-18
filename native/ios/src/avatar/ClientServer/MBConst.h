//
//  MBConst.h
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/3/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#pragma mark - JSON-RPC message keys

extern NSString *const kMethodKey;
extern NSString *const kParametersKey;


#pragma mark - MessageBroker error codes

extern int const kMBControllerExists;		/* Controller has been already registered. */
extern int const kMBSubscriptionExists;		/* Property has been already subscribed by this controller. */
extern int const kMBParsingError;			/* Invalid JSON. An error occurred on the server while parsing the JSON text. */
extern int const kMBInvalidRequest;			/* The received JSON not a valid MessageBroker Request. */
extern int const kMBMethodNotFound;			/* The requested remote-procedure does not exist / is not available. */
extern int const kMBInvalidParams;			/* Invalid method parameters. */
extern int const kMBInternalError;			/* Internal MessageBroker error. */


#pragma mark - Custom error codes

extern int const kMBRequestNotHandled;      /* The request has been received and parsed correctly, but the component couldn't handle it. */
extern int const kMBCantSendEmail;          /* The current configuration of the device doesn't support email sending. */
extern int const kMBRateNoNetwork;          /* The rate request can't be processed because there is no internets on the device. */
extern int const kMBRateStoreNotAvailable;  /* The rate request can't be processed because the device's Store seems to be down. */


#pragma mark - MBConst class

// A class that contains general MessageBroker-related functions
@interface MBConst : NSObject

// Gets some data that may contain 1+ JSON objects in a row and parses them
// one after another. For each object, processingBlock is called.
// Stops parsing when a parsing error occurs.
+ (void)parseJSONMessagesFromData:(NSData *)data withProcessingBlock:(void (^)(id message))processingBlock;

@end
