//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "Scene/InputParameter.h"
#include "Scene/ParameterManipulator.h"

#include <vector>

@interface WidgetGroup : NSObject

+(UILabel*) createLabelWithText:(NSString*)text;
+(void) layoutNameLabel:(UILabel*)nameLabel andStepper:(UIStepper*)stepper andValueLabel:(UILabel*)valueLabel atIndex:(int)index;

@end

@interface FloatWidgetGroup : NSObject
{
@private
    std::unique_ptr<ParameterManipulatorFloat> m_manipulator;
    UILabel* m_nameLabel;
    UIStepper* m_stepper;
    UILabel* m_valueLabel;
}

-(id) initWithManipulator:(ParameterManipulatorFloat)manipulator andView:(UIView*)view;
-(void) layout;

@end

@interface EnumWidgetGroup : NSObject
{
@private
    std::unique_ptr<ParameterManipulatorEnum> m_manipulator;
    UILabel* m_nameLabel;
    UIStepper* m_stepper;
    UILabel* m_valueLabel;
}

-(id) initWithManipulator:(ParameterManipulatorEnum)manipulator andView:(UIView*)view;
-(void) layout;

@end


@interface DynamicUIBuilder : NSObject
{
@private
    NSMutableArray<FloatWidgetGroup*>* m_floatWidgetGroups;
    NSMutableArray<EnumWidgetGroup*>* m_enumWidgetGroups;
}

-(id) initWitView:(UIView*)view
    andFloatManipulators:(const std::vector<ParameterManipulatorFloat>&)manipulators
    andEnumManipulators:(const std::vector<ParameterManipulatorEnum>&)manipulators;
-(void) layoutUI;

@end