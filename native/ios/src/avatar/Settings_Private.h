//
//  Settings_Private.h
//  avatar
//
//  Created by Eugene Nikolsky on 2013-05-10.
//
//

#import "Settings.h"

#define kKeyFullscreen			@"fullscreenEnabled"
#define kKeyLaunchCount			@"launchCount"
#define kKeyLastAppVersion		@"lastAppVersion"
#define kKeyNavigationEnabled	@"navigationEnabled"
#define kKeyVehicleModelId      @"vehicleModelId"
#define kKeyEULAAccepted        @"eulaAccepted"
// legacy option key
#define kKeyVehicleModelIndex	@"vehicleSettings"

#define kInitLaunchCount		0


// Private stuff of the Settings interface.
// Used for testing.
@interface Settings ()

// The list of supported strings for vehicle models.
// NOTE: the ids of items should be the same as in the settings bundle.
@property (nonatomic, strong) NSArray *vehicleModels;

// Internal properties
// We have to keep them in order to detect which of them have changed
// when we receive a NSUserDefaultsDidChangeNotification notification.
@property (nonatomic, assign) BOOL intIsFullscreen;
@property (nonatomic, assign) BOOL intNavigationEnabled;
@property (nonatomic, strong) NSString *intVehicleModelId;


// Sets the specified user defaults for the object. You MUST call -baseInit
// before using the object.
- (id)initWithUserDefaults:(NSUserDefaults *)userDefaults;

// Initializes the object. You MUST call this method when you use the
// -initWithUserDefaults: initializer.
- (void)baseInit;

// Returns the app version from the info plist.
- (NSString *)appVersion;

@end
