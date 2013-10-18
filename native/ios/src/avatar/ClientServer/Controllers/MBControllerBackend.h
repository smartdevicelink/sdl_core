//
//  MBControllerBackend.h
//  avatar
//
//  Created by Eugene Nikolsky on 7/6/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "MBController.h"

extern NSString *const kKeyEmailRecipient;
extern NSString *const kKeyEmailBody;
extern NSString *const kKeyEmailBodyInHTML;


@protocol MBControllerBackendDelegate;
@protocol MBControllerBackendQueryDelegate;

@interface MBControllerBackend : MBController

@property (nonatomic, unsafe_unretained) id<MBControllerBackendDelegate> delegate;
@property (nonatomic, unsafe_unretained) id<MBControllerBackendQueryDelegate> queryDelegate;

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port;

// Sends a notification that the fullscreen option has been changed.
- (void)onFullscreenChanged:(BOOL)isFullscreen;
// Sends a notification that the navigation enabled option has been changed.
- (void)onNavigationEnabledChanged:(BOOL)isNavigationEnabled;
// Sends a notification that the vehicle model has been changed.
- (void)onVehicleModelChanged:(NSString *)vehicleModel;
// Sends a notification when the app has become active/not active.
- (void)onAppStatusChanged:(BOOL)appDidBecomeActive;

@end



// Protocol to notify the application about events coming from the web.
@protocol MBControllerBackendDelegate <NSObject>

@required
// Called when the web part of the app is loaded.
- (void)webAppDidLoad;
// Changes isNavigationEnabled flag.
- (void)setIsNavigationEnabled:(BOOL)isNavigationEnabled;
// Changes current vehicle model.
- (void)setVehicleModel:(NSString *)vehicleModel;
// Requests to open the app's EULA.
- (void)openEULA;
// Requests to open a new email composing window with the specified parameters.
// Should return YES if opened successfully.
- (BOOL)sendSupportEmailWithParameters:(NSDictionary *)params;
// Changes the full screen mode.
- (void)setIsFullScreen:(BOOL)isFullScreen;

@end


// Protocol that used to get some information from the app.
@protocol MBControllerBackendQueryDelegate <NSObject>

@required
// Should return YES if it's the first app launch after installation.
- (BOOL)isFirstStart;
// Should return YES if the app is in fullscreen mode.
- (BOOL)isFullscreen;
// Should return YES if navigation is enabled.
- (BOOL)isNavigationEnabled;
// Should return the model of the current vehicle.
- (NSString *)vehicleModel;
// Should return the dimensions of the main webView.
- (CGSize)webViewSize;

@end
