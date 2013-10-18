//
//  MBWebSocketHelperTest.m
//  avatar
//
//  Created by Eugene Nikolsky on 8/7/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <GHUnitIOS/GHUnit.h>
#import "MBWebSocketHelper.h"


@interface MBWebSocketHelperTest : GHTestCase

@end


@implementation MBWebSocketHelperTest

// Tests if the singleton has been created
- (void)testMBWebSocketHelperSingleton {
	MBWebSocketHelper *helper = [MBWebSocketHelper sharedHelper];
	GHAssertNotNil(helper, @"sharedHelper is not created");
}

#pragma mark - RFC6455 Protocol

// Tests the generation of accept key according to RFC 6455.
// http://datatracker.ietf.org/doc/rfc6455/?include_text=1
- (void)testRFC6455AcceptKey {
	NSString *webSocketKey = @"dGhlIHNhbXBsZSBub25jZQ==";
	NSString *acceptKey = [[MBWebSocketHelper sharedHelper] acceptKeyFromRequestKey:webSocketKey];
	GHAssertEqualStrings(acceptKey, @"s3pPLMBiTxaQ9kYGzzhZRbK+xOo=", @"Generated accept key doesn't match the correct one");
}

// Tests parsing of a single-frame unmasked text message
- (void)testParseSingleFrameUnmaskedTextMessage {
	const char dataBytes[] = {0x81, 0x05, 0x48, 0x65, 0x6c, 0x6c, 0x6f};
	NSData *data = [NSData dataWithBytes:&dataBytes length:sizeof(dataBytes)];

	NSData *result = [[MBWebSocketHelper sharedHelper] parseData:data];
	NSString *resultString = [[NSString alloc] initWithData:result encoding:NSISOLatin1StringEncoding];
	GHAssertEqualStrings(resultString, @"Hello", @"Decoded string doesn't match");
}

// Tests parsing of a single-frame masked text message
- (void)testParseSingleFrameMaskedTextMessage {
	const char dataBytes[] = {0x81, 0x85, 0x37, 0xfa, 0x21, 0x3d, 0x7f, 0x9f, 0x4d, 0x51, 0x58};
	NSData *data = [NSData dataWithBytes:&dataBytes length:sizeof(dataBytes)];

	NSData *result = [[MBWebSocketHelper sharedHelper] parseData:data];
	NSString *resultString = [[NSString alloc] initWithData:result encoding:NSISOLatin1StringEncoding];
	GHAssertEqualStrings(resultString, @"Hello", @"Decoded string doesn't match");
}

// Tests that the data frame for a small message (<= 125 bytes) is correct
- (void)testPrepareHeaderForSmallMessage {
	char dataBytes[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
	NSData *data = [NSData dataWithBytesNoCopy:&dataBytes
										length:sizeof(dataBytes)
								  freeWhenDone:NO];
	
	NSData *header = [[MBWebSocketHelper sharedHelper]
					  prepareHeaderForData:data];
	GHAssertNotNil(header, @"Header must not be nil");
	const NSUInteger correctLength = 2u;
	GHAssertEquals(header.length, correctLength,
				   @"Header must be %d bytes long", correctLength);
	
	char headerBytes[correctLength];
	[header getBytes:&headerBytes length:correctLength];
	// check RSV fields
	GHAssertEquals(headerBytes[0] & 0b01110000, '\0', @"RSV fields must be 0");
	// check opcode == for text frame
	GHAssertEquals(headerBytes[0] & 0b00000001, '\x01',
				   @"Opcode must be for text frame");
	GHAssertEquals(headerBytes[1] & 0b10000000, '\0', @"Mask bit must be 0");
	uint8_t payloadLength = headerBytes[1] & 0b01111111;
	GHAssertEquals(payloadLength, (uint8_t) sizeof(dataBytes),
				   @"Payload length is incorrect");
}

// Tests that the data frame for a medium message (126…65536 bytes) is correct
- (void)testPrepareHeaderForMediumMessage {
	const int dataLength = 256;
	NSData *data = [[NSMutableData dataWithLength:dataLength] copy];
	
	NSData *header = [[MBWebSocketHelper sharedHelper]
					  prepareHeaderForData:data];
	GHAssertNotNil(header, @"Header must not be nil");
	const NSUInteger correctLength = 4u;
	GHAssertEquals(header.length, correctLength,
				   @"Header must be %d bytes long", correctLength);
	
	char headerBytes[correctLength];
	[header getBytes:&headerBytes length:correctLength];
	// check RSV fields
	GHAssertEquals(headerBytes[0] & 0b01110000, '\0', @"RSV fields must be 0");
	// check opcode == for text frame
	GHAssertEquals(headerBytes[0] & 0b00000001, '\x01',
				   @"Opcode must be for text frame");
	GHAssertEquals(headerBytes[1] & 0b10000000, '\0', @"Mask bit must be 0");
	
	uint8_t payloadLength = headerBytes[1] & 0b01111111;
	GHAssertEquals(payloadLength, (uint8_t) 126, @"Payload length is incorrect");
	// here, extPayloadLength is intentionally constructed in the wrong order
	// of bytes, and then converted to network-byte order
	uint16_t extPayloadLength = (headerBytes[3] << 8) | headerBytes[2];
	extPayloadLength = htons(extPayloadLength);
	GHAssertEquals(extPayloadLength, (uint16_t) dataLength,
				   @"Extended payload length is incorrect");
}

/*
// Tests that the data frame for a large message (65536…2^64 bytes) is correct
- (void)testPrepareHeaderForLargeMessage {
	const int dataLength = 66066;
	NSData *data = [[NSMutableData dataWithLength:dataLength] copy];
	
	NSData *header = [[MBWebSocketHelper sharedHelper]
					  prepareHeaderForData:data];
	GHAssertNotNil(header, @"Header must not be nil");
	const NSUInteger correctLength = 10u;
	GHAssertEquals(header.length, correctLength,
				   @"Header must be %d bytes long", correctLength);
	
	char headerBytes[correctLength];
	[header getBytes:&headerBytes length:correctLength];
	// check RSV fields
	GHAssertEquals(headerBytes[0] & 0b01110000, '\0', @"RSV fields must be 0");
	// check opcode == for text frame
	GHAssertEquals(headerBytes[0] & 0b00000001, '\x01',
				   @"Opcode must be for text frame");
	GHAssertEquals(headerBytes[1] & 0b10000000, '\0', @"Mask bit must be 0");
	
	uint8_t payloadLength = headerBytes[1] & 0b01111111;
	GHAssertEquals(payloadLength, (uint8_t) 127, @"Payload length is incorrect");
	// get 2…10 bytes of header as a uint64_t
	void *headerBytesPtr = (void *)headerBytes;
	uint64_t extPayloadLength = ((uint64_t *) (headerBytesPtr + 2))[0];
	extPayloadLength = OSSwapHostToBigConstInt64(extPayloadLength);
	GHAssertEquals(extPayloadLength, (uint64_t) dataLength,
				   @"Extended payload length is incorrect");
}
*/

#pragma mark - Hybi-00 Protocol

// Tests the generation of accept key according to the older Hybi-00 spec.
// http://tools.ietf.org/html/draft-ietf-hybi-thewebsocketprotocol-00
- (void)testHybi00AcceptKey {
	NSString *requestKey1 = @"3e6b263  4 17 80";
	NSString *requestKey2 = @"17  9 G`ZD9   2 2b 7X 3 /r90";
	NSData *requestKey3 = [@"WjN}|M(6" dataUsingEncoding:NSISOLatin1StringEncoding];

	NSData *acceptKey = [[MBWebSocketHelper sharedHelper] acceptKeyFromRequestKey1:requestKey1
																			  key2:requestKey2
																		   andKey3:requestKey3];
	NSData *correctAcceptKey = [@"n`9eBk9z$R8pOtVb" dataUsingEncoding:NSISOLatin1StringEncoding];
	GHAssertEqualObjects(acceptKey, correctAcceptKey, @"Generated accept key doesn't match the correct one");
}

// When a request key has no spaces, accept key should not be calculated:
// "If either /spaces_1/ or /spaces_2/ is zero, then abort the
//  WebSocket connection.  This is a symptom of a cross-protocol
//  attack."
- (void)testHybi00RequestKeyHasNoSpaces {
	// remove all the spaces
	NSString *requestKey1 = @"3e6b26341780";
	NSString *requestKey2 = @"179G`ZD922b7X3/r90";
	NSData *requestKey3 = [@"WjN}|M(6" dataUsingEncoding:NSISOLatin1StringEncoding];

	NSData *acceptKey = [[MBWebSocketHelper sharedHelper] acceptKeyFromRequestKey1:requestKey1
																			  key2:requestKey2
																		   andKey3:requestKey3];
	GHAssertNil(acceptKey, @"Accept key should be nil when request key has no spaces");
}

// "If /key-number_1/ is not an integral multiple of /spaces_1/,
//  then abort the WebSocket connection.
//  [The same with the second key]
//  NOTE: This can only happen if the client is not a conforming
//  WebSocket client."
- (void)testHybi00RequestKeyHasWrongNumberOfSpaces {
	// add 2 spaces
	NSString *requestKey1 = @"3e 6 b263  4 17 80";
	// remove 2 spaces
	NSString *requestKey2 = @"17  9G`ZD9  2 2b 7X 3 /r90";
	NSData *requestKey3 = [@"WjN}|M(6" dataUsingEncoding:NSISOLatin1StringEncoding];

	NSData *acceptKey = [[MBWebSocketHelper sharedHelper] acceptKeyFromRequestKey1:requestKey1
																			  key2:requestKey2
																		   andKey3:requestKey3];
	GHAssertNil(acceptKey, @"Accept key should be nil when keyNumber is not an integral multiple of number of spaces");
}

// Request key 3 should be 8 bytes long. If not, accept key should be nil.
- (void)testHybi00RequestKey3HasWrongLength {
	NSString *requestKey1 = @"3e6b263  4 17 80";
	NSString *requestKey2 = @"17  9 G`ZD9   2 2b 7X 3 /r90";
	// remove 2 chars
	NSData *requestKey3 = [@"WjN}|M" dataUsingEncoding:NSISOLatin1StringEncoding];

	NSData *acceptKey = [[MBWebSocketHelper sharedHelper] acceptKeyFromRequestKey1:requestKey1
																			  key2:requestKey2
																		   andKey3:requestKey3];
	GHAssertNil(acceptKey, @"Accept key should be nil when requestKey3 is not 8 bytes length");
}

@end
