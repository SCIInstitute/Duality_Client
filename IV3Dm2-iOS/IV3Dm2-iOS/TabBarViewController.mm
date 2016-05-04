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

- (id)init {
    self = [super init];
    if (self) {
        [self setupUI];
    }
    return self;
}

- (void) setupUI {
    // Create nav-controller for local use
    UINavigationController *navController;
    
    // New array to contain the view controllers
    NSMutableArray *viewControllersArray = [[NSMutableArray alloc] init];
    UITabBarItem* tabBarItem;
    
    m_selectSceneViewController = [[SelectSceneViewController alloc] init];
    navController = [[UINavigationController alloc] initWithRootViewController:m_selectSceneViewController];
    tabBarItem =  [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"Select Scene", @"Title of the Dataset tab") image:[UIImage imageNamed:@"data.png"] tag:0];
    navController.tabBarItem = tabBarItem;
    [viewControllersArray addObject:navController];
    
    self.viewControllers = viewControllersArray;
}

@end
