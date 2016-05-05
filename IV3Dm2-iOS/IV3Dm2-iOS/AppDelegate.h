//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "TabBarViewController.h"

#include "ServerAdapter.h"

@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    UIWindow * window;
    TabBarViewController* m_tabBarViewController;
    ServerAdapter* m_serverAdapter;
}

@property (strong, nonatomic) IBOutlet UIWindow *window;


@end

