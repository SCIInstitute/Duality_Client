//
//  AppDelegate.h
//  IV3Dm2-iOS
//
//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "TabBarViewController.h"

#import <UIKit/UIKit.h>

@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    UIWindow * window;
    TabBarViewController* m_tabBarViewController;
}

@property (strong, nonatomic) IBOutlet UIWindow *window;


@end
