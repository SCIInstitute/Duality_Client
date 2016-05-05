//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "SelectSceneViewController.h"
#import "SettingsViewController.h"

#import <UIKit/UIKit.h>

class SceneDefinitionProvider;

@interface TabBarViewController : UITabBarController {
@private
    SelectSceneViewController* m_selectSceneViewController;
    SettingsViewController* m_settingsViewController;
}

- (id)initWithSceneDefinitionProvider:(SceneDefinitionProvider*)provider;

@end
