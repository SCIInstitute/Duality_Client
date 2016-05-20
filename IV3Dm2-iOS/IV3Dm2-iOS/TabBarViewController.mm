//
//  TabBarViewController.m
//  IV3Dm2-iOS
//
//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "TabBarViewController.h"

#include "Scene/SceneLoader.h"

@implementation TabBarViewController

- (id)init
{
    self = [super init];
    
    m_render3DViewController = [[Render3DViewController alloc] init];
    m_selectSceneViewController = [[SelectSceneViewController alloc] init];
    m_settingsViewController = [[SettingsViewController alloc] init];

    [self createNavigationControllers];
    [self reinitSceneLoader:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reinitSceneLoader:) name:@"ServerAddressChanged" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(selectNewScene:) name:@"SelectedSceneChanged" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updateDatasets:) name:@"DatasetChanged" object:nil];
    
    return self;
}

- (void) createNavigationControllers {
    // Array to contain the view controllers
    NSMutableArray *viewControllersArray = [[NSMutableArray alloc] init];
    UINavigationController* navController;

    navController = [self createNavigationController:m_render3DViewController withTitle:@"3D View"];
    navController.navigationBar.hidden = YES;
    navController.tabBarItem.tag = 0;
    [viewControllersArray addObject:navController];
    
    navController = [self createNavigationController:m_selectSceneViewController withTitle:@"Select Scene"];
    navController.tabBarItem.tag = 1;
    [viewControllersArray addObject:navController];

    navController = [self createNavigationController:m_settingsViewController withTitle:@"Settings"];
    navController.tabBarItem.tag = 2;
    [viewControllersArray addObject:navController];
    
    self.viewControllers = viewControllersArray;
}

- (UINavigationController*) createNavigationController:(UIViewController*)viewController withTitle:(NSString*)title
{
    UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:viewController];
    UITabBarItem* tabBarItem  = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(title, @"Title of the tab") image:[UIImage imageNamed:@"data.png"] tag:0];
    navController.tabBarItem = tabBarItem;
    return navController;
}

-(void) selectNewScene:(NSNotification*)notification
{
    NSString* name = notification.userInfo[@"name"];
    if (m_sceneLoader->loadScene(std::string([name UTF8String]))) {
        Scene* scene = m_sceneLoader->activeScene();
        scene->updateDatasets();
        [m_render3DViewController setScene:m_sceneLoader->activeScene()];
    }
}

-(void) reinitSceneLoader:(NSNotification*)notification
{
    std::string serverIP = [[[NSUserDefaults standardUserDefaults] stringForKey:@"ServerIP"] UTF8String];
    uint16_t serverPort = [[NSUserDefaults standardUserDefaults] integerForKey:@"ServerPort"];
    mocca::net::Endpoint endpoint("tcp.prefixed", serverIP, std::to_string(serverPort));
    m_sceneLoader = std::make_unique<SceneLoader>(endpoint);
    [m_render3DViewController reset];
}

-(void) updateDatasets:(NSNotification*)notification
{
    Scene* scene = m_sceneLoader->activeScene();
    NSDictionary* changeData = notification.userInfo;
    std::string objectName([(NSString*)changeData[@"objectName"] UTF8String]);
    std::string variableName([(NSString*)changeData[@"variableName"] UTF8String]);
    id value = changeData[@"value"];
    if ([value isKindOfClass:[NSNumber class]]) {
        scene->setVariable(objectName, variableName, [(NSNumber*)value floatValue]);
    } else if ([value isKindOfClass:[NSString class]]) {
        scene->setVariable(objectName, variableName, std::string([(NSString*)value UTF8String]));
    }
    scene->updateDatasets();
}

- (void)tabBar:(UITabBar *)tabBar didSelectItem:(UITabBarItem *)item
{
    // "Select Scene" tab was selected
    if ([item tag] == 1) {
        [m_selectSceneViewController setMetadata:m_sceneLoader->listMetadata()];
    }
}

@end
