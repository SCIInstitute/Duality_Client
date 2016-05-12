//  Created by David McCann on 5/12/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "SceneWrapper.h"

@implementation SceneWrapper

-(id) initWithScene:(Scene*)scene
{
    self = [super init];
    m_scene = scene;
    return self;
}

-(Scene*) scene
{
    return m_scene;
}

@end