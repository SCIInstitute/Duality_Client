//
//  TabBarViewController.m
//  IV3Dm2-iOS
//
//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "TabBarViewController.h"
#import "AppDelegate.h"

#import <Foundation/Foundation.h>

@implementation TabBarViewController

- (id)initWithRenderView:(Render3DViewController*)renderView andSceneView:(SelectSceneViewController*)sceneView andSettingsView:(SettingsViewController*)settingsView {
    self = [super init];
    if (self) {
        m_render3DViewController = renderView;
        m_selectSceneViewController = sceneView;
        m_settingsViewController = settingsView;
        [self createNavigationControllers];
    }
    return self;
}

- (void) createNavigationControllers {
    // Array to contain the view controllers
    NSMutableArray *viewControllersArray = [[NSMutableArray alloc] init];
    UINavigationController* navController;

    navController = [self createNavigationController:m_render3DViewController withTitle:@"3D View"];
    [viewControllersArray addObject:navController];
    
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

- (void)tabBar:(UITabBar *)tabBar didSelectItem:(UITabBarItem *)item
{
    AppDelegate* app = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    Scene* scene = [app currentScene];
    [m_render3DViewController changeScene:scene];
}

@end
