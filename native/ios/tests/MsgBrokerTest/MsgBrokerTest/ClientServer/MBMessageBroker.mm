//
//  MBMessageBroker.mm
//  MsgBrokerTest
//
//  Created by Eugene Nikolsky on 7/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "MBMessageBroker.h"
#import <MessageBrokerLib/CMessageBroker.hpp>
#import <MessageBrokerLib/CSender.hpp>

/* C++ class that will receive messages from the message broker
 and pass them to the owner (MBServer object). */
class SenderImpl : public NsMessageBroker::CSender {
public:
	SenderImpl(id<MBMessageBrokerSender> sender) {
		this->_sender = sender;
	}

	ssize_t Send(int fd, const std::string& data) {
		ssize_t size = -1;
		if ([this->_sender respondsToSelector:@selector(sendString:withFd:)]) {
			NSString *string = [NSString stringWithCString:data.c_str() encoding:NSUTF8StringEncoding];
			size = [this->_sender sendString:string withFd:fd];
		}
		return size;
	}

private:
	id<MBMessageBrokerSender> _sender;
};



static MBMessageBroker *instance;


@interface MBMessageBroker ()

@property (nonatomic, assign) NsMessageBroker::CMessageBroker *messageBroker;
@property (nonatomic, assign) SenderImpl *senderImpl;

@end


@implementation MBMessageBroker

@synthesize messageBroker = _messageBroker;
@synthesize senderImpl = _senderImpl;

- (void)dealloc {
	[self stop];
	delete _senderImpl, _senderImpl = NULL;
	[super dealloc];
}

+ (void)initialize {
	static BOOL initialized = NO;
	if (!initialized) {
		initialized = YES;
		instance = [[MBMessageBroker alloc] init];
	}
}

- (id)init {
	if (self = [super init]) {
		_messageBroker = NsMessageBroker::CMessageBroker::getInstance();
	}
	return self;
}

#pragma mark - Methods

+ (MBMessageBroker *)sharedMessageBroker {
	return instance;
}

- (void)startWithSender:(id<MBMessageBrokerSender>)sender inDispatchQueue:(dispatch_queue_t)dispatchQueue {
	NSAssert(dispatchQueue, @"dispatchQueue can't be nil");
	
	if (self.senderImpl != NULL) {
		delete self.senderImpl, self.senderImpl = NULL;
	}
	
	self.senderImpl = new SenderImpl(sender);
	self.messageBroker->startMessageBroker(self.senderImpl);
	
	// process messageBroker's messages in a different queue (thread)
	dispatch_async(dispatchQueue, ^{
		self.messageBroker->MethodForThread(NULL);
	});
}

- (void)stop {
	self.messageBroker->stopMessageBroker();
}

- (void)receiveMessage:(NSString *)message fromSocketWithFd:(int)fd {
	self.messageBroker->onMessageReceived(fd, std::string(message.UTF8String));
}

@end
