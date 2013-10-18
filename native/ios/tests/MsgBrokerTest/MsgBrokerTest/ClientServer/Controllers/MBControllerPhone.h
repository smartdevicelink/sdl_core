//
//  MBControllerPhone.h
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MBController.h"

@interface MBControllerPhone : MBController

- (id)initWithHost:(NSString *)host andPort:(uint16_t)port;

// Sends a notification that the phone book changed.
- (void)onPhoneBookChanged;

- (void)processRequest:(NSDictionary *)request;

@end
