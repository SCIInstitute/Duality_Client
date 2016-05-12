//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#import "DynamicUIBuilder.h"

#include "IVDA/Vectors.h"

#include <memory>

class Scene;
class RenderDispatcher;

@interface Render3DViewController : GLKViewController
{
@protected
    Scene* m_scene;
    DynamicUIBuilder* m_uiBuilder;
    std::unique_ptr<RenderDispatcher> m_rendererDispatcher;
    IVDA::Vec2f m_touchPos1;
    IVDA::Vec2f m_touchPos2;
}

-(id) init;
-(void)reloadScene:(NSNotification*)notification;

@property (nonatomic, retain) EAGLContext *context;

@end