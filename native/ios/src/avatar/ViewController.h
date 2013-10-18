//
//  ViewController.h
//  avatar
//
//  Created by Vasiliy Saenko on 23.09.11.
//  Copyright (c) 2012 Luxoft. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const WebTag;
// Output web logs with this macro
#define DDLogWeb(frmt, ...) LOG_OBJC_TAG_MACRO(NO, 0, 0, 0, WebTag, frmt, ##__VA_ARGS__)


@interface ViewController : UIViewController

@property (nonatomic, strong) UIImageView *imageView;
@property (nonatomic, strong) UIWebView *webView;
@property (strong, nonatomic) IBOutlet UIActivityIndicatorView *indicatorLoading;

// Starts loading webview and other stuff.
- (void)initController;

// Called when the app became active or non active.
- (void)appStatusDidChange:(BOOL)appDidBecomeActive;

@end
