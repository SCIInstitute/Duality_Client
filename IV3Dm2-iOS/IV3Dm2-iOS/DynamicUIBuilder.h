//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "Scene/InputParameter.h"

#include <vector>

@interface DynamicUIBuilder : NSObject
{
@protected
    UIView* m_view;
    std::vector<InputParameterFloat> m_floatParams;
    NSMutableArray<UILabel*>* m_labels;
    NSMutableArray<UIStepper*>* m_steppers;
}

-(id) initWitView:(UIView*)view andFloatParams:(const std::vector<InputParameterFloat>&)floatParams;
-(void) generateUI;
-(void) layoutUI;

@end