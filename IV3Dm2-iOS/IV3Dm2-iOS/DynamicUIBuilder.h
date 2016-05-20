//  Created by David McCann on 5/9/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "Scene/DataProvider.h"
#include "Scene/Scene.h"

#include <vector>

@interface WidgetGroup : NSObject

+(UILabel*) createLabelWithText:(NSString*)text;
+(void) layoutNameLabel:(UILabel*)nameLabel andStepper:(UIStepper*)stepper andValueLabel:(UILabel*)valueLabel atIndex:(int)index;

@end

@interface FloatWidgetGroup : NSObject
{
@private
    NSString* m_objectName;
    InputVariableFloat::Info m_variableInfo;
    UILabel* m_nameLabel;
    UIStepper* m_stepper;
    UILabel* m_valueLabel;
}

-(id) initWithObjectName:(NSString*)objectName andVariableInfo:(const InputVariableFloat::Info&)variableInfo andView:(UIView*)view;
-(void) layout;

@end

@interface EnumWidgetGroup : NSObject
{
@private
    NSString* m_objectName;
    InputVariableEnum::Info m_variableInfo;
    UILabel* m_nameLabel;
    UIStepper* m_stepper;
    UILabel* m_valueLabel;
}

-(id) initWithObjectName:(NSString*)objectName andVariableInfo:(const InputVariableEnum::Info&)variableInfo andView:(UIView*)view;
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