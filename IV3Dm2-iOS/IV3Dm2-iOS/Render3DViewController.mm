//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "AppDelegate.h"
#import "Render3DViewController.h"
#import "SceneWrapper.h"

#include "IVDA/iOS.h"
#include "Scene/RenderDispatcher.h"
#include "Scene/ScreenInfo.h"
#include "Scene/Scene.h"

@implementation Render3DViewController

@synthesize context = _context;

- (id)init {
    self = [super init];
    m_scene = nullptr;
    m_rendererDispatcher = nullptr;
    m_uiBuilder = nullptr;
    return self;
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
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reloadScene:) name:@"NewSceneLoaded" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updateDatasets:) name:@"DatasetChanged" object:nil];
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    m_rendererDispatcher = std::make_unique<RenderDispatcher>([self screenInfo]);
}

- (void) viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    [m_uiBuilder layoutUI];
}

-(void)reloadScene:(NSNotification*)notification
{
    SceneWrapper* sceneWrapper = notification.userInfo[@"scene"];
    m_scene = [sceneWrapper scene];
    m_scene->updateDatasets();
    auto manipulators = m_scene->manipulators();
    if (!manipulators.first.empty() || !manipulators.second.empty()) {
        m_uiBuilder = [[DynamicUIBuilder alloc] initWitView:self.view andFloatManipulators:manipulators.first andEnumManipulators:manipulators.second];
    }
}

-(void)updateDatasets:(NSNotification*)notification
{
    m_scene->updateDatasets();
}


// Drawing
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if (m_scene) {
        GLMatrix modelView = m_scene->modelViewMatrix();
        m_rendererDispatcher->setModelView(&modelView);
        m_scene->render(*m_rendererDispatcher);
    }
    [view bindDrawable];
}

// Interaction
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];
    
    NSUInteger numTouches = [[event allTouches] count];
    NSArray* allTouches = [[event allTouches] allObjects];
    if (numTouches == 2) {
        CGPoint touchPoint1 = [[allTouches objectAtIndex:0] locationInView:self.view];
        CGPoint touchPoint2 = [[allTouches objectAtIndex:1] locationInView:self.view];
        m_touchPos1 = IVDA::Vec2f(touchPoint1.x/self.view.frame.size.width,
                                  touchPoint1.y/self.view.frame.size.height);
        m_touchPos2 = IVDA::Vec2f(touchPoint2.x/self.view.frame.size.width,
                                  touchPoint2.y/self.view.frame.size.height);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    UITouch* touch = [[event touchesForView:self.view] anyObject];
    CGPoint pos = [touch locationInView:self.view];
    CGPoint prev = [touch previousLocationInView:self.view];
    NSUInteger numTouches = [[event allTouches] count];
    
    if (pos.x == prev.x && pos.y == prev.y && numTouches == 1) {
        return;
    }
    
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    
    NSArray* allTouches = [[event allTouches] allObjects];
    if (numTouches == 2) {
        CGPoint touchPoint1 = [[allTouches objectAtIndex:0] locationInView:self.view];
        CGPoint touchPoint2 = [[allTouches objectAtIndex:1] locationInView:self.view];
        
        IVDA::Vec2f touchPos1(touchPoint1.x/self.view.frame.size.width,
                              touchPoint1.y/self.view.frame.size.height);
        IVDA::Vec2f touchPos2(touchPoint2.x/self.view.frame.size.width,
                              touchPoint2.y/self.view.frame.size.height);
        
        [self translateSceneWith:touchPos1 andWith:touchPos2];
        
        m_touchPos1 = touchPos1;
        m_touchPos2 = touchPos2;
    }
}

- (void) translateSceneWith:(const IVDA::Vec2f&)touchPos1 andWith:(const IVDA::Vec2f&)touchPos2 {
    IVDA::Vec2f c1((m_touchPos1.x + m_touchPos2.x) / 2, (m_touchPos1.y + m_touchPos2.y) / 2);
    IVDA::Vec2f c2((touchPos1.x + touchPos2.x) / 2, (touchPos1.y + touchPos2.y) / 2);
    IVDA::Vec2f translation(c2.x - c1.x, -(c2.y - c1.y));
    m_scene->addTranslation(translation);
}

@end
