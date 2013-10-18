//
//  SettingsTest.m
//  avatar
//
//  Created by Eugene Nikolsky on 2013-05-08.
//
//

#import <GHUnit.h>
#import <OCMock.h>

#import "Settings_Private.h"

@interface SettingsTest : GHAsyncTestCase

@end


@implementation SettingsTest

- (void)testSingleton {
	Settings *settings1 = [Settings sharedSettings];
	GHAssertNotNil(settings1, @"Settings singleton must not be nil");

	Settings *settings2 = [Settings sharedSettings];
	GHAssertNotNil(settings2, @"Settings singleton must not be nil");
	
	GHAssertEquals(settings1, settings2,
				   @"The Settings singleton must be the same object");
}

#pragma mark - Reset Launch Count On Version Change Tests

// The Settings object should reset the launch count (so the user would see
// the welcome orientation screen) when the app has upgraded.
- (void)testResetLaunchCountOnVersionUpgrade {
	[self resetLaunchCountOnChangeFromVersion:@"1.0" toVersion:@"1.1"];
}

// The Settings object should not reset the launch count when the app version
// hasn't changed since last launch.
- (void)testNotResetLaunchCountOnSameVersion {
	static NSString *appVersion = @"1.0";
	
	// return mocked, "saved" old version
	id userDefaultsMock = [OCMockObject mockForClass:[NSUserDefaults class]];
	[[userDefaultsMock stub] registerDefaults:OCMOCK_ANY];
    [[[userDefaultsMock stub] objectForKey:kKeyVehicleModelIndex] andReturn:nil];
	[[userDefaultsMock stub] synchronize];
	[[[userDefaultsMock expect] andReturn:appVersion]
	 stringForKey:kKeyLastAppVersion];
	
	Settings *settings = [[Settings alloc]
						  initWithUserDefaults:userDefaultsMock];
	
	// return mocked new version
	id settingsMock = [OCMockObject partialMockForObject:settings];
	[[[settingsMock stub] andReturn:appVersion] appVersion];
	
	// mock not first launch
	[[[userDefaultsMock expect] andReturn:@3] objectForKey:kKeyLaunchCount];
	// dangerous, the object is not fully initialized yet!
	GHAssertFalse(settings.isFirstLaunch, @"Should not be first launch");
	
	[[[userDefaultsMock expect] andReturn:appVersion]
	 objectForKey:kKeyLastAppVersion];
	
	// -baseInit should not reset the launch count
	[settings baseInit];
	
	[[[userDefaultsMock expect] andReturn:@3] objectForKey:kKeyLaunchCount];
	GHAssertFalse(settings.isFirstLaunch, @"Should not be first launch still");
	
	[userDefaultsMock verify];
}

// Currently, the Settings object should reset the launch count (so the user
// would see the welcome orientation screen) when the app has downgraded (does
// it ever happen?).
- (void)testResetLaunchCountOnVersionDowngrade {
	[self resetLaunchCountOnChangeFromVersion:@"1.1" toVersion:@"1.0"];
}

// The Settings object should reset the launch count (so the user would see
// the welcome orientation screen) when the app has upgraded to a new major
// version (say, "1.0" => "2.0).
- (void)testResetLaunchCountOnMajorVersionUpgrade {
	[self resetLaunchCountOnChangeFromVersion:@"1.0" toVersion:@"2.0"];
}

// Currently, the Settings object should reset the launch count (so the user
// would see the welcome orientation screen) when the app has downgraded to an
// old major version (does it ever happen?).
- (void)testResetLaunchCountOnMajorVersionDowngrade {
	[self resetLaunchCountOnChangeFromVersion:@"2.0" toVersion:@"1.0"];
}

#pragma mark - Properties Tests

// Tests that the navigationEnabled property does work with NSUserDefaults –
// sets and gets the value properly.
- (void)testNavigationEnabledProperty {
	id userDefaultsMock = [OCMockObject niceMockForClass:[NSUserDefaults class]];
	Settings *settings = [[Settings alloc] initWithUserDefaults:userDefaultsMock];
	[settings baseInit];
	
	const BOOL initValue = YES;
	[[[userDefaultsMock expect] andReturn:@(initValue)]
	 objectForKey:kKeyNavigationEnabled];
	BOOL navEnabled = settings.navigationEnabled;
	GHAssertEquals(navEnabled, initValue, @"Wrong value of property");

	navEnabled = !navEnabled;
	[[userDefaultsMock expect] setObject:@(navEnabled)
								  forKey:kKeyNavigationEnabled];
	settings.navigationEnabled = navEnabled;
	
	[userDefaultsMock verify];
}

// Tests that the vehicleModel property does work with NSUserDefaults –
// sets and gets the value properly.
- (void)testVehicleModelProperty {
	id userDefaultsMock = [OCMockObject niceMockForClass:[NSUserDefaults class]];
	Settings *settings = [[Settings alloc] initWithUserDefaults:userDefaultsMock];
	[settings baseInit];
	
	NSString *initValue = @"focus";
	[[[userDefaultsMock expect] andReturn:initValue]
     objectForKey:kKeyVehicleModelId];
	NSString *model = settings.vehicleModel;
	GHAssertEqualObjects(model, initValue, @"Wrong value of property");
	
	model = @"edge";
	[[userDefaultsMock expect] setObject:model
								  forKey:kKeyVehicleModelId];
	settings.vehicleModel = model;
	
	[userDefaultsMock verify];
}

// isFirstLaunch should be YES when launch count is 0 (should never happen)
- (void)testIsFirstLaunchPropertyWithLaunchCount0 {
	[self checkIsFirstLaunchProperty:YES withLaunchCount:0];
}

// isFirstLaunch should be YES when launch count is 1 (first start)
- (void)testIsFirstLaunchPropertyWithLaunchCount1 {
	[self checkIsFirstLaunchProperty:YES withLaunchCount:1];
}

// isFirstLaunch should be YES when launch count is >= 1
- (void)testIsFirstLaunchPropertyWithLaunchCount2 {
	[self checkIsFirstLaunchProperty:NO withLaunchCount:2];
}

// Tests that the isFullscreen property is backed by NSUserDefaults.
- (void)testIsFullscreenProperty {
	id userDefaultsMock = [OCMockObject niceMockForClass:[NSUserDefaults class]];
	Settings *settings = [[Settings alloc] initWithUserDefaults:userDefaultsMock];
	[settings baseInit];
	
	[[[userDefaultsMock expect] andReturn:@(YES)]
	 objectForKey:kKeyFullscreen];
	BOOL isFullscreen = settings.isFullscreen;
	GHAssertEquals(isFullscreen, YES, @"Wrong value of property");
	
	[userDefaultsMock verify];
}

#pragma mark - Delegate Tests

// Change in isFullscreen user defaults value should trigger delegate's
// notification method.
- (void)testFullscreenDidChange {
	[self prepare];
	
	id userDefaultsMock = [OCMockObject niceMockForClass:[NSUserDefaults class]];
	
	Settings *settings = [[Settings alloc] initWithUserDefaults:userDefaultsMock];
	[settings baseInit];
	id settingsMock = [OCMockObject partialMockForObject:settings];
	
	id delegateMock = [OCMockObject mockForProtocol:@protocol(SettingsDelegate)];
	settings.delegate = delegateMock;
	[[[delegateMock expect] andDo:^(NSInvocation *inv) {
#pragma unused (inv)
		GHAssertEquals(dispatch_get_current_queue(), dispatch_get_main_queue(),
					   @"Method must be called on main queue");
		
		[self notify:kGHUnitWaitStatusSuccess];
	}] settings:settings fullscreenDidChange:YES];

	// return the same values for other properties not to trigger their change
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] navigationEnabled];
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] intNavigationEnabled];
	[[[settingsMock stub] andReturn:@"edge"] vehicleModel];
	[[[settingsMock stub] andReturn:@"edge"] intVehicleModelId];
	
	settings.intIsFullscreen = NO;
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] isFullscreen];
	[[NSNotificationCenter defaultCenter] postNotificationName:NSUserDefaultsDidChangeNotification
														object:nil];
	
	[self waitForStatus:kGHUnitWaitStatusSuccess timeout:1.0f];
	
	[delegateMock verify];
}

// Change in navigationEnabled user defaults value should trigger delegate's
// notification method.
- (void)testNavigationEnabledDidChange {
	[self prepare];
	
	id userDefaultsMock = [OCMockObject niceMockForClass:[NSUserDefaults class]];
	
	Settings *settings = [[Settings alloc] initWithUserDefaults:userDefaultsMock];
	[settings baseInit];
	id settingsMock = [OCMockObject partialMockForObject:settings];
	
	id delegateMock = [OCMockObject mockForProtocol:@protocol(SettingsDelegate)];
	settings.delegate = delegateMock;
	[[[delegateMock expect] andDo:^(NSInvocation *inv) {
#pragma unused (inv)
		GHAssertEquals(dispatch_get_current_queue(), dispatch_get_main_queue(),
					   @"Method must be called on main queue");
		
		[self notify:kGHUnitWaitStatusSuccess];
	}] settings:settings navigationEnabledDidChange:YES];
	
	// return the same values for other properties not to trigger their change
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] isFullscreen];
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] intIsFullscreen];
	[[[settingsMock stub] andReturn:@"edge"] vehicleModel];
	[[[settingsMock stub] andReturn:@"edge"] intVehicleModelId];
	
	settings.intNavigationEnabled = NO;
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] navigationEnabled];
	[[NSNotificationCenter defaultCenter] postNotificationName:NSUserDefaultsDidChangeNotification
														object:nil];
	
	[self waitForStatus:kGHUnitWaitStatusSuccess timeout:1.0f];
	
	[delegateMock verify];
}

// Change in vehicleModelId user defaults value should trigger delegate's
// notification method.
- (void)testVehicleModelDidChange {
	[self prepare];
	
	NSString *newModel = @"focus";
	NSString *oldModel = @"edge";
	
	id userDefaultsMock = [OCMockObject niceMockForClass:[NSUserDefaults class]];
	
	Settings *settings = [[Settings alloc] initWithUserDefaults:userDefaultsMock];
	[settings baseInit];
	id settingsMock = [OCMockObject partialMockForObject:settings];
	
	id delegateMock = [OCMockObject mockForProtocol:@protocol(SettingsDelegate)];
	settings.delegate = delegateMock;
	[[[delegateMock expect] andDo:^(NSInvocation *inv) {
#pragma unused (inv)
		GHAssertEquals(dispatch_get_current_queue(), dispatch_get_main_queue(),
					   @"Method must be called on main queue");
		
		[self notify:kGHUnitWaitStatusSuccess];
	}] settings:settings vehicleModelDidChange:newModel];
	
	// return the same values for other properties not to trigger their change
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] isFullscreen];
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] intIsFullscreen];
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] navigationEnabled];
	[[[settingsMock stub] andReturnValue:OCMOCK_VALUE((BOOL){YES})] intNavigationEnabled];
	
	settings.intVehicleModelId = oldModel;
	int newModelIndex = [settings.vehicleModels indexOfObject:newModel];
	GHAssertNotEquals(newModelIndex, NSNotFound, @"Incorrect vehicle model");
	[[[settingsMock stub] andReturn:newModel] vehicleModel];
	[[NSNotificationCenter defaultCenter] postNotificationName:NSUserDefaultsDidChangeNotification
														object:nil];
	
	[self waitForStatus:kGHUnitWaitStatusSuccess timeout:1.0f];
	
	[delegateMock verify];
}

#pragma mark - Private Methods

- (void)resetLaunchCountOnChangeFromVersion:(NSString *)oldAppVersion
								  toVersion:(NSString *)newAppVersion {
	// return mocked, "saved" old version
	id userDefaultsMock = [OCMockObject mockForClass:[NSUserDefaults class]];
	[[userDefaultsMock stub] registerDefaults:OCMOCK_ANY];
    [[[userDefaultsMock stub] objectForKey:kKeyVehicleModelIndex] andReturn:nil];
	[[userDefaultsMock stub] synchronize];
	[[[userDefaultsMock expect] andReturn:oldAppVersion]
	 stringForKey:kKeyLastAppVersion];
	
	Settings *settings = [[Settings alloc]
						  initWithUserDefaults:userDefaultsMock];
	
	// return mocked new version
	id settingsMock = [OCMockObject partialMockForObject:settings];
	[[[settingsMock stub] andReturn:newAppVersion] appVersion];
	
	// mock not first launch
	[[[userDefaultsMock expect] andReturn:@3] objectForKey:kKeyLaunchCount];
	// dangerous, the object is not fully initialized yet!
	GHAssertFalse(settings.isFirstLaunch, @"Should not be first launch");
	
	// -baseInit should reset the launch count and set new app version
	// we anticipate this!
	[[[userDefaultsMock expect] andReturn:oldAppVersion]
	 objectForKey:kKeyLastAppVersion];
	[[userDefaultsMock expect] setObject:@(kInitLaunchCount)
								  forKey:kKeyLaunchCount];
	[[userDefaultsMock expect] setObject:newAppVersion
								  forKey:kKeyLastAppVersion];
	[settings baseInit];
	
	[[[userDefaultsMock expect] andReturn:@(kInitLaunchCount)]
	 objectForKey:kKeyLaunchCount];
	GHAssertTrue(settings.isFirstLaunch, @"Should be first launch");
	
	[userDefaultsMock verify];
}

// Tests that the isFirstLaunch property is backed by NSUserDefaults.
- (void)checkIsFirstLaunchProperty:(BOOL)isFirstLaunch
				   withLaunchCount:(NSInteger)launchCount {
	id userDefaultsMock = [OCMockObject niceMockForClass:[NSUserDefaults class]];
	Settings *settings = [[Settings alloc] initWithUserDefaults:userDefaultsMock];
	[settings baseInit];
	
	[[[userDefaultsMock expect] andReturn:@(launchCount)]
	 objectForKey:kKeyLaunchCount];
	BOOL ret = settings.isFirstLaunch;
	GHAssertEquals(ret, isFirstLaunch,
				   @"isFirstLaunch should be %d when launch count == %i",
				   isFirstLaunch, launchCount);
	
	[userDefaultsMock verify];
}

@end
