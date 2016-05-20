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
    std::unique_ptr<DataProvider::InputSetter<InputVariableFloat>> m_setter; // unique_ptr because InputSetter does not have default ctor
    UILabel* m_nameLabel;
    UIStepper* m_stepper;
    UILabel* m_valueLabel;
}

-(id) initWithSetter:(const DataProvider::InputSetter<InputVariableFloat>&)setter andView:(UIView*)view;
-(void) layout;

@end

@interface EnumWidgetGroup : NSObject
{
@private
    std::unique_ptr<DataProvider::InputSetter<InputVariableEnum>> m_setter; // unique_ptr because InputSetter does not have default ctor
    UILabel* m_nameLabel;
    UIStepper* m_stepper;
    UILabel* m_valueLabel;
}

-(id) initWithSetter:(const DataProvider::InputSetter<InputVariableEnum>&)setter andView:(UIView*)view;
-(void) layout;

@end


@interface DynamicUIBuilder : NSObject
{
@private
    NSMutableArray<FloatWidgetGroup*>* m_floatWidgetGroups;
    NSMutableArray<EnumWidgetGroup*>* m_enumWidgetGroups;
}

-(id) initWitView:(UIView*)view andSetterMap:(Scene::VariableSetterMap)setterMap;
-(void) layoutUI;

@end