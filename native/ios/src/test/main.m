//
//  main.m
//  test
//
//  Created by Eugene Nikolsky on 8/6/12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <objc/runtime.h>
#import <objc/message.h>

#import <DDTTYLogger.h>

@interface UIWindow (Private)
- (void) swizzled_createContext;
@end

@implementation UIWindow (Private)
- (void) swizzled_createContext {
	// nop
}
@end


// https://github.com/gabriel/gh-unit/issues/96#issuecomment-12881447
int main(int argc, char *argv[]) {
	int retVal;
	@autoreleasepool {
		[DDLog addLogger:[DDTTYLogger sharedInstance]];
		[DDTTYLogger sharedInstance].colorsEnabled = YES;

		CFMessagePortRef portRef = NULL;
		if (getenv("GHUNIT_CLI")) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundeclared-selector"
			Method originalWindowCreateContext =
			class_getInstanceMethod([UIWindow class],
									@selector(_createContext));
#pragma clang diagnostic pop
			
			Method swizzledWindowCreateConext =
			class_getInstanceMethod([UIWindow class],
									@selector(swizzled_createContext));
			
			method_exchangeImplementations(originalWindowCreateContext,
										   swizzledWindowCreateConext);
			
			
			portRef = CFMessagePortCreateLocal(NULL,
											   (CFStringRef) @"PurpleWorkspacePort",
											   NULL,
											   NULL,
											   NULL);
		}
		
		retVal = UIApplicationMain(argc, argv,
								   NSStringFromClass([UIApplication class]),
								   @"GHUnitIOSAppDelegate");
		if (portRef != NULL) { CFRelease(portRef); }
	}
	return retVal;
}