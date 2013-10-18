//
//  Settings.h
//  avatar
//
//  Created by Eugene Nikolsky on 7/6/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol SettingsDelegate;

// Wrapper that provides access to the app's settings.
// It's a singleton.
@interface Settings : NSObject

@property (nonatomic, unsafe_unretained) id<SettingsDelegate> delegate;

@property (nonatomic, readonly) BOOL isFirstLaunch;
@property (nonatomic, assign) BOOL isFullscreen;
@property (nonatomic, assign) BOOL navigationEnabled;
@property (nonatomic, copy) NSString *vehicleModel;
@property (nonatomic, assign) BOOL eulaAccepted;

+ (Settings *)sharedSettings;

// Should be called when the app starts.
- (void)increaseLaunchCount;

#ifdef ENABLE_TESTING_FEATURES
// Resets the launch count to emulate the first launch.
// For testing.
- (void)resetLaunchCount;
#endif

@end



// The delegate that will receive notifications when fullscreen,
// navigationEnabled, or vehicleModel options has changed.
@protocol SettingsDelegate <NSObject>

@required
- (void)settings:(Settings *)settings fullscreenDidChange:(BOOL)isFullscreen;
- (void)settings:(Settings *)settings navigationEnabledDidChange:(BOOL)isNavigationEnabled;
- (void)settings:(Settings *)settings vehicleModelDidChange:(NSString *)vehicleModel;

@end
