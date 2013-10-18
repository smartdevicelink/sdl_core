//
//  ViewController.m
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"
#import "MBServer.h"
#import "MBControllerAVA.h"
#import "MBControllerPhone.h"
#import "MBMessageBroker.h"

#define kHost	@"127.0.0.1"
#define kPort	8086

#define kPhoneBookChangedProperty	@"Phone.onPhoneBookChanged"

enum {
	tableCellRegisterComponentsIndex = 0,
	tableCellExternalMessageIndex,
	tableCellSubscribeIndex,
	tableCellNotifyIndex,
	tableCellUnsubscribeIndex,
	tableCellUnregisterComponentsIndex,
	
	tableCellCount	// always at the end
};

@interface ViewController ()

@property (nonatomic, retain) MBServer *server;
@property (nonatomic, retain) MBControllerAVA *controllerAVA;
@property (nonatomic, retain) MBControllerPhone *controllerPhone;

@end

@implementation ViewController

@synthesize tableView = _tableView;
@synthesize server = _server;
@synthesize controllerAVA = _controllerAVA;
@synthesize controllerPhone = _controllerPhone;

- (void)dealloc {
	[[MBMessageBroker sharedMessageBroker] stop];
	[_controllerAVA release], _controllerAVA = nil;
	[_controllerPhone release], _controllerPhone = nil;
	[_server release], _server = nil;
	
	[_tableView release];
	[super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
	
	MBMessageBroker *messageBroker = [MBMessageBroker sharedMessageBroker];
	
	self.server = [[[MBServer alloc] initWithHost:kHost port:kPort andMessageBroker:messageBroker] autorelease];
	DDLogInfo(@"server = %@", self.server);
	
	[messageBroker startWithSender:self.server inDispatchQueue:dispatch_get_global_queue(0, 0)];
		
	if (![self.server listen]) {
		DDLogWarn(@"Can't listen to port");
		return;
	}
	
	self.controllerAVA = [[[MBControllerAVA alloc] initWithHost:kHost andPort:kPort] autorelease];
	DDLogInfo(@"controllerAVA = %@", self.controllerAVA);
	
	if (![self.controllerAVA connect]) {
		DDLogWarn(@"controllerAVA: Can't connect to server");
		return;
	}
	
	self.controllerPhone = [[[MBControllerPhone alloc] initWithHost:kHost andPort:kPort] autorelease];
	DDLogInfo(@"controllerPhone = %@", self.controllerPhone);
	
	if (![self.controllerPhone connect]) {
		DDLogWarn(@"controllerPhone: Can't connect to server");
		return;
	}
}

- (void)viewDidUnload
{
	[[MBMessageBroker sharedMessageBroker] stop];
	self.controllerAVA = nil;
	self.controllerPhone = nil;
	self.server = nil;
	
	self.tableView = nil;
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return tableCellCount;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	static NSString *cellId = @"cellId";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellId];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellId] autorelease];
	}
	
	NSString *text = nil;
	switch (indexPath.row) {
		case tableCellRegisterComponentsIndex:
			text = NSLocalizedString(@"Register components", nil);
			break;
			
		case tableCellExternalMessageIndex:
			text = NSLocalizedString(@"External message", nil);
			break;
			
		case tableCellSubscribeIndex:
			text = NSLocalizedString(@"Subscribe to property", nil);
			break;
			
		case tableCellNotifyIndex:
			text = NSLocalizedString(@"Notify", nil);
			break;
			
		case tableCellUnsubscribeIndex:
			text = NSLocalizedString(@"Unsubscribe from property", nil);
			break;
			
		case tableCellUnregisterComponentsIndex:
			text = NSLocalizedString(@"Unregister components", nil);
			break;
			
		default:
			break;
	}
	
	cell.textLabel.text = text;
	
	return cell;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
	
	switch (indexPath.row) {
		case tableCellRegisterComponentsIndex:
			[self.controllerAVA registerControllerWithStartId:0];
			[self.controllerPhone registerControllerWithStartId:1];
			break;
			
		case tableCellExternalMessageIndex: {
			NSDate *startTime = [NSDate date];
			for (int i = 0; i < 100; ++i) {
				[self.controllerAVA makeCall:@"+18005552368"];
			}
			DDLogInfo(@" ****** Exec time = %f s.", [[NSDate date] timeIntervalSinceDate:startTime]);
			break;
		}
			
		case tableCellSubscribeIndex:
			[self.controllerAVA subscribeToProperty:kPhoneBookChangedProperty];
			break;
			
		case tableCellNotifyIndex:
			[self.controllerPhone onPhoneBookChanged];
			break;
			
		case tableCellUnsubscribeIndex:
			[self.controllerAVA unsubscribeFromProperty:kPhoneBookChangedProperty];
			break;
			
		case tableCellUnregisterComponentsIndex:
			[self.controllerAVA unregisterController];
			[self.controllerPhone unregisterController];
			break;
			
		default:
			break;
	}
}

@end
