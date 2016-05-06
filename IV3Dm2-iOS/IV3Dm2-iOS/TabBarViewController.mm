//
//  TabBarViewController.m
//  IV3Dm2-iOS
//
//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "TabBarViewController.h"

#import <Foundation/Foundation.h>

@implementation TabBarViewController

- (id)initWithSceneProvider:(SceneProvider*)provider {
    self = [super init];
    if (self) {
        m_selectSceneViewController = [[SelectSceneViewController alloc] initWithSceneProvider:provider];
        m_settingsViewController = [[SettingsViewController alloc] init];
        [self createNavigationControllers];
    }
    return self;
}

- (void) createNavigationControllers {
    // Array to contain the view controllers
    NSMutableArray *viewControllersArray = [[NSMutableArray alloc] init];
    UINavigationController* navController;
    
    navController = [self createNavigationController:m_selectSceneViewController withTitle:@"Select Scene"];
    [viewControllersArray addObject:navController];

    navController = [self createNavigationController:m_settingsViewController withTitle:@"Settings"];
    [viewControllersArray addObject:navController];
    
    self.viewControllers = viewControllersArray;
}

- (UINavigationController*) createNavigationController:(UIViewController*)viewController withTitle:(NSString*)title {
    UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:viewController];
    UITabBarItem* tabBarItem  = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(title, @"Title of the tab") image:[UIImage imageNamed:@"data.png"] tag:0];
    navController.tabBarItem = tabBarItem;
    return navController;
}

@end
