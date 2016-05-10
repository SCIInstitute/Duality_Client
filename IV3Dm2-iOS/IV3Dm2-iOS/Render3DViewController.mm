//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "Render3DViewController.h"

#include "IVDA/iOS.h"
#include "Render/RenderDispatcher.h"
#include "Render/ScreenInfo.h"
#include "Scene/Scene.h"

@implementation Render3DViewController

@synthesize context = _context;

- (id)initWithScene:(Scene*)scene {
    self = [super init];
    m_scene = scene;
    m_rendererDispatcher = nullptr;
    return self;
}

-(void) changeScene:(Scene*)scene {
    m_scene = scene;
}

- (ScreenInfo)screenInfo {
    float scale = iOS::DetectScreenScale();
    float iPad = iOS::DetectIPad() ? 2 : 1;
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    float windowWidth = 1.0f;
    float windowHeight = 1.0f;
    unsigned int width = scale * self.view.bounds.size.width;
    unsigned int height = scale * self.view.bounds.size.height;
    ScreenInfo screenInfo(width, height, xOffset, yOffset,
                          /*m_pSettings->getUseRetinaResolution() ? 1.0 : fScale*/ scale, iPad * scale * 2, windowWidth, windowHeight);
    return screenInfo;
}

- (void)initGL {
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    // FIXME: Only OpenGL ES 3???
    
    if (!self.context) {
        NSLog(@"Failed to create OpenGLES context");
    }

    [EAGLContext setCurrentContext:self.context];
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.multipleTouchEnabled = YES;
    
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initGL];
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    m_rendererDispatcher = std::make_unique<RenderDispatcher>([self screenInfo]);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    if (m_scene) {
        m_scene->render(*m_rendererDispatcher);
    }
    [view bindDrawable];
}

@end
