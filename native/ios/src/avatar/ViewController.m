//
//  ViewController.m
//  avatar
//
//  Created by Vasiliy Saenko on 10.01.12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <AVFoundation/AVFoundation.h>
#import <MessageUI/MessageUI.h>

#import <Reachability/Reachability.h>

#import "ViewController.h"
#import "AppDelegate.h"
#import "MBMessageBroker.h"
#import "MBServer.h"
#import "MBControllerBackend.h"
#import "MBControllerRate.h"
#import "Settings.h"
#import "EULAViewController.h"


#define IS_IPAD		([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad)
#define IS_IPHONE	(!IS_IPAD)
#define IS_BIG_IPHONE	(IS_IPHONE && ([UIScreen mainScreen].bounds.size.height == 1136/2))

// Access to the global AppDelegate instance
#define APP_DELEGATE	((AppDelegate *)[UIApplication sharedApplication].delegate)

#define kHost		@"127.0.0.1"

// Get the target's language as defined by the preprocessor literal LANG=en
// http://stackoverflow.com/questions/7605857/preprocessor-macro-value-to-objective-c-string-literal/7605905#7605905
#define STRINGIZE(x) #x
#define STRINGIZE2(x) STRINGIZE(x)
#define LANGLITERAL @ STRINGIZE2(LANG)

NSString *const WebTag = @"WebTag";

static const int kWebAppWidth = 800;
static const int kWebAppHeight = 480;


// Key for the app id in info.plist
#define KEY_APP_ID              @"AppId"
// Template URL to rate the app. Should include the %@ placeholder to place
// the actual app id.
// http://stackoverflow.com/questions/3654144/direct-rate-in-itunes-link-in-my-app
#define RATE_APP_URL_HOST      @"itunes.apple.com"
#define RATE_APP_URL_TEMPLATE  @"http://" RATE_APP_URL_HOST "/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=%@&pageNumber=0&sortOrdering=1&type=Purple+Software&mt=8"

@interface ViewController () <MBControllerBackendDelegate,
    MBControllerBackendQueryDelegate, SettingsDelegate, UIWebViewDelegate,
    EULAViewControllerDelegate, MFMailComposeViewControllerDelegate,
    MBControllerRateDelegate>

@property (nonatomic, strong) MBServer *server;
@property (nonatomic, strong) MBControllerBackend *controllerBackend;
@property (nonatomic, strong) MBControllerRate *controllerRate;

#ifdef ENABLE_TESTING_FEATURES
@property (nonatomic, strong) UILongPressGestureRecognizer *longPressRecognizer;
@property (nonatomic, strong) UISwipeGestureRecognizer *swipeRecognizer;
@property (nonatomic, assign) BOOL showLaunchTime;
@property (nonatomic, strong) NSDate *launchTime;
@property (nonatomic, strong) UIView *memoryUsageView;
@property (nonatomic, strong) UIView *versionView;
@property (nonatomic, strong) UITapGestureRecognizer *tapRecognizer;
@property (nonatomic, strong) UITapGestureRecognizer *tapVersionRecognizer;
#endif

// Updates the view's subviews' frames when the fullscreen option has changed.
- (void)updateSubviewsFramesWhenChangingFullscreen:(BOOL)isFullscreen;
// Calculates the frame of view centered in another view.
- (CGRect)frameOfViewWithSize:(CGSize)size centeredInViewWithFrame:(CGRect)frame;

// Returns the screen size in landscape orientation.
- (CGSize)landscapeScreenSize;

#ifdef ENABLE_TESTING_FEATURES
// Handles the long press gesture on splash image.
- (void)handleLongPressGesture;
// Handles the swipe gesture on splash image.
- (void)handleSwipeGesture;
// Handles the tap gesture on the top-right corner.
- (void)handleTapGesture;
// Handles the tap gesture on the top-left corner.
- (void)handleVersionTapGesture;
#endif

@end


@implementation ViewController

@synthesize imageView = _imageView;
@synthesize webView = _webView;
@synthesize indicatorLoading = _indicatorLoading;

@synthesize server = _server;
@synthesize controllerBackend = _controllerBackend;

#ifdef ENABLE_TESTING_FEATURES
@synthesize longPressRecognizer = _longPressRecognizer;
@synthesize swipeRecognizer = _swipeRecognizer;
@synthesize showLaunchTime = _showLaunchTime;
@synthesize launchTime = _launchTime;
@synthesize memoryUsageView = _memoryUsageView;
@synthesize versionView = _versionView;
@synthesize tapRecognizer = _tapRecognizer;
@synthesize tapVersionRecognizer = _tapVersionRecognizer;
#endif

- (void)dealloc {
	if ([Settings sharedSettings].delegate == self) {
		[Settings sharedSettings].delegate = nil;
	}
	
	[[MBMessageBroker sharedMessageBroker] stop];
	_controllerBackend.delegate = nil;
	_controllerBackend.queryDelegate = nil;
    _controllerRate.delegate = nil;

#ifdef ENABLE_TESTING_FEATURES
	[_imageView removeGestureRecognizer:_longPressRecognizer];
	[_imageView removeGestureRecognizer:_swipeRecognizer];
#endif
}

#pragma mark - UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView2 shouldStartLoadWithRequest:(NSURLRequest *)request  navigationType:(UIWebViewNavigationType)navigationType {
#pragma unused (webView2, navigationType)
	NSString *requestString = [request.URL.absoluteString stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	
	if ([requestString hasPrefix:@"ios-log:"]) {
		NSString* logString = [[requestString componentsSeparatedByString:@":#iOS#"] objectAtIndex:1];
		DDLogWeb(@"UIWebView console: %@", logString);
		return NO;
	}
	
	return YES;
}

#pragma mark -

- (void)hideSplash {
	DDLogWarn(@"hiding splash");
	
	[self.indicatorLoading stopAnimating];
	[UIView animateWithDuration:1.0f animations:^{
		self.imageView.alpha = 0.0f;
		self.indicatorLoading.alpha = 0.0f;
	} completion:^(BOOL finished) {
#pragma unused (finished)
		self.imageView.hidden = YES;
		self.indicatorLoading.hidden = YES;
		
#ifdef ENABLE_TESTING_FEATURES
		[self.imageView removeGestureRecognizer:self.longPressRecognizer];
		self.longPressRecognizer = nil;
		[self.imageView removeGestureRecognizer:self.swipeRecognizer];
		self.swipeRecognizer = nil;
#endif
	}];
}

// create webview and prepare to show a content
- (void)loadContent{
    self.webView.allowsInlineMediaPlayback = YES;
	self.webView.delegate = self;

	// turns off webView's bouncing
	for (UIScrollView *subview in self.webView.subviews) {
		if (subview) {
			subview.bounces = NO;
		}
	}

    // load html page
	DDLogVerbose(@"Loading page");
	NSURL *url = [[NSBundle mainBundle] URLForResource:@"index" withExtension:@"html"];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[self.webView loadRequest:request];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

- (void)appStatusDidChange:(BOOL)appDidBecomeActive {
	DDLogWarn(@"app became active: %d", appDidBecomeActive);
	static NSString *kAppStatusChangedMethod = @"FFW.Backend.onAppStatusChanged(%@);";
	
	if (!appDidBecomeActive) {
		NSString *result = [self.webView stringByEvaluatingJavaScriptFromString:[NSString stringWithFormat:kAppStatusChangedMethod, @"false"]];
		DDLogInfo(@"onAppStatusChanged result = '%@'", result);

		// when the app is moving to background, we need to unsubscribe from
		// all notifications, unregister all controllers, drop all network
		// connections, and server should stop listening (quiet time!)
		[self.controllerBackend unregisterController];
        [self.controllerRate unregisterController];
		[self.server stop];
	} else {
		// server should start listening, reconnect all controllers
		[self.server listen];
        [self.controllerBackend connect];
        [self.controllerRate connect];
		
		// the app has woken up, we have to notify the web about it
		// via javascript, as the WebSocket connection is down
		NSString *result = [self.webView stringByEvaluatingJavaScriptFromString:[NSString stringWithFormat:kAppStatusChangedMethod, @"true"]];
		DDLogInfo(@"onAppStatusChanged result = '%@'", result);
	}
}

#pragma mark - View lifecycle

#ifdef ENABLE_TESTING_FEATURES
- (NSString *)appVersion {
	NSDictionary *infoDict = [NSBundle mainBundle].infoDictionary;
	NSString *version = [NSString stringWithFormat:@"%@ (%@, %@)",
						 [infoDict objectForKey:@"CFBundleShortVersionString"],
						 [infoDict objectForKey:@"CFBundleVersion"],
						 [infoDict objectForKey:@"BuildTime"]];
	return version;
}

- (void)addVersionButton {
	CGRect viewRect = CGRectZero;
	if (IS_BIG_IPHONE) {
		CGSize screenSize = [self landscapeScreenSize];
		const CGFloat viewWidth = 15;
		viewRect = CGRectMake(0, 0, viewWidth, screenSize.height);
	} else {
		const CGFloat viewWidth = self.view.frame.size.width / 2;
		const CGFloat viewHeight = IS_IPAD ? 48 : 16;
		viewRect = CGRectMake(0, 0, viewWidth, viewHeight);
	}
	self.versionView = [[UIView alloc] initWithFrame:viewRect];
	self.versionView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleBottomMargin;
	self.versionView.backgroundColor = [UIColor clearColor];
	
	[self.view addSubview:self.versionView];
	
	self.tapVersionRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleVersionTapGesture)];
	self.tapVersionRecognizer.numberOfTapsRequired = 4;
	[self.versionView addGestureRecognizer:self.tapVersionRecognizer];
}

// Adds a view at the top-right corner (or along the right side on
// big iPhone screen), by interacting with which a tester can find
// out the memory usage at certain checkpoints.
- (void)addMemoryUsageFeature {
	CGRect viewRect = CGRectZero;
	if (IS_BIG_IPHONE) {
		CGSize screenSize = [self landscapeScreenSize];
		const CGFloat viewWidth = 15;
		viewRect = CGRectMake(screenSize.width - viewWidth, 0, viewWidth, screenSize.height);
	} else {
		const CGFloat viewWidth = self.view.frame.size.width / 2;
		const CGFloat viewHeight = IS_IPAD ? 48 : 16;
		viewRect = CGRectMake(viewWidth, 0, viewWidth, viewHeight);
	}
	self.memoryUsageView = [[UIView alloc] initWithFrame:viewRect];
	self.memoryUsageView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleBottomMargin;
	self.memoryUsageView.backgroundColor = [UIColor clearColor];
	
	[self.view addSubview:self.memoryUsageView];
	
	self.tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTapGesture)];
	self.tapRecognizer.numberOfTapsRequired = 4;
	[self.memoryUsageView addGestureRecognizer:self.tapRecognizer];
}
#endif

// Updates the localization. Currently it substitutes the splash screen.
- (void)applyLocalization {
	NSString *localization = LANGLITERAL;
	if (localization.length > 0) {
		self.imageView.image = [UIImage imageNamed:[NSString stringWithFormat:@"splash-%@.png", localization]];
	}
}

// Sets the audio session category to playback. This should keep the sound
// playing even if the mute is on on iPhone with iOS 6.0+.
// http://stackoverflow.com/questions/11616001/uiwebview-html5-audio-pauses-in-ios-6-when-app-enters-background
- (void)initAudioSession {
	AVAudioSession *audioSession = [AVAudioSession sharedInstance];
	BOOL ok;
	NSError *error = nil;
	ok = [audioSession setCategory:AVAudioSessionCategoryPlayback
							 error:&error];
	if (!ok) {
		NSLog(@"%@ error: %@", NSStringFromSelector(_cmd), error);
	}
}

- (void)initController {
	DDLogVerbose(@"initController");

	// required for proper scaling via affine transformations
	self.webView.layer.anchorPoint = self.imageView.layer.anchorPoint = CGPointZero;
	
	[self applyLocalization];
	
	// if fullscreen is off, don't scale up the splash
	if (![Settings sharedSettings].isFullscreen) {
		CGSize webAppSize = CGSizeMake(kWebAppWidth, kWebAppHeight);
		if (IS_IPHONE) {
			webAppSize = CGSizeMake(kWebAppWidth / 2., kWebAppHeight / 2.);
		}
		self.imageView.frame = [self frameOfViewWithSize:webAppSize centeredInViewWithFrame:self.view.frame];
	}
	
	[self updateSubviewsFramesWhenChangingFullscreen:[Settings sharedSettings].isFullscreen];
	[self.indicatorLoading startAnimating];
	
#ifdef ENABLE_TESTING_FEATURES
	self.showLaunchTime = NO;
	self.launchTime = [NSDate date];
#endif
	
	[self initAudioSession];
	[self loadContent];
	
	double delayInSeconds = .1;
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
	dispatch_after(popTime, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^(void){
        int kPort = 8086;
        if ([LANGLITERAL isEqualToString:@"fr"]) {
            kPort = 8088;
        } else if ([LANGLITERAL isEqualToString:@"es"]) {
            kPort = 8089;
        }
        
		MBMessageBroker *messageBroker = [MBMessageBroker sharedMessageBroker];
		self.server = [[MBServer alloc] initWithHost:kHost port:kPort andMessageBroker:messageBroker];
		[messageBroker startWithSender:self.server inDispatchQueue:dispatch_queue_create("MessageBrokerQueue", 0)];
		[self.server listen];

		self.controllerBackend = [[MBControllerBackend alloc] initWithHost:kHost andPort:kPort];
		[self.controllerBackend connect];
		self.controllerBackend.delegate = self;
		self.controllerBackend.queryDelegate = self;
        
        self.controllerRate = [[MBControllerRate alloc] initWithHost:kHost
                                                             andPort:kPort];
        self.controllerRate.delegate = self;
        [self.controllerRate connect];

		[Settings sharedSettings].delegate = self;
	});
	
#ifdef ENABLE_TESTING_FEATURES
	self.imageView.userInteractionEnabled = YES;
	self.longPressRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPressGesture)];
	self.longPressRecognizer.minimumPressDuration = 2;
	self.longPressRecognizer.numberOfTouchesRequired = 3;
	[self.imageView addGestureRecognizer:self.longPressRecognizer];
	
	self.swipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture)];
	self.swipeRecognizer.direction = UISwipeGestureRecognizerDirectionLeft;
	self.swipeRecognizer.numberOfTouchesRequired = 2;
	[self.imageView addGestureRecognizer:self.swipeRecognizer];
	
	[self addVersionButton];
	[self addMemoryUsageFeature];
#endif
}

- (void)viewDidUnload
{
	if ([Settings sharedSettings].delegate == self) {
		[Settings sharedSettings].delegate = nil;
	}

	[[MBMessageBroker sharedMessageBroker] stop];
	self.controllerBackend.delegate = nil;
	self.controllerBackend.queryDelegate = nil;
	self.controllerBackend = nil;
    self.controllerRate.delegate = nil;
    self.controllerRate = nil;
	self.server = nil;

#ifdef ENABLE_TESTING_FEATURES
	[self.imageView removeGestureRecognizer:self.longPressRecognizer];
	self.longPressRecognizer = nil;
	[self.imageView removeGestureRecognizer:self.swipeRecognizer];
	self.swipeRecognizer = nil;
	[self.memoryUsageView removeGestureRecognizer:self.tapRecognizer];
	self.tapRecognizer = nil;
	[self.versionView removeGestureRecognizer:self.tapVersionRecognizer];
	self.tapVersionRecognizer = nil;
#endif

	self.imageView = nil;
	self.webView = nil;
	[self setIndicatorLoading:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}

#pragma mark - MBControllerBackendDelegate

- (void)webAppDidLoad {
	DDLogVerbose(@"webAppDidLoad");
	[self hideSplash];
	[APP_DELEGATE webAppDidLoad];
	
#ifdef ENABLE_TESTING_FEATURES
	NSTimeInterval launchDiff = [[NSDate date] timeIntervalSinceDate:self.launchTime];
	DDLogInfo(@"Launch time: %f s.", launchDiff);
	if (self.showLaunchTime) {
		int64_t delayInSeconds = 0.5f;
		dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
		dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
			[[[UIAlertView alloc] initWithTitle:nil
										message:[NSString stringWithFormat:@"Launch time: %f s.", launchDiff]
									   delegate:nil
							  cancelButtonTitle:@"OK"
							  otherButtonTitles:nil] show];
		});
	}
#endif
}

- (void)setIsNavigationEnabled:(BOOL)isNavigationEnabled {
	[Settings sharedSettings].navigationEnabled = isNavigationEnabled;
}

- (void)setVehicleModel:(NSString *)vehicleModel {
	[Settings sharedSettings].vehicleModel = vehicleModel;
}

- (void)openEULA {
    EULAViewController *eulaViewController = [EULAViewController new];
    eulaViewController.viewMode = EULAViewModeReview;
    eulaViewController.delegate = self;
    [self presentModalViewController:eulaViewController animated:YES];
}

- (BOOL)sendSupportEmailWithParameters:(NSDictionary *)params {
    if (![MFMailComposeViewController canSendMail]) {
        DDLogInfo(@"This device can't send emails");
        return NO;
    }
    
    NSString *toRecipient = params[kKeyEmailRecipient];
    if (!toRecipient) {
        DDLogWarn(@"Recipient is null, setting to empy string");
        toRecipient = @"";
    }
    
    NSString *body = params[kKeyEmailBody];
    BOOL bodyInHTML = [params[kKeyEmailBodyInHTML] boolValue];
    
    MFMailComposeViewController *mailController = [MFMailComposeViewController new];
    mailController.mailComposeDelegate = self;
    [mailController setToRecipients:@[toRecipient]];
    [mailController setMessageBody:body isHTML:bodyInHTML];
    
    [self presentModalViewController:mailController animated:YES];
    
    return YES;
}

- (void)setIsFullScreen:(BOOL)isFullScreen {
    DDLogWarn(@"Set full screen %d", isFullScreen);
    [Settings sharedSettings].isFullscreen = isFullScreen;
	[self updateSubviewsFramesWhenChangingFullscreen:isFullScreen];
}

#pragma mark - MBControllerBackendQueryDelegate

- (BOOL)isFirstStart {
	return [[Settings sharedSettings] isFirstLaunch];
}

- (BOOL)isFullscreen {
	return [[Settings sharedSettings] isFullscreen];
}

- (BOOL)isNavigationEnabled {
	return [Settings sharedSettings].navigationEnabled;
}

- (NSString *)vehicleModel {
	return [Settings sharedSettings].vehicleModel;
}

- (CGSize)webViewSize {
	return self.webView.frame.size;
}

#pragma mark - EULAViewControllerDelegate

- (void)eulaViewControllerDidFinish:(EULAViewController *)eulaViewController {
#pragma unused (eulaViewController)
    [self dismissModalViewControllerAnimated:YES];
}

#pragma mark - MFMailComposeViewControllerDelegate

- (void)mailComposeController:(MFMailComposeViewController *)controller
          didFinishWithResult:(MFMailComposeResult)result
                        error:(NSError *)error {
#pragma unused (controller)
#pragma unused (error)
    DDLogInfo(@"Mail controller result: %d", result);
    if (error) {
        DDLogError(@"Mail controller error: %@", error);
    }
    [self dismissModalViewControllerAnimated:YES];
}

#pragma mark - MBControllerRateDelegate

- (MBControllerRateResult)rateAppFromControllerRate:(MBControllerRate *)controller {
#pragma unused (controller)
    // get the app id from info.plist
    NSString *appId = [[[NSBundle mainBundle] infoDictionary] objectForKey:KEY_APP_ID];
    if (!appId) {
        DDLogError(@"App Id is not defined in info.plist");
        return MBControllerRateResultUnknownAppID;
    }
    
    if (![[Reachability reachabilityForInternetConnection] isReachable]) {
        DDLogInfo(@"No internet connection");
        return MBControllerRateResultNoNetwork;
    }
    
    Reachability *appStoreReachability = [Reachability reachabilityWithHostname:RATE_APP_URL_HOST];
    if (![appStoreReachability isReachable]) {
        DDLogWarn(@"AppStore is unreachable");
        return MBControllerRateResultStoreNotAvailable;
    }
    
    NSString *url = [NSString stringWithFormat:RATE_APP_URL_TEMPLATE, appId];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
    return MBControllerRateResultOK;
}

#pragma mark - Privates

- (void)updateSubviewsFramesWhenChangingFullscreen:(BOOL)isFullscreen {
	self.webView.bounds = self.imageView.bounds = CGRectMake(0, 0, kWebAppWidth, kWebAppHeight);
	
	CGRect frame = [UIScreen mainScreen].bounds;
	// for some reason, the view has portrait's frame in landscape orientation
	// after reopening the app. so we set width to the bigger value
	CGSize correctSize;
	correctSize.width = MAX(frame.size.width, frame.size.height);
	correctSize.height = MIN(frame.size.width, frame.size.height);
	frame.size = correctSize;

	float scaleFactor = 1.0;

	if (isFullscreen) {
		scaleFactor = MIN(frame.size.width / kWebAppWidth, frame.size.height / kWebAppHeight);
	} else {
		if (IS_IPHONE) {
			// to apps, the dimensions of the iPhone screen is 480x320
			// so we half the size
			scaleFactor = 0.5;
		}
	}
	
	CGFloat newHeight = kWebAppHeight * scaleFactor;
	CGPoint shiftPoint = CGPointMake((frame.size.width - (kWebAppWidth * scaleFactor)) / 2.0, (frame.size.height - newHeight) / 2.0);
	self.webView.center = self.imageView.center = shiftPoint;
	self.webView.transform = self.imageView.transform = CGAffineTransformMakeScale(scaleFactor, scaleFactor);

	// relocate loading indicator
	CGFloat indicatorY = .705f;
	CGPoint center = CGPointMake(frame.size.width / 2.0, shiftPoint.y + (newHeight * indicatorY));
	self.indicatorLoading.center = center;
}

- (CGRect)frameOfViewWithSize:(CGSize)size centeredInViewWithFrame:(CGRect)frame {
	return CGRectMake((frame.size.width - size.width) / 2, (frame.size.height - size.height) / 2, size.width, size.height);
}

- (CGSize)landscapeScreenSize {
	CGSize screenBounds = [UIScreen mainScreen].bounds.size;
	return CGSizeMake(MAX(screenBounds.width, screenBounds.height),
					  MIN(screenBounds.width, screenBounds.height));
}

#pragma mark - SettingsDelegate

- (void)settings:(Settings *)settings fullscreenDidChange:(BOOL)isFullscreen {
#pragma unused (settings)
	DDLogVerbose(@"fullscreen: %d", isFullscreen);
	[self updateSubviewsFramesWhenChangingFullscreen:isFullscreen];
	[self.controllerBackend onFullscreenChanged:isFullscreen];
}

- (void)settings:(Settings *)settings navigationEnabledDidChange:(BOOL)isNavigationEnabled {
#pragma unused (settings)
	DDLogVerbose(@"navigationEnabled: %d", isNavigationEnabled);
	[self.controllerBackend onNavigationEnabledChanged:isNavigationEnabled];
}

- (void)settings:(Settings *)settings vehicleModelDidChange:(NSString *)vehicleModel {
#pragma unused (settings)
	DDLogVerbose(@"vehicleModel: %@", vehicleModel);
	[self.controllerBackend onVehicleModelChanged:vehicleModel];
}

#ifdef ENABLE_TESTING_FEATURES

#pragma mark - Gesture recognizer

- (void)handleLongPressGesture {
	[self.imageView removeGestureRecognizer:self.longPressRecognizer];
	self.longPressRecognizer = nil;

	[[Settings sharedSettings] resetLaunchCount];

	static const int yDiff = 5;
	CGPoint center = self.imageView.center;
	center.y += yDiff;
	self.imageView.center = center;

	double delayInSeconds = .25;
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
	dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
		CGPoint center = self.imageView.center;
		center.y -= yDiff;
		self.imageView.center = center;
	});
}

- (void)handleSwipeGesture {
	[self.imageView removeGestureRecognizer:self.swipeRecognizer];
	self.swipeRecognizer = nil;
	
	self.showLaunchTime = YES;
}

- (void)handleTapGesture {
	NSString *message = [APP_DELEGATE.memoryUsageComments componentsJoinedByString:@"\n"];
	[[[UIAlertView alloc] initWithTitle:@"Memory usage"
								message:message
							   delegate:nil
					  cancelButtonTitle:@"Close"
					  otherButtonTitles:nil] show];
}

- (void)handleVersionTapGesture {
	NSString *message = [self appVersion];
	[[[UIAlertView alloc] initWithTitle:nil
								message:message
							   delegate:nil
					  cancelButtonTitle:@"Close"
					  otherButtonTitles:nil] show];
}

#endif

@end
