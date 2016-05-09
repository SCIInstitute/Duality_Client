//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

class Scene;
class AbstractRenderer;

@interface Render3DViewController : GLKViewController
{
@protected
    Scene* m_scene;
    AbstractRenderer* m_renderer;
}

-(id) initWithScene:(Scene*)scene andRenderer:(AbstractRenderer*)renderer;

@end