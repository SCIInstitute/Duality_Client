//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "Scene/InputVariable.h"
#include "Scene/Scene.h"

#include <vector>

@interface WidgetGroup : NSObject

+(UILabel*) createLabelWithText:(NSString*)text;
+(void) layoutNameLabel:(UILabel*)nameLabel andStepper:(UIStepper*)stepper andValueLabel:(UILabel*)valueLabel atIndex:(int)index;

@end

@interface FloatWidgetGroup : NSObject
{
@private
    InputVariableFloat* m_variable;
    UILabel* m_nameLabel;
    UIStepper* m_stepper;
    UILabel* m_valueLabel;
}

-(id) initWithVariable:(InputVariableFloat*)variable andView:(UIView*)view;
-(void) layout;

@end

@interface EnumWidgetGroup : NSObject
{
@private
    InputVariableEnum* m_variable;
    UILabel* m_nameLabel;
    UIStepper* m_stepper;
    UILabel* m_valueLabel;
}

-(id) initWithVariable:(InputVariableEnum*)variable andView:(UIView*)view;
-(void) layout;

@end


@interface DynamicUIBuilder : NSObject
{
@private
    NSMutableArray<FloatWidgetGroup*>* m_floatWidgetGroups;
    NSMutableArray<EnumWidgetGroup*>* m_enumWidgetGroups;
}

-(id) initWitView:(UIView*)view andVariableMap:(Scene::VariableMap)variableMap;
-(void) layoutUI;

@end