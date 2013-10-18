//
//  EULAViewController.h
//  avatar
//
//  Created by Eugene Nikolsky on 2013-08-15.
//
//

#import <UIKit/UIKit.h>

@protocol EULAViewControllerDelegate;


typedef NS_ENUM(NSUInteger, EULAViewMode) {
    EULAViewModeAgree = 0,
    EULAViewModeReview
};

/*
 * The class is responsible for displaying the app's EULA.
 */
@interface EULAViewController : UIViewController

@property (strong, nonatomic) IBOutlet UIButton *btnAgree;
@property (strong, nonatomic) IBOutlet UITextView *textView;

@property (nonatomic, assign) EULAViewMode viewMode;
@property (nonatomic, unsafe_unretained) id<EULAViewControllerDelegate> delegate;

- (IBAction)actionAgree;

@end



/*
 * This protocol is used to notify the caller of different events.
 */
@protocol EULAViewControllerDelegate <NSObject>

// Called when the user has finished with (agreed or reviewed) the EULA.
- (void)eulaViewControllerDidFinish:(EULAViewController *)eulaViewController;

@end
