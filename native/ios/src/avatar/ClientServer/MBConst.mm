//
//  MBConst.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/3/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "MBConst.h"
#import <MessageBroker/CMessageBroker.hpp>
#import "JSONKit.h"

#pragma mark - JSON-RPC message keys

NSString *const kMethodKey = @"method";
NSString *const kParametersKey = @"params";


#pragma mark - MessageBroker error codes

int const kMBControllerExists = NsMessageBroker::CONTROLLER_EXISTS;
int const kMBSubscriptionExists = NsMessageBroker::SUBSCRIBTION_EXISTS;
int const kMBParsingError = NsMessageBroker::PARSING_ERROR;
int const kMBInvalidRequest = NsMessageBroker::INVALID_REQUEST;
int const kMBMethodNotFound = NsMessageBroker::METHOD_NOT_FOUND;
int const kMBInvalidParams = NsMessageBroker::INVALID_PARAMS;
int const kMBInternalError = NsMessageBroker::INTERNAL_ERROR;

#pragma mark - Custom error codes

// according to the Backend.json spec
int const kMBRequestNotHandled = -1;
int const kMBCantSendEmail = -2;
int const kMBRateNoNetwork = 42;
int const kMBRateStoreNotAvailable = 43;


#pragma mark - MBConst class

#define kJSONKitErrorDomain			@"JKErrorDomain"
#define kJSONKitErrorAtIndexKey		@"JKAtIndexKey"

@implementation MBConst

+ (void)parseJSONMessagesFromData:(NSData *)data withProcessingBlock:(void (^)(id message))processingBlock {
	// no data => no messages
	if (data.length <= 0) {
		return;
	}

	// First, we try to parse the JSON data with default options (will fail if there is extra
	// data at the end of the first message).
	// If it fails and this is the error, split the data into two parts. Parse the first one
	// (should be OK now).
	// Go to the beginning of the algorithm with the rest.

	NSError *error = nil;
	do {
		DDLogVerbose(@"data.length = %d", data.length);
		DDLogVerbose(@"## data = %@", data);
		NSData *restOfData = nil;
		id jsonMessage = [data objectFromJSONDataWithParseOptions:JKParseOptionNone error:&error];
		if (error) {
			BOOL moreDataFound = NO;
			// check if we have more JSON messages
			if ([error.domain isEqualToString:kJSONKitErrorDomain] && (error.code == -1)) {
				NSNumber *messageEndIndex = [error.userInfo objectForKey:kJSONKitErrorAtIndexKey];
				DDLogVerbose(@"messageEndIndex = %@", messageEndIndex);
				if (messageEndIndex) {
					int index = [messageEndIndex intValue];
					DDLogVerbose(@"index = %d", index);
					if (index > 0) {
						// if the message ends with an LF char, skip it, because MessageBroker's and
						// WebSocket messages may have it at the end
						NSData *LFData = [@"\x0a" dataUsingEncoding:NSISOLatin1StringEncoding];
						BOOL trailingLF = [[data subdataWithRange:NSMakeRange(index - 1, 1)] isEqualToData:LFData];

						// check if the error is indeed about extra chars after the object
						NSData *endOfObjectData = [@"}" dataUsingEncoding:NSUTF8StringEncoding];
						if ([[data subdataWithRange:NSMakeRange(index - 1, 1)] isEqualToData:endOfObjectData] || trailingLF) {
							if (trailingLF) {
								DDLogVerbose(@"LF at the end of message, skipping!");
							}

							// split data into two parts
							restOfData = [data subdataWithRange:NSMakeRange(index, data.length - index)];
							data = [data subdataWithRange:NSMakeRange(0, index)];
							moreDataFound = YES;
						} else {
							DDLogVerbose(@"%@ not equal to end %@ or %@", [data subdataWithRange:NSMakeRange(index - 1, 1)], endOfObjectData, LFData);
						}
					}
				}
			}
			
			if (moreDataFound) {
				error = nil;
				jsonMessage = [data objectFromJSONDataWithParseOptions:JKParseOptionNone error:&error];
				
				if (error) {
					DDLogError(@"oh, error again: %@", error);
					break;
				} else {
					// do whatever you want with jsonMessage
					if (processingBlock) {
						processingBlock(jsonMessage);
					}
				}
			} else {
				DDLogWarn(@"parse error %@", error);
				break;
			}
		} else {
			// do whatever you want with jsonMessage
			if (processingBlock) {
				processingBlock(jsonMessage);
			}
			break;
		}
		
		data = restOfData;
	} while (data.length > 0);
}

@end