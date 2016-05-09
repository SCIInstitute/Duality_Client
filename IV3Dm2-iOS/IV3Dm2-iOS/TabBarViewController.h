//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "SelectSceneViewController.h"
#import "SettingsViewController.h"

#import <UIKit/UIKit.h>

class SceneProvider;

@interface TabBarViewController : UITabBarController {
@private
    SelectSceneViewController* m_selectSceneViewController;
    SettingsViewController* m_settingsViewController;
}

- (id)initWithSceneProvider:(SceneProvider*)provider;

@end
