//
//  Settings.m
//  avatar
//
//  Created by Eugene Nikolsky on 7/6/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "Settings_Private.h"


static Settings *instance;


@interface Settings ()

@property (nonatomic, strong) NSUserDefaults *userDefaults;

- (void)settingsDidChange:(NSNotification *)notification;

- (void)addObserver;
- (void)removeObserver;

@end


@implementation Settings

@synthesize delegate = _delegate;
@synthesize vehicleModels = _vehicleModels;
@synthesize intIsFullscreen = _intIsFullscreen;
@synthesize intNavigationEnabled = _intNavigationEnabled;

- (void)dealloc {
	_delegate = nil;
	[self removeObserver];
}

- (void)baseInit {
	_vehicleModels = [NSArray arrayWithObjects:@"edge", @"explorer", @"escape_2013", @"f150_2013", @"fiesta", @"flex_2013", @"focus", @"fusion_2013", @"taurus_2013", nil];
	
	// default values
	_intIsFullscreen = YES;
	_intNavigationEnabled = YES;
    _intVehicleModelId = _vehicleModels[0];
	
	NSString *currAppVersion = [self appVersion];
	// set initial value of app version
	if ([_userDefaults stringForKey:kKeyLastAppVersion] == nil) {
		[_userDefaults setObject:currAppVersion forKey:kKeyLastAppVersion];
	}
	
	NSDictionary *initialDefaults = [NSDictionary dictionaryWithObjectsAndKeys:
									 [NSNumber numberWithBool:_intIsFullscreen], kKeyFullscreen,
									 [NSNumber numberWithInt:kInitLaunchCount], kKeyLaunchCount,
									 currAppVersion, kKeyLastAppVersion,
									 [NSNumber numberWithBool:_intNavigationEnabled], kKeyNavigationEnabled,
									 _intVehicleModelId, kKeyVehicleModelId,
                                     @NO, kKeyEULAAccepted,
									 nil];
	[_userDefaults registerDefaults:initialDefaults];
	
	NSString *lastAppVersion = [_userDefaults objectForKey:kKeyLastAppVersion];
	if (![currAppVersion isEqualToString:lastAppVersion]) {
		// if the app has upgraded, we reset the launch count, so that
		// welcome orientation will popup
		[_userDefaults setObject:[NSNumber numberWithInt:kInitLaunchCount]
						  forKey:kKeyLaunchCount];
		[_userDefaults setObject:currAppVersion forKey:kKeyLastAppVersion];
	}
    
    id vehicleModelIndexObj = [_userDefaults objectForKey:kKeyVehicleModelIndex];
    if (vehicleModelIndexObj != nil) {
        // the old vehicle model index exists from a previous version =>
        // convert it to newer vehicleModelId
        DDLogVerbose(@"Old Vehicle Model Index found: %@", vehicleModelIndexObj);
        int index = [vehicleModelIndexObj intValue];
        NSString *vehicleModel = [self vehicleModelByIndex:index];
        if (vehicleModel != nil) {
            DDLogVerbose(@"Setting vehicle model id: %@", vehicleModel);
            [_userDefaults setObject:vehicleModel forKey:kKeyVehicleModelId];
        }
        // remove the old setting
        [_userDefaults removeObjectForKey:kKeyVehicleModelIndex];
    }
    
	[_userDefaults synchronize];
	
	[self addObserver];
}

- (NSString *)vehicleModelByIndex:(NSUInteger)index {
    // returns the vehicle model id by it's index. should take into account
    // changes in vehicle model indexes on app upgrade.
    NSUInteger i = index;
    // in the latest version, Fiesta is added after f150
    NSUInteger f150Index = [self.vehicleModels indexOfObject:@"f150_2013"];
    if (f150Index != NSNotFound) {
        if (i > f150Index) {
            ++i;
        }
    }
    
    NSString *ret = nil;
    if (i < self.vehicleModels.count) {
        ret = self.vehicleModels[i];
    }
    return ret;
}

- (id)initWithUserDefaults:(NSUserDefaults *)userDefaults {
	if (self = [super init]) {
		_userDefaults = userDefaults;
	}
	return self;
}

- (id)init {
	if (self = [self initWithUserDefaults:
				[NSUserDefaults standardUserDefaults]]) {
		[self baseInit];
	}
	return self;
}

+ (void)initialize {
	static BOOL initialized = NO;
	if (!initialized) {
		initialized = YES;
		instance = [[Settings alloc] init];
	}
}

+ (Settings *)sharedSettings {
	return instance;
}

#pragma mark - Privates

- (void)addObserver {
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(settingsDidChange:)
												 name:NSUserDefaultsDidChangeNotification
											   object:nil];
}

- (void)removeObserver {
	[[NSNotificationCenter defaultCenter] removeObserver:self
													name:NSUserDefaultsDidChangeNotification
												  object:nil];
}

- (NSString *)appVersion {
	return [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
}

#pragma mark - Methods

- (void)increaseLaunchCount {
	int launchCount = [[self.userDefaults objectForKey:kKeyLaunchCount] intValue];
	++launchCount;
	[self.userDefaults setObject:[NSNumber numberWithInt:launchCount]
						  forKey:kKeyLaunchCount];
	[self.userDefaults synchronize];
}

#ifdef ENABLE_TESTING_FEATURES
- (void)resetLaunchCount {
	[self removeObserver];

	[self.userDefaults setObject:[NSNumber numberWithInt:kInitLaunchCount]
						  forKey:kKeyLaunchCount];
	[self.userDefaults synchronize];

	[self addObserver];
}
#endif

#pragma mark - Getters & Setters

- (BOOL)isFirstLaunch {
	int launchCount = [[self.userDefaults objectForKey:kKeyLaunchCount] intValue];
	return launchCount <= 1;
}

- (BOOL)isFullscreen {
	return [[self.userDefaults objectForKey:kKeyFullscreen] boolValue];
}

- (void)setIsFullscreen:(BOOL)isFullscreen {
    self.intIsFullscreen = isFullscreen;
    [self.userDefaults setObject:@(isFullscreen) forKey:kKeyFullscreen];
    [self.userDefaults synchronize];
}

- (BOOL)navigationEnabled {
	return [[self.userDefaults objectForKey:kKeyNavigationEnabled] boolValue];
}

- (void)setNavigationEnabled:(BOOL)navigationEnabled {
	self.intNavigationEnabled = navigationEnabled;
	[self.userDefaults setObject:[NSNumber
								  numberWithBool:self.intNavigationEnabled]
						  forKey:kKeyNavigationEnabled];
	[self.userDefaults synchronize];
}

- (NSString *)vehicleModel {
	return [self.userDefaults objectForKey:kKeyVehicleModelId];
}

- (void)setVehicleModel:(NSString *)vehicleModel {
    self.intVehicleModelId = [vehicleModel copy];
    if ([self.vehicleModels indexOfObject:self.intVehicleModelId] == NSNotFound) {
        DDLogError(@"setVehicleModel: unknown model %@", vehicleModel);
		self.intVehicleModelId = self.vehicleModels[0];
	}
	[self.userDefaults setObject:self.intVehicleModelId
						  forKey:kKeyVehicleModelId];
	[self.userDefaults synchronize];
}

- (BOOL)eulaAccepted {
    return [self.userDefaults boolForKey:kKeyEULAAccepted];
}

- (void)setEulaAccepted:(BOOL)eulaAccepted {
    [self.userDefaults setBool:eulaAccepted forKey:kKeyEULAAccepted];
    [self.userDefaults synchronize];
}

#pragma mark - Notifications

- (void)settingsDidChange:(NSNotification *)notification {
#pragma unused (notification)
	DDLogInfo(@"settingsDidChange");
	
	// check which properties have changed
	if (self.intIsFullscreen != self.isFullscreen) {
		DDLogInfo(@"fullscreen changed: %d -> %d", self.intIsFullscreen, self.isFullscreen);
		self.intIsFullscreen = self.isFullscreen;
		
		if ([self.delegate respondsToSelector:@selector(settings:fullscreenDidChange:)]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				[self.delegate settings:self fullscreenDidChange:self.intIsFullscreen];
			});
		}
	}
	
	if (self.intNavigationEnabled != self.navigationEnabled) {
		DDLogInfo(@"navigationEnabled changed: %d -> %d", self.intNavigationEnabled, self.navigationEnabled);
		self.intNavigationEnabled = self.navigationEnabled;
		
		if ([self.delegate respondsToSelector:@selector(settings:navigationEnabledDidChange:)]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				[self.delegate settings:self navigationEnabledDidChange:self.intNavigationEnabled];
			});
		}
	}
	
	if (![self.intVehicleModelId isEqualToString:self.vehicleModel]) {
		DDLogInfo(@"vehicleModelIndex changed: %@ -> %@", self.intVehicleModelId, self.vehicleModel);
		self.intVehicleModelId = self.vehicleModel;
		
		if ([self.delegate respondsToSelector:@selector(settings:vehicleModelDidChange:)]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				[self.delegate settings:self vehicleModelDidChange:self.vehicleModel];
			});
		}
	}
}

@end
