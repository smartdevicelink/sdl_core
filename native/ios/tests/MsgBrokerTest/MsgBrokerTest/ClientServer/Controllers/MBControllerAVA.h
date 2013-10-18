//
//  MBControllerAVA.h
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MBController.h"

@interface MBControllerAVA : MBController

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port;

// Asks the phone to make a call to the specified phone number
- (void)makeCall:(NSString *)phoneNumber;

- (void)processNotification:(NSDictionary *)notification;

@end
