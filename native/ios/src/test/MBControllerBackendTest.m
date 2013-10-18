//
//  MBControllerBackendTest.m
//  avatar
//
//  Created by Eugene Nikolsky on 4/25/13.
//  Copyright (c) 2013 Luxoft. All rights reserved.
//

#import <GHUnitIOS/GHUnit.h>
#import <OCMock.h>
#import "MBControllerBackend.h"
#import "MBController_Private.h"
#import "MBConst.h"


@interface DummyBackendDelegate : NSObject <MBControllerBackendDelegate>

@end


// The test case tests the MBControllerBackend interface.
@interface MBControllerBackendTest : GHAsyncTestCase

@property (nonatomic, strong) MBControllerBackend *controller;
@property (nonatomic, strong) id controllerMock;

@end


@implementation MBControllerBackendTest

- (void)setUpClass {
	self.controller = [[MBControllerBackend alloc]
					   initWithHost:@"" port:0 andName:@""];
}

- (void)tearDownClass {
	self.controller = nil;
}

- (void)setUp {
	self.controllerMock = [OCMockObject partialMockForObject:self.controller];
}

- (void)tearDown {
	[self.controllerMock verify];
	self.controllerMock = nil;
}

#pragma mark - Notifications tests

// -onFullscreenChanged: should send a certain notification.
- (void)testOnFullscreenChanged {
	static NSString *kNotificationName = @"Backend.onFullScreenChanged";
	static NSString *kParamIsFullscreen = @"isFullScreen";
	
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		NSDictionary *correct = @{kJsonRpcKey: kJsonRpcVersion,
							kMethodKey: kNotificationName,
							kParametersKey: @{kParamIsFullscreen: @1}};
		GHAssertEqualObjects(msg, correct, @"Wrong notification");
		
		return YES;
	}]];
	[self.controller onFullscreenChanged:YES];
	
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		NSDictionary *correct = @{kJsonRpcKey: kJsonRpcVersion,
							kMethodKey: kNotificationName,
							kParametersKey: @{kParamIsFullscreen: @0}};
		GHAssertEqualObjects(msg, correct, @"Wrong notification");
		
		return YES;
	}]];
	[self.controller onFullscreenChanged:NO];
}

// -onNavigationEnabledChanged: should send a certain notification.
- (void)testOnNavigationEnabledChanged {
	static NSString *kNotificationName = @"Backend.onHasMapsChanged";
	static NSString *kParamIsNavEnabled = @"hasMaps";
	
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		NSDictionary *correct = @{kJsonRpcKey: kJsonRpcVersion,
							kMethodKey: kNotificationName,
							kParametersKey: @{kParamIsNavEnabled: @1}};
		GHAssertEqualObjects(msg, correct, @"Wrong notification");
		
		return YES;
	}]];
	[self.controller onNavigationEnabledChanged:YES];
	
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		NSDictionary *correct = @{kJsonRpcKey: kJsonRpcVersion,
							kMethodKey: kNotificationName,
							kParametersKey: @{kParamIsNavEnabled: @0}};
		GHAssertEqualObjects(msg, correct, @"Wrong notification");
		
		return YES;
	}]];
	[self.controller onNavigationEnabledChanged:NO];
}

// -onVehicleModelChanged: should send a certain notification.
- (void)testOnVehicleModelChanged {
	static NSString *kNotificationName = @"Backend.onVehicleChanged";
	static NSString *kParamVehicleModel = @"vehicle";
	static NSString *kVehicleModel = @"fusion";
	
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		NSDictionary *correct = @{kJsonRpcKey: kJsonRpcVersion,
							kMethodKey: kNotificationName,
							kParametersKey: @{kParamVehicleModel: kVehicleModel}};
		GHAssertEqualObjects(msg, correct, @"Wrong notification");
		
		return YES;
	}]];
	[self.controller onVehicleModelChanged:kVehicleModel];
}

// -onAppStatusChanged: should send a certain notification.
- (void)testOnAppStatusChanged {
	static NSString *kNotificationName = @"Backend.onAppStatusChanged";
	static NSString *kParamAppStatusChanged = @"appBecameActive";
	
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		NSDictionary *correct = @{kJsonRpcKey: kJsonRpcVersion,
							kMethodKey: kNotificationName,
							kParametersKey: @{kParamAppStatusChanged: @1}};
		GHAssertEqualObjects(msg, correct, @"Wrong notification");
		
		return YES;
	}]];
	[self.controller onAppStatusChanged:YES];
	
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *msg = (NSDictionary *)obj;
		GHAssertNotNil(msg, @"Message should not be nil");
		
		NSDictionary *correct = @{kJsonRpcKey: kJsonRpcVersion,
							kMethodKey: kNotificationName,
							kParametersKey: @{kParamAppStatusChanged: @0}};
		GHAssertEqualObjects(msg, correct, @"Wrong notification");
		
		return YES;
	}]];
	[self.controller onAppStatusChanged:NO];
}

#pragma mark - Responses Tests
/*
 * The tests in this group verify that the responses from the -processRequest:
 * method conform to the Backend.json specification.
 */

- (void)testProcessRequestIsFirstStart {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
					   kMethodKey: @"Backend.isFirstStart"};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSDictionary *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain result");
		id isFirstStart = [result objectForKey:@"isFirstStart"];
		GHAssertNotNil(isFirstStart, @"Result should contain isFirstStart key");
		GHAssertTrue([isFirstStart isKindOfClass:[NSNumber class]],
					 @"isFirstStart should be a boolean");
		GHAssertEquals(result.count, 1u, @"Response should contain 1 pair");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestIsFullScreen {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.isFullScreen"};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSDictionary *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain result");
		id isFullScreen = [result objectForKey:@"isFullScreen"];
		GHAssertNotNil(isFullScreen, @"Result should contain isFullScreen key");
		GHAssertTrue([isFullScreen isKindOfClass:[NSNumber class]],
					 @"isFullScreen should be a boolean");
		GHAssertEquals(result.count, 1u, @"Response should contain 1 pair");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestHasMaps {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.hasMaps"};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSDictionary *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain result");
		id hasMaps = [result objectForKey:@"hasMaps"];
		GHAssertNotNil(hasMaps, @"Result should contain hasMaps key");
		GHAssertTrue([hasMaps isKindOfClass:[NSNumber class]],
					 @"hasMaps should be a boolean");
		GHAssertEquals(result.count, 1u, @"Response should contain 1 pair");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestGetWindowSize {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.getWindowSize"};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSDictionary *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain result");
		
		id width = [result objectForKey:@"width"];
		GHAssertNotNil(width, @"Result should contain width key");
		GHAssertTrue([width isKindOfClass:[NSNumber class]],
					 @"width should be a number");
		
		id height = [result objectForKey:@"height"];
		GHAssertNotNil(height, @"Result should contain height key");
		GHAssertTrue([height isKindOfClass:[NSNumber class]],
					 @"height should be a number");
		
		GHAssertEquals(result.count, 2u, @"Response should contain 2 pairs");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestGetWindowDensity {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.getWindowDensity"};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSDictionary *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain result");
		id windowDensity = [result objectForKey:@"windowDensity"];
		GHAssertNotNil(windowDensity, @"Result should contain windowDensity key");
		GHAssertTrue([windowDensity isKindOfClass:[NSNumber class]],
					 @"isFullScreen should be a floating number");
		GHAssertEquals(result.count, 1u, @"Response should contain 1 pair");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestGetVehicleModel {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.getVehicleModel"};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSDictionary *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain result");
		id vehicle = [result objectForKey:@"vehicle"];
		GHAssertNotNil(vehicle, @"Result should contain vehicle key");
		GHAssertTrue([vehicle isKindOfClass:[NSString class]],
					 @"vehicle should be a string");
		GHAssertEquals(result.count, 1u, @"Response should contain 1 pair");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestGetOSInfo {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.getOSInfo"};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSDictionary *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain result");
		
		id osType = [result objectForKey:@"osType"];
		GHAssertNotNil(osType, @"Result should contain osType key");
		GHAssertTrue([osType isKindOfClass:[NSString class]],
					 @"osType should be a string");
		
		id osVersion = [result objectForKey:@"osVersion"];
		GHAssertNotNil(osVersion, @"Result should contain osVersion key");
		GHAssertTrue([osVersion isKindOfClass:[NSString class]],
					 @"osVersion should be a string");
		
		id isNativeApplication = [result objectForKey:@"isNativeApplication"];
		GHAssertNotNil(isNativeApplication,
					   @"Result should contain isNativeApplication key");
		GHAssertTrue([isNativeApplication isKindOfClass:[NSNumber class]],
					 @"isNativeApplication should be a boolean");
		
		GHAssertEquals(result.count, 3u, @"Response should contain 3 pairs");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestLogToOS {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.logToOS", kParametersKey:@{
								   @"message": @"message to log"}};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSString *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain a string result");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestSetHasMaps {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setHasMaps", kParametersKey:@{
								   @"hasMaps": @YES}};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSString *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain a string result");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestSetVehicleModel {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setVehicleModel", kParametersKey:@{
								   @"vehicle": @"focus"}};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSString *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain a string result");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestSetHasMapsNoParameters {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setHasMaps"};
	[self checkResponseToWrongParamsInRequest:request];
}

- (void)testProcessRequestSetHasMapsWrongParameters {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setHasMaps", kParametersKey:@YES};
	[self checkResponseToWrongParamsInRequest:request];
}

- (void)testProcessRequestSetHasMapsWrongTypeParameters {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setHasMaps", kParametersKey:@{
								   @"hasMaps": @[@YES]}};
	[self checkResponseToWrongParamsInRequest:request];
}

- (void)testProcessRequestSetVehicleModelNoParameters {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setVehicleModel"};
	[self checkResponseToWrongParamsInRequest:request];
}

- (void)testProcessRequestSetVehicleModelWrongParameters {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setVehicleModel",
						   kParametersKey:@"focus"};
	[self checkResponseToWrongParamsInRequest:request];
}

- (void)testProcessRequestSetVehicleModelWrongTypeParameters {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setVehicleModel",
						   kParametersKey:@{@"vehicle": @[@"focus"]}};
	[self checkResponseToWrongParamsInRequest:request];
}

- (void)testProcessOpenEULAWithoutDelegate {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.openEULA"};
    [self checkResponseWithoutDelegateToRequest:request];
}

- (void)testProcessOpenEULA {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.openEULA"};
    __block id<MBControllerBackendDelegate> delegate = [DummyBackendDelegate new];
    self.controller.delegate = delegate;
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSString *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain a string result");
		
        self.controller.delegate = delegate = nil;
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessSendSupportEmailWithoutDelegate {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.sendSupportEmail"};
    [self checkResponseWithoutDelegateToRequest:request];
}

- (void)testProcessSendSupportEmail {
    [self prepare:@selector(testProcessSendSupportEmail)];
    
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.sendSupportEmail"};
    __block id<MBControllerBackendDelegate> delegate = [DummyBackendDelegate new];
    self.controller.delegate = delegate;
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSString *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain a string result");
		
        self.controller.delegate = delegate = nil;
        
        [self notify:kGHUnitWaitStatusSuccess
         forSelector:@selector(testProcessSendSupportEmail)];
        
		return YES;
	}]];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [self.controller processRequest:request];
    });
    
    [self waitForStatus:kGHUnitWaitStatusSuccess timeout:1.0];
}

- (void)testProcessRequestSetFullScreen {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setFullScreen",
                           kParametersKey:@{@"isFullScreen": @YES}};
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		NSString *result = [response objectForKey:kResultKey];
		GHAssertNotNil(result, @"Response should contain a string result");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)testProcessRequestSetFullScreenNoParameters {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setFullScreen"};
	[self checkResponseToWrongParamsInRequest:request];
}

- (void)testProcessRequestSetFullScreenWrongParameters {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setFullScreen",
                           kParametersKey:@YES};
	[self checkResponseToWrongParamsInRequest:request];
}

- (void)testProcessRequestSetFullScreenWrongTypeParameters {
	NSDictionary *request = @{kJsonRpcKey: kJsonRpcVersion, kMessageIdKey: @42,
						   kMethodKey: @"Backend.setFullScreen",
                           kParametersKey:@{@"isFullScreen": @[@YES]}};
	[self checkResponseToWrongParamsInRequest:request];
}

#pragma mark - Processing Notifications Tests

// -processNotification: should call the
// -[MBControllerBackendDelegate webAppDidLoad] method on the main dispatch
// queue and unsubscribe from the onAppLoaded notification when it's received
- (void)testProcessNotificationOnAppLoaded {
	static NSString *kNotificationName = @"Backend.onAppLoaded";
	
	// prepare for async testing
	[self prepare:_cmd];
	
	// we expect the unsubscribeFromProperty: to be called
	[[self.controllerMock expect] unsubscribeFromProperty:kNotificationName];
	
	// mock the delegate, which should notify the test of success when the
	// webAppDidLoad is called
	id delegateMock = [OCMockObject mockForProtocol:
					   @protocol(MBControllerBackendDelegate)];
	self.controller.delegate = delegateMock;
	[[[delegateMock expect] andDo:^(NSInvocation *obj) {
#pragma unused (obj)
		GHAssertEquals(dispatch_get_current_queue(), dispatch_get_main_queue(),
					   @"webAppDidLoad should be called on the main dispatch queue");
		
		[self notify:kGHUnitWaitStatusSuccess
		 forSelector:@selector(testProcessNotificationOnAppLoaded)];
	}] webAppDidLoad];
	
	// send the notification from another queue to make sure the delegate
	// is called on the main queue
	dispatch_sync(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
		NSDictionary *msg = @{kJsonRpcKey: kJsonRpcVersion,
						   kMethodKey: kNotificationName};
		[self.controller processNotification:msg];
	});
	
	// wait for success, or else…
	[self waitForStatus:kGHUnitWaitStatusSuccess timeout:1.0f];
	
	// cleanup
	self.controller.delegate = nil;
	
	// verify the mock
	[delegateMock verify];
}

#pragma mark - Private methods

- (void)checkResponseToWrongParamsInRequest:(NSDictionary *)request {
	[[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
		NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		GHAssertNil([response objectForKey:kResultKey],
					@"Result must not be present due to an error");
		
		NSDictionary *error = [response objectForKey:kErrorKey];
		GHAssertNotNil(error, @"Error should be present");
		
		NSNumber *errorCode = [error objectForKey:kErrorCodeKey];
		GHAssertEqualObjects(errorCode, @(kMBInvalidParams),
								@"Error code should be InvalidParams");
		
		NSString *errorMessage = [error objectForKey:kErrorMessageKey];
		GHAssertGreaterThan(errorMessage.length, 0u,
							@"Error message should not be empty");
		
		return YES;
	}]];
	[self.controller processRequest:request];
}

- (void)checkResponseWithoutDelegateToRequest:(NSDictionary *)request {
    self.controller.delegate = nil;
    [[self.controllerMock expect] sendJsonMessage:[OCMArg checkWithBlock:^BOOL(id obj) {
        NSDictionary *response = (NSDictionary *)obj;
		GHAssertNotNil(response, @"Response should not be nil");
		
		GHAssertNil([response objectForKey:kResultKey],
					@"Result must not be present due to an error");
		
		NSDictionary *error = [response objectForKey:kErrorKey];
		GHAssertNotNil(error, @"Error should be present");
		
		NSNumber *errorCode = [error objectForKey:kErrorCodeKey];
		GHAssertEqualObjects(errorCode, @(kMBRequestNotHandled),
                             @"Error code should be RequestNotHandled");
		
		NSString *errorMessage = [error objectForKey:kErrorMessageKey];
		GHAssertGreaterThan(errorMessage.length, 0u,
							@"Error message should not be empty");
        return YES;
    }]];
    [self.controllerMock processRequest:request];
}

@end


@implementation DummyBackendDelegate

- (void)webAppDidLoad {
}

- (void)setIsNavigationEnabled:(BOOL)isNavigationEnabled {
#pragma unused (isNavigationEnabled)
}

- (void)setVehicleModel:(NSString *)vehicleModel {
#pragma unused (vehicleModel)
}

- (void)openEULA {
}

- (BOOL)sendSupportEmailWithParameters:(NSDictionary *)params {
#pragma unused (params)
    return YES;
}

- (void)setIsFullScreen:(BOOL)isFullScreen {
#pragma unused (isFullScreen)
}

@end
