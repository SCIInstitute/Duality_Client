//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "TabBarViewController.h"

class SceneLoader;

@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    UIWindow * window;
    TabBarViewController* m_tabBarViewController;
    std::unique_ptr<SceneLoader> m_sceneLoader;
}

@property (strong, nonatomic) IBOutlet UIWindow *window;

-(void)loadScene:(NSNotification*)notification;

@end

