//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include <memory>

class Scene;
class AbstractRenderer;

@interface Render3DViewController : GLKViewController
{
@protected
    Scene* m_scene;
    std::unique_ptr<AbstractRenderer> m_renderer;
}

-(id) initWithScene:(Scene*)scene;
-(void) changeScene:(Scene*)scene;

@property (nonatomic, retain) EAGLContext *context;

@end