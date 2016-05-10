//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "TabBarViewController.h"

class ServerAdapter;
class Scene;
class AbstractRenderer;

@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    UIWindow * window;
    TabBarViewController* m_tabBarViewController;
    ServerAdapter* m_serverAdapter;
    Scene* m_scene;
}

@property (strong, nonatomic) IBOutlet UIWindow *window;

- (void) changeScene:(Scene*)scene;
- (Scene*) currentScene;

@end

