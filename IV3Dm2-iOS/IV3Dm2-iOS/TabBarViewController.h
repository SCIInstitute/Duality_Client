//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "SelectSceneViewController.h"
#import "SettingsViewController.h"
#import "Render3DViewController.h"

#import <UIKit/UIKit.h>

class SceneProvider;

@interface TabBarViewController : UITabBarController {
@private
    Render3DViewController* m_render3DViewController;
    SelectSceneViewController* m_selectSceneViewController;
    SettingsViewController* m_settingsViewController;
}

- (id)initWithRenderView:(Render3DViewController*)renderView andSceneView:(SelectSceneViewController*)sceneView andSettingsView:(SettingsViewController*)settingsView;

@end
