//
//  AppDelegate.m
//  avatar
//
//  Created by Vasiliy Saenko on 10.01.12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"
#import "EULAViewController.h"

#import "DDTTYLogger.h"
#import "DDFileLogger.h"

#import "Settings.h"

#import <mach/mach.h>


@interface AppDelegate () <EULAViewControllerDelegate>

@property (nonatomic, assign) BOOL firstTimeAppActivation;

#ifdef ENABLE_TESTING_FEATURES
@property (nonatomic, strong) NSMutableArray *intMemoryUsageComments;
#endif

// Returns the memory in use, in bytes.
- (NSUInteger)memoryUsage;

// Prints the current memory usage of the app.
// The comment is added to the log if specified.
// Also, adds the result string to the memoryUsageComments
// array (if ENABLE_TESTING_FEATURES is set).
- (void)printMemoryUsageWithComment:(NSString *)comment;

@end


@implementation AppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;
@synthesize firstTimeAppActivation = _firstTimeAppActivation;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
#pragma unused (application, launchOptions)
	self.firstTimeAppActivation = YES;
	
#ifdef ENABLE_TESTING_FEATURES
	self.intMemoryUsageComments = [NSMutableArray array];
#endif
	
	[DDLog addLogger:[DDTTYLogger sharedInstance]];
	[DDTTYLogger sharedInstance].colorsEnabled = YES;
	[[DDTTYLogger sharedInstance] setForegroundColor:[UIColor yellowColor] backgroundColor:nil forTag:WebTag];
	
	DDFileLogger *fileLogger = [[DDFileLogger alloc] init];
	fileLogger.rollingFrequency = 60 * 60 * 24; // 24-hour rolling
	fileLogger.logFileManager.maximumNumberOfLogFiles = 7;
	[DDLog addLogger:fileLogger];
	
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPhone" bundle:nil];
    } else {
        self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPad" bundle:nil];
    }
    
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
    if ([Settings sharedSettings].eulaAccepted) {
        [[Settings sharedSettings] increaseLaunchCount];
        [self.viewController initController];
    } else {
        EULAViewController *eulaViewController = [[EULAViewController alloc]
                                                  initWithNibName:nil bundle:nil];
        eulaViewController.delegate = self;
        [self.viewController presentModalViewController:eulaViewController
                                               animated:NO];
    }
    
	[self printMemoryUsageWithComment:@"at startup"];
	
    return YES;
}

- (void)webAppDidLoad {
	[self printMemoryUsageWithComment:@"web loaded"];
}

- (void)eulaViewControllerDidFinish:(EULAViewController *)eulaViewController {
#pragma unused (eulaViewController)
    [Settings sharedSettings].eulaAccepted = YES;
	[[Settings sharedSettings] increaseLaunchCount];
    [self.viewController initController];
    [self.viewController dismissModalViewControllerAnimated:YES];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
	
	[self.viewController appStatusDidChange:NO];
	
	// the app doesn't have enought time to process the event above,
	// so we ask the system to run our background task for a while
	__block UIBackgroundTaskIdentifier bgTask = [application beginBackgroundTaskWithExpirationHandler:^{
		[application endBackgroundTask:bgTask];
		bgTask = UIBackgroundTaskInvalid;
	}];
	
	[self printMemoryUsageWithComment:@"backgrounding"];
	
	// two seconds should be enough
	double delayInSeconds = 2.0;
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
	dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
		DDLogInfo(@"yahoo!");
		[application endBackgroundTask:bgTask];
		bgTask = UIBackgroundTaskInvalid;
	});
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
#pragma unused (application)
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
#pragma unused (application)
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
#pragma unused (application)
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
	
	if (!self.firstTimeAppActivation) {
		double delayInSeconds = .2f;
		dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
		dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
			[self.viewController appStatusDidChange:YES];
		});
		
		[self printMemoryUsageWithComment:@"foregrounding"];
	}
	self.firstTimeAppActivation = NO;
}

- (void)applicationWillTerminate:(UIApplication *)application
{
#pragma unused (application)
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
	
	[self printMemoryUsageWithComment:@"at termination"];
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
#pragma unused (application)
	[self printMemoryUsageWithComment:@"memory warning"];
}

- (void)printMemoryUsageWithComment:(NSString *)comment {
	NSUInteger usage = [self memoryUsage];
	float megabyteUsage = usage * 1. / (1024 * 1024);
	DDLogWarn(@"## Memory usage [%@]: %d Bytes / %d KBytes / %f MBytes",
			  comment ? comment : @"",
			  usage, usage / 1024, megabyteUsage);

#ifdef ENABLE_TESTING_FEATURES
	[self.intMemoryUsageComments addObject:[NSString stringWithFormat:
											@"[%@]: %f MBytes",
											comment ? comment : @"",
											megabyteUsage]];
#endif
}

- (NSUInteger)memoryUsage {
	// http://stackoverflow.com/questions/787160/programmatically-retrieve-memory-usage-on-iphone/787535#787535
	struct task_basic_info info;
	mach_msg_type_number_t size = sizeof(info);
	kern_return_t kerr = task_info(mach_task_self(),
								   TASK_BASIC_INFO,
								   (task_info_t)&info,
								   &size);
	uint result = 0;
	
	if (kerr == KERN_SUCCESS) {
		result = info.resident_size;
	} else {
		DDLogInfo(@"Error with task_info(): %s", mach_error_string(kerr));
	}
	
	return result;
}

#ifdef ENABLE_TESTING_FEATURES
#pragma mark - Getters & Setters

- (NSArray *)memoryUsageComments {
	return [self.intMemoryUsageComments copy];
}
#endif

@end
