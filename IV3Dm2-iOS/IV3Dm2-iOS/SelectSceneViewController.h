//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "Scene.h"
#include "Communication/SceneProvider.h"

#include <vector>

@interface SelectSceneViewController : UITableViewController
{
    std::shared_ptr<SceneProvider> m_provider;
}

- (id)initWithSceneProvider:(std::shared_ptr<SceneProvider>)provider;

@end

