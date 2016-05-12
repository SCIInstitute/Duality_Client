//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "AppDelegate.h"
#import "Render3DViewController.h"
#import "SelectSceneViewController.h"
#import "SettingsViewController.h"
#import "SceneWrapper.h"

#include "Scene/ServerAdapter.h"
#include "Scene/Scene.h"
#include "Scene/GeometryRenderer.h"

#include "mocca/net/ConnectionFactorySelector.h"

@interface AppDelegate ()

@end


@implementation AppDelegate

@synthesize window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    mocca::net::ConnectionFactorySelector::addDefaultFactories();
    
    auto serverAdapter = std::make_unique<ServerAdapter>();
    m_sceneLoader = std::make_unique<SceneLoader>(std::move(serverAdapter));
    
    Render3DViewController* renderView = [[Render3DViewController alloc] init];
    SelectSceneViewController* sceneView = [[SelectSceneViewController alloc] init];
    SettingsViewController* settingsView = [[SettingsViewController alloc] init];    
    m_tabBarViewController = [[TabBarViewController alloc] initWithRenderView:renderView andSceneView:sceneView andSettingsView:settingsView andSceneLoader:m_sceneLoader.get()];
    
    // Create window and content view (to hold other views)
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];

    // Set background color
    [window setBackgroundColor:[UIColor blackColor]];
    
    if ([window respondsToSelector:@selector(setRootViewController:)]) {
        window.rootViewController = m_tabBarViewController;
    } else {
        [window addSubview:m_tabBarViewController.view];
    }
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loadScene:) name:@"SelectedSceneChanged" object:nil];
    
    // Add subview to window to reference the tabbarcontroller
    [window makeKeyAndVisible];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

-(void)loadScene:(NSNotification*)notification
{
    NSString* name = notification.userInfo[@"name"];
    if (m_sceneLoader->loadScene(std::string([name UTF8String]))) {
        Scene* scene = m_sceneLoader->activeScene();
        SceneWrapper* sceneWrapper = [[SceneWrapper alloc] initWithScene:scene];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"NewSceneLoaded" object:self userInfo:@{@"scene":sceneWrapper}];
    }
}

@end
