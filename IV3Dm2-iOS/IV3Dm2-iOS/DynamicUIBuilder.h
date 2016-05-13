//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "Scene/InputParameter.h"
#include "Scene/ParameterManipulator.h"

#include <vector>

@interface DynamicUIBuilder : NSObject
{
@protected
    UIView* m_view;
    std::vector<ParameterManipulator*> m_manipulators;
    NSMutableArray<UILabel*>* m_nameLabels;
    NSMutableArray<UILabel*>* m_valueLabels;
    NSMutableArray<UIStepper*>* m_steppers;
}

-(id) initWitView:(UIView*)view andParameterManipulators:(std::vector<ParameterManipulator*>)manipulators;
-(void) generateUI;
-(void) layoutUI;

@end