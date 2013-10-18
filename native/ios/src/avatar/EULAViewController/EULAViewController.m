//
//  EULAViewController.m
//  avatar
//
//  Created by Eugene Nikolsky on 2013-08-15.
//
//

#import "EULAViewController.h"

#import <QuartzCore/QuartzCore.h>

#define GRADIENT_TOP_COLOR [UIColor colorWithRed:54.0/255 green:54.0/255 blue:54.0/255 alpha:1]
#define GRADIENT_BOTTOM_COLOR [UIColor colorWithRed:98.0/255 green:98.0/255 blue:98.0/255 alpha:1]

@interface EULAViewController ()

@property (nonatomic, strong) CAGradientLayer *gradientLayer;

@end

@implementation EULAViewController

- (void)viewWillLayoutSubviews {
    if (self.gradientLayer) {
        self.gradientLayer.frame = self.view.bounds;
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.gradientLayer = [CAGradientLayer layer];
    self.gradientLayer.frame = self.view.bounds;
    self.gradientLayer.colors = @[(id)GRADIENT_TOP_COLOR.CGColor,
                                  (id)GRADIENT_BOTTOM_COLOR.CGColor];
    [self.view.layer insertSublayer:self.gradientLayer atIndex:0];
    
    NSString *btnAgreeTitle = nil;
    switch (self.viewMode) {
        case EULAViewModeAgree:
            btnAgreeTitle = NSLocalizedString(@"I Agree", nil);
            break;
            
        case EULAViewModeReview:
        default:
            btnAgreeTitle = NSLocalizedString(@"Done", nil);
            break;
    }
    [self.btnAgree setTitle:btnAgreeTitle forState:UIControlStateNormal];

    NSString *eulaFile = [[NSBundle mainBundle] pathForResource:@"MyFord_Touch_EULA"
                                                     ofType:@"txt"];
    NSError *error = nil;
    NSString *eula = [NSString stringWithContentsOfFile:eulaFile
                                               encoding:NSUTF8StringEncoding
                                                  error:&error];
    if (eula) {
        self.textView.text = eula;
    } else {
        DDLogError(@"Can't load EULA: %@", error.localizedDescription);
    }
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    // hack for iOS 4
    [self viewWillLayoutSubviews];
}

- (void)viewDidUnload {
    self.textView = nil;
    self.btnAgree = nil;
    [self.gradientLayer removeFromSuperlayer], self.gradientLayer = nil;
    [super viewDidUnload];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return UIInterfaceOrientationIsLandscape(toInterfaceOrientation);
}

#pragma mark - IBActions

- (IBAction)actionAgree {
    if ([self.delegate respondsToSelector:@selector(eulaViewControllerDidFinish:)]) {
        [self.delegate eulaViewControllerDidFinish:self];
    }
}

@end
