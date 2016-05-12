//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/Scene.h"

@interface SceneWrapper : NSObject
{
    Scene* m_scene;
}

-(id) initWithScene:(Scene*)scene;
-(Scene*)scene;

@end

