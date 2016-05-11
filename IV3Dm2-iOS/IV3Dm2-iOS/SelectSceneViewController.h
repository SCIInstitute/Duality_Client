//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "Scene/Scene.h"
#include "Scene/SceneLoader.h"

#include <vector>

@interface SelectSceneViewController : UITableViewController
{
    std::shared_ptr<SceneLoader> m_provider;
}

- (id)initWithSceneLoader:(std::shared_ptr<SceneLoader>)provider;

@end

