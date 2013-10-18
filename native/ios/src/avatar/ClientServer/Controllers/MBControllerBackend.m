//
//  MBControllerBackend.m
//  avatar
//
//  Created by Eugene Nikolsky on 7/6/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import "MBControllerBackend.h"
#import "MBConst.h"

NSString *const kKeyEmailRecipient = @"EmailRecipient";
NSString *const kKeyEmailBody = @"EmailBody";
NSString *const kKeyEmailBodyInHTML = @"EmailBodyInHTML";


#define kControllerName				@"Backend"

#define kMethodIsFirstStart			@"isFirstStart"
#define kMethodIsFullscreen			@"isFullScreen"
#define kMethodIsNavigationEnabled	@"hasMaps"
#define kMethodVehicleModel			@"getVehicleModel"
#define kMethodWebViewSize			@"getWindowSize"
#define kMethodScreenDensity		@"getWindowDensity"
#define kMethodOSInfo				@"getOSInfo"
#define kMethodLogToOS				@"logToOS"
#define kMethodSetIsNavigationEnabled	@"setHasMaps"
#define kMethodSetVehicleModel		@"setVehicleModel"
#define kMethodOpenEULA             @"openEULA"
#define kMethodSendSupportEmail     @"sendSupportEmail"
#define kMethodSetFullscreen        @"setFullScreen"

#define kParameterIsFirstStart		@"isFirstStart"
#define kParameterIsFullScreen		@"isFullScreen"
#define kParameterIsNavigationEnabled	@"hasMaps"
#define kParameterVehicleModel		@"vehicle"
#define kParameterWebViewWidth		@"width"
#define kParameterWebViewHeight		@"height"
#define kParameterScreenDensity		@"windowDensity"
#define kParameterOSType			@"osType"
#define kParameterOSVersion			@"osVersion"
#define kParameterIsNativeApp		@"isNativeApplication"
#define kParameterMessage			@"message"
#define kParameterAppBecameActive	@"appBecameActive"

#define kNotificationFullscreenChanged	kControllerName @".onFullScreenChanged"
#define kNotificationAppStatusChanged	kControllerName @".onAppStatusChanged"
#define kNotificationNavigationEnabledChanged	kControllerName @".onHasMapsChanged"
#define kNotificationVehicleModelChanged	kControllerName @".onVehicleChanged"

#define kMethodAppLoaded			@"onAppLoaded"
#define kPropertyAppLoaded			kControllerName @"." kMethodAppLoaded


@interface MBControllerBackend ()

// Flag to indicate whether the appLoaded notification has been received
@property (nonatomic, assign) BOOL appLoadedReceived;

// All of the following functions return dictionaries with properties.

// Returns YES if it's the first application launch after installation.
- (NSDictionary *)isFirstStart;
// Returns YES if the app is in fullscreen mode.
- (NSDictionary *)isFullscreen;
// Returns whether navigation is enabled.
- (NSDictionary *)isNavigationEnabled;
// Returns the current vehicle model, selected by the user.
- (NSDictionary *)vehicleModel;
// Returns webView's dimensions (width and height).
- (NSDictionary *)webViewSize;
// Returns screen's scale factor.
- (NSDictionary *)screenScale;
// Returns OS type, version, and whether it's a native application.
- (NSDictionary *)OSInfo;
// Logs the string in the system log. Returns "OK".
- (NSString *)logString:(NSString *)string;
// Sets the isNavigationEnabled flag and returns the result.
- (NSString *)setIsNavigationEnabled:(BOOL)isNavigationEnabled;
// Sets the vehicle model and returns the result.
- (NSString *)setVehicleModel:(NSString *)vehicleModel;
// Sets the full screen mode and returns the result.
- (NSString *)setFullScreen:(BOOL)isFullScreen;

- (NSString *)processOpenEULARequest:(NSDictionary *)request
                               error:(NSDictionary **)error;
- (NSString *)processSendSupportEmailRequest:(NSDictionary *)request
                                       error:(NSDictionary **)error;
- (NSString *)processSetFullScreenRequest:(NSDictionary *)request
                                    error:(NSDictionary **)error;

@end


@implementation MBControllerBackend

@synthesize delegate = _delegate;
@synthesize queryDelegate = _queryDelegate;
@synthesize appLoadedReceived = _appLoadedReceived;

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port {
	if (self = [super initWithHost:host port:port andName:kControllerName]) {
		_appLoadedReceived = NO;
	}
	return self;
}

#pragma mark - Requests

- (void)processRequest:(NSDictionary *)request {
	NSString *method = [self methodNameOfMessage:request];
	id response = nil;
	NSDictionary *error = nil;
	
	if ([method isEqualToString:kMethodIsFirstStart]) {
		response = [self isFirstStart];
	} else if ([method isEqualToString:kMethodIsFullscreen]) {
		response = [self isFullscreen];
	} else if ([method isEqualToString:kMethodIsNavigationEnabled]) {
		response = [self isNavigationEnabled];
	} else if ([method isEqualToString:kMethodVehicleModel]) {
		response = [self vehicleModel];
	} else if ([method isEqualToString:kMethodWebViewSize]) {
		response = [self webViewSize];
	} else if ([method isEqualToString:kMethodScreenDensity]) {
		response = [self screenScale];
	} else if ([method isEqualToString:kMethodOSInfo]) {
		response = [self OSInfo];
	} else if ([method isEqualToString:kMethodLogToOS]) {
		NSDictionary *parameters = [request objectForKey:kParametersKey];
		NSString *string = [parameters objectForKey:kParameterMessage];
		response = [self logString:string];
	} else if ([method isEqualToString:kMethodSetIsNavigationEnabled]) {
		
		id params = request[kParametersKey];
		if ([params isKindOfClass:[NSDictionary class]]) {
			id param = params[kParameterIsNavigationEnabled];
			if ([param isKindOfClass:[NSNumber class]]) {
				BOOL isNavigationEnabled = [param boolValue];
				response = [self setIsNavigationEnabled:isNavigationEnabled];
			} else {
				error = [self prepareErrorResponseWithCode:kMBInvalidParams
												   message:@"The message has invalid " kParameterIsNavigationEnabled @" param"
												forMessage:request];
			}
		} else {
			error = [self prepareErrorResponseWithCode:kMBInvalidParams
											   message:@"The message has invalid params"
											forMessage:request];
		}
		
	} else if ([method isEqualToString:kMethodSetVehicleModel]) {
		
		id params = request[kParametersKey];
		if ([params isKindOfClass:[NSDictionary class]]) {
			id param = params[kParameterVehicleModel];
			if ([param isKindOfClass:[NSString class]]) {
				NSString *vehicle = (NSString *)param;
				response = [self setVehicleModel:vehicle];
			} else {
				error = [self prepareErrorResponseWithCode:kMBInvalidParams
												   message:@"The message has invalid " kParameterVehicleModel @" param"
												forMessage:request];
			}
		} else {
			error = [self prepareErrorResponseWithCode:kMBInvalidParams
											   message:@"The message has invalid params"
											forMessage:request];
		}
	} else if ([method isEqualToString:kMethodOpenEULA]) {
        response = [self processOpenEULARequest:request error:&error];
    } else if ([method isEqualToString:kMethodSendSupportEmail]) {
        response = [self processSendSupportEmailRequest:request error:&error];
    } else if ([method isEqualToString:kMethodSetFullscreen]) {
        response = [self processSetFullScreenRequest:request error:&error];
    }
	
	if (error) {
		[self sendJsonMessage:error];
	} else if (response) {
		[self sendJsonMessage:[self prepareResponse:response forMessage:request]];
	} else {
		DDLogWarn(@"Backend: no response for request %@", request);
		[super processRequest:request];
	}
}

#pragma mark - Notifications

- (void)processNotification:(NSDictionary *)notification {
	NSString *method = [self methodNameOfMessage:notification];
	if ([method isEqualToString:kMethodAppLoaded]) {
		if ([self.delegate respondsToSelector:@selector(webAppDidLoad)]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				[self.delegate webAppDidLoad];
			});
		}

		self.appLoadedReceived = YES;
		[self unsubscribeFromProperty:kPropertyAppLoaded];
	} else {
		[super processNotification:notification];
	}
}

- (void)onFullscreenChanged:(BOOL)isFullscreen {
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:isFullscreen]
														   forKey:kParameterIsFullScreen];
	[self sendJsonMessage:[self constructNotificationWithMethod:kNotificationFullscreenChanged
												   andParameter:parameters]];
}

- (void)onAppStatusChanged:(BOOL)appDidBecomeActive {
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:appDidBecomeActive]
														   forKey:kParameterAppBecameActive];
	[self sendJsonMessage:[self constructNotificationWithMethod:kNotificationAppStatusChanged
												   andParameter:parameters]];
}

- (void)onNavigationEnabledChanged:(BOOL)isNavigationEnabled {
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:isNavigationEnabled]
														   forKey:kParameterIsNavigationEnabled];
	[self sendJsonMessage:[self constructNotificationWithMethod:kNotificationNavigationEnabledChanged
												   andParameter:parameters]];
}

- (void)onVehicleModelChanged:(NSString *)vehicleModel {
	NSDictionary *parameters = [NSDictionary dictionaryWithObject:vehicleModel
														   forKey:kParameterVehicleModel];
	[self sendJsonMessage:[self constructNotificationWithMethod:kNotificationVehicleModelChanged
												   andParameter:parameters]];
}


- (void)didRegisterController {
	if (!self.appLoadedReceived) {
		// we require the appLoaded notification only once
		[self subscribeToProperty:kPropertyAppLoaded];
	}
}

#pragma mark - Overridden methods

- (void)unregisterController {
	if (!self.appLoadedReceived) {
		// unsubscribe from the appLoaded property if it hasn't been received
		[self unsubscribeFromProperty:kPropertyAppLoaded];
	}

	[super unregisterController];
}

#pragma mark - Privates

- (NSDictionary *)isFirstStart {
	BOOL firstStart = NO;
	if ([self.queryDelegate respondsToSelector:@selector(isFirstStart)]) {
		firstStart = [self.queryDelegate isFirstStart];
	}
	
	return [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:firstStart]
									   forKey:kParameterIsFirstStart];
}

- (NSDictionary *)isFullscreen {
	BOOL fullscreen = YES;
	if ([self.queryDelegate respondsToSelector:@selector(isFullscreen)]) {
		fullscreen = [self.queryDelegate isFullscreen];
	}
	
	return [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:fullscreen]
									   forKey:kParameterIsFullScreen];
}

- (NSDictionary *)isNavigationEnabled {
	BOOL isNavigationEnabled = YES;
	if ([self.queryDelegate respondsToSelector:@selector(isNavigationEnabled)]) {
		isNavigationEnabled = [self.queryDelegate isNavigationEnabled];
	}
	
	return [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:isNavigationEnabled]
									   forKey:kParameterIsNavigationEnabled];
}

- (NSDictionary *)vehicleModel {
	NSString *vehicle = @"";
	if ([self.queryDelegate respondsToSelector:@selector(vehicleModel)]) {
		vehicle = [self.queryDelegate vehicleModel];
	}
	
	return [NSDictionary dictionaryWithObject:vehicle
									   forKey:kParameterVehicleModel];
}

- (NSDictionary *)webViewSize {
	CGSize size = CGSizeMake(480, 320);
	if ([self.queryDelegate respondsToSelector:@selector(webViewSize)]) {
		size = [self.queryDelegate webViewSize];
	}
	
	return [NSDictionary dictionaryWithObjectsAndKeys:
			[NSNumber numberWithInt:size.width], kParameterWebViewWidth,
			[NSNumber numberWithInt:size.height], kParameterWebViewHeight,
			nil];
}

- (NSDictionary *)screenScale {
	float scale = 1;
	if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
		scale = [[UIScreen mainScreen] scale];
	}
	
	return [NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:scale]
									   forKey:kParameterScreenDensity];
}

- (NSDictionary *)OSInfo {
	return [NSDictionary dictionaryWithObjectsAndKeys:
			@"iOS", kParameterOSType,
			[[UIDevice currentDevice] systemVersion], kParameterOSVersion,
			[NSNumber numberWithBool:YES], kParameterIsNativeApp,
			nil];
}

- (NSString *)logString:(NSString *)string {
	DDLogInfo(@"logToOS: %@", string);
	
	return @"OK";
}

- (NSString *)setIsNavigationEnabled:(BOOL)isNavigationEnabled {
	if ([self.delegate respondsToSelector:@selector(setIsNavigationEnabled:)]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			[self.delegate setIsNavigationEnabled:isNavigationEnabled];
		});
	}
	
	return @"OK";
}

- (NSString *)setVehicleModel:(NSString *)vehicleModel {
	if ([self.delegate respondsToSelector:@selector(setVehicleModel:)]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			[self.delegate setVehicleModel:vehicleModel];
		});
	}
	
	return @"OK";
}

- (NSString *)setFullScreen:(BOOL)isFullScreen {
	if ([self.delegate respondsToSelector:@selector(setIsFullScreen:)]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			[self.delegate setIsFullScreen:isFullScreen];
		});
	}
	
	return @"OK";
}

- (id)processOpenEULARequest:(NSDictionary *)request
                       error:(NSDictionary **)error {
    if ([self.delegate respondsToSelector:@selector(openEULA)]) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.delegate openEULA];
        });
        
        return @"OK";
    } else {
        *error = [self createRequestNotHandledErrorForRequest:request];
    }
    
    return nil;
}

- (NSString *)processSendSupportEmailRequest:(NSDictionary *)request
                                       error:(NSDictionary **)error {
    if ([self.delegate respondsToSelector:@selector(sendSupportEmailWithParameters:)]) {
        UIDevice *device = [UIDevice currentDevice];
        NSString *body = [NSString stringWithFormat:@"Sent from %@ %@",
                          device.name, device.systemVersion];
        NSDictionary *emailParams = @{kKeyEmailRecipient: @"mftguide@ford.com",
                                      kKeyEmailBody: body,
                                      kKeyEmailBodyInHTML: @NO};
        __block BOOL success = NO;
        dispatch_sync(dispatch_get_main_queue(), ^{
            success = [self.delegate sendSupportEmailWithParameters:emailParams];
        });
        
        if (success) {
            return @"OK";
        } else {
            *error = [[self prepareErrorResponseWithCode:kMBCantSendEmail
                                                 message:@"This device can't send email"
                                              forMessage:request] copy];
        }
    } else {
        *error = [self createRequestNotHandledErrorForRequest:request];
    }
    
    return nil;
}

- (NSString *)processSetFullScreenRequest:(NSDictionary *)request
                                    error:(NSDictionary **)error {
    id params = request[kParametersKey];
    if ([params isKindOfClass:[NSDictionary class]]) {
        id param = params[kParameterIsFullScreen];
        if ([param isKindOfClass:[NSNumber class]]) {
            BOOL isFullScreen = [param boolValue];
            return [self setFullScreen:isFullScreen];
        } else {
            *error = [self prepareErrorResponseWithCode:kMBInvalidParams
                                               message:@"The message has invalid " kParameterIsFullScreen @" param"
                                            forMessage:request];
        }
    } else {
        *error = [self prepareErrorResponseWithCode:kMBInvalidParams
                                           message:@"The message has invalid params"
                                        forMessage:request];
    }
    
    return nil;
}

- (NSDictionary *)createRequestNotHandledErrorForRequest:(NSDictionary *)request {
    return [[self prepareErrorResponseWithCode:kMBRequestNotHandled
                                       message:@"No delegate for this request"
                                    forMessage:request] copy];
}

@end
