//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "Scene.h"

#include <vector>

class SceneProvider;

@interface SelectSceneViewController : UITableViewController
{
    std::vector<Scene> m_scenes;
}

- (id)initWithSceneProvider:(SceneProvider*)provider;

@end

