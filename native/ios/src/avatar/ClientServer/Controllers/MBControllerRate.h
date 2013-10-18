//
//  MBControllerRate.h
//  avatar
//
//  Created by Eugene Nikolsky on 2013-09-04.
//  Copyright (c) 2013 Luxoft. All rights reserved.
//

#import "MBController.h"

@protocol MBControllerRateDelegate;

@interface MBControllerRate : MBController

@property (nonatomic, unsafe_unretained) id<MBControllerRateDelegate> delegate;

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port;

@end



typedef NS_ENUM(NSInteger, MBControllerRateResult) {
    MBControllerRateResultOK,
    MBControllerRateResultNoNetwork,
    MBControllerRateResultStoreNotAvailable,
    MBControllerRateResultUnknownAppID
};

// Protocol to notify the application about events coming from the web.
@protocol MBControllerRateDelegate <NSObject>

// Asks the delegate to open the AppStore to rate the app. Should return the
// result.
- (MBControllerRateResult)rateAppFromControllerRate:(MBControllerRate *)controller;

@end
