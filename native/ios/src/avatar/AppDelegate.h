//
//  AppDelegate.h
//  avatar
//
//  Created by Vasiliy Saenko on 10.01.12.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) ViewController *viewController;

#ifdef ENABLE_TESTING_FEATURES
@property (unsafe_unretained, nonatomic, readonly) NSArray *memoryUsageComments;
#endif

// Notifies the AppDelegate that web is loaded
- (void)webAppDidLoad;

@end
