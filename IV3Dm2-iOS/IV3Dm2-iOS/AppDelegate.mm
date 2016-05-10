//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "AppDelegate.h"
#import "Render3DViewController.h"
#import "SelectSceneViewController.h"
#import "SettingsViewController.h"

#include "Communication/ServerAdapter.h"
#include "Scene/Scene.h"
#include "Render/GeometryRenderer.h"

#include "mocca/net/ConnectionFactorySelector.h"

@interface AppDelegate ()

@end


@implementation AppDelegate

@synthesize window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    mocca::net::ConnectionFactorySelector::addDefaultFactories();
    
    m_serverAdapter = new ServerAdapter();
    m_scene = nullptr;
    
    Render3DViewController* renderView = [[Render3DViewController alloc] initWithScene:m_scene];

    auto sceneProvider = std::make_shared<SceneProvider>(*m_serverAdapter);
    SelectSceneViewController* sceneView = [[SelectSceneViewController alloc] initWithSceneProvider:sceneProvider];
    
    SettingsViewController* settingsView = [[SettingsViewController alloc] init];
    
    m_tabBarViewController = [[TabBarViewController alloc] initWithRenderView:renderView andSceneView:sceneView andSettingsView:settingsView];
    
    // Create window and content view (to hold other views)
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];

    // Set background color
    [window setBackgroundColor:[UIColor blackColor]];
    
    if ([window respondsToSelector:@selector(setRootViewController:)]) {
        window.rootViewController = m_tabBarViewController;
    } else {
        [window addSubview:m_tabBarViewController.view];
    }
    
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

- (void) changeScene:(Scene*)scene {
    m_scene = scene;
    m_scene->updateDatasets();
}

- (Scene*) currentScene {
    return m_scene;
}

@end
