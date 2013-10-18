//
//  MBConst.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MBConst.h"
#import <MessageBrokerLib/CMessageBroker.hpp>
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


#pragma mark - MBConst class

#define kJSONKitErrorDomain			@"JKErrorDomain"
#define kJSONKitErrorAtIndexKey		@"JKAtIndexKey"

@implementation MBConst

+ (void)parseJSONMessagesFromData:(NSData *)data withProcessingBlock:(void (^)(id message))processingBlock {
	// First, we try to parse the JSON data with default options (will fail if there is extra
	// data at the end of the first message).
	// If it fails and this is the error, split the data into two parts. Parse the first one
	// (should be OK now).
	// Go to the beginning of the algorithm with the rest.
	
	NSError *error = nil;
	do {
		DDLogVerbose(@"data.length = %d", data.length);
		NSData *restOfData = nil;
		id jsonMessage = [data objectFromJSONDataWithParseOptions:JKParseOptionNone error:&error];
		if (error) {
			BOOL moreDataFound = NO;
			// check if we have more JSON messages
			if ([error.domain isEqualToString:kJSONKitErrorDomain] && (error.code == -1)) {
				NSNumber *messageEndIndex = [error.userInfo objectForKey:kJSONKitErrorAtIndexKey];
				if (messageEndIndex) {
					int index = [messageEndIndex intValue];
					// check if the error is indeed about extra chars after the object
					NSData *endOfObjectData = [@"}" dataUsingEncoding:NSUTF8StringEncoding];
					if ([[data subdataWithRange:NSMakeRange(index - 1, 1)] isEqualToData:endOfObjectData]) {
						// split data into two parts
						restOfData = [data subdataWithRange:NSMakeRange(index, data.length - index)];
						data = [data subdataWithRange:NSMakeRange(0, index)];
						moreDataFound = YES;
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