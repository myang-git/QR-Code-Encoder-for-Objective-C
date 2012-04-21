//
//  AppDelegate.h
//  QRCodeEncoderDemo
//
//  Created by Yang Ming-Hsien on 4/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QRCodeEncoderDemoViewController.h"

@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) QRCodeEncoderDemoViewController *viewController;

@end
