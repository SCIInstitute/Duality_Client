//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Render3DViewController.h"

#include "Scene/Scene.h"
#include "Render/AbstractRenderer.h"

@implementation Render3DViewController

-(id) initWithScene:(Scene*)scene andRenderer:(AbstractRenderer*)renderer {
    self = [super init];
    m_scene = scene;
    m_renderer = renderer;
    return self;
}


@end
