//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "SceneDefinition.h"

#include <vector>

class SceneDefinitionProvider;

@interface SelectSceneViewController : UITableViewController
{
    std::vector<SceneDefinition> m_definitions;
}

- (id)initWithSceneDefinitionProvider:(SceneDefinitionProvider*)provider;

@end

