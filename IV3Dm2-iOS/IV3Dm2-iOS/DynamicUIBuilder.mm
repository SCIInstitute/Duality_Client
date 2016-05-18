//  Created by David McCann on 5/12/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "DynamicUIBuilder.h"

@implementation WidgetGroup

+(UILabel*) createLabelWithText:(NSString*)text
{
    UILabel* label = [[UILabel alloc] init];
    label.text = text;
    label.textColor = [UIColor whiteColor];
    label.backgroundColor = [UIColor colorWithWhite:0 alpha:0];
    return label;
}

+(void) layoutNameLabel:(UILabel*)nameLabel andStepper:(UIStepper*)stepper andValueLabel:(UILabel*)valueLabel atIndex:(int)index
{
    CGFloat const topMargin = 50;
    CGFloat const leftMargin = 60;
    CGFloat const height = 40;
    CGFloat const spacing = 2;
    
    CGFloat const stepperWidth = 120;
    CGFloat const nameLabelWidth = 240;
    CGFloat const valueLabelWidth = 80;
    
    CGFloat top = topMargin + index * (height + spacing);
    nameLabel.frame = CGRectMake(leftMargin, top, nameLabelWidth, height);
    stepper.frame = CGRectMake(leftMargin + nameLabelWidth, top, stepperWidth, height);
    valueLabel.frame = CGRectMake(leftMargin + nameLabelWidth + stepperWidth, top, valueLabelWidth, height);
}

@end

@implementation FloatWidgetGroup

-(id) initWithVariable:(InputVariableFloat*)variable andView:(UIView *)view
{
    self = [super init];
    
    m_variable = variable;
 
    auto info = m_variable->info();
    
    m_nameLabel = [WidgetGroup createLabelWithText:[NSString stringWithUTF8String:info.name.c_str()]];
    m_nameLabel.textAlignment = NSTextAlignmentLeft;
    [view addSubview:m_nameLabel];
    
    m_stepper = [[UIStepper alloc] init];
    m_stepper.minimumValue = info.lowerBound;
    m_stepper.maximumValue = info.upperBound;
    m_stepper.stepValue = info.stepSize;
    m_stepper.value = info.defaultValue;
    [view addSubview:m_stepper];
    [m_stepper addTarget:self action:@selector(stepToValue:) forControlEvents:UIControlEventValueChanged];
    
    m_valueLabel = [WidgetGroup createLabelWithText:@""];
    m_valueLabel.textAlignment = NSTextAlignmentRight;
    [view addSubview:m_valueLabel];
    
    m_valueLabel.text = [NSString stringWithFormat:@"%.02f", info.defaultValue];
    
    return self;
}

-(void) layout
{
    int index = m_variable->info().index;
    [WidgetGroup layoutNameLabel:m_nameLabel andStepper:m_stepper andValueLabel:m_valueLabel atIndex:index];
}

- (void)stepToValue:(UIStepper*)stepper
{
    m_valueLabel.text = [NSString stringWithFormat:@"%.02f", stepper.value];
    m_variable->setValue(stepper.value);
    [[NSNotificationCenter defaultCenter] postNotificationName:@"DatasetChanged" object:self];
}

- (void) dealloc
{
    [m_nameLabel removeFromSuperview];
    [m_stepper removeFromSuperview];
    [m_valueLabel removeFromSuperview];
}

@end


@implementation EnumWidgetGroup

-(id) initWithVariable:(InputVariableEnum *)variable andView:(UIView *)view
{
    self = [super init];
    
    m_variable = variable;
    
    auto info = m_variable->info();
    
    m_nameLabel = [WidgetGroup createLabelWithText:[NSString stringWithUTF8String:info.name.c_str()]];
    m_nameLabel.textAlignment = NSTextAlignmentLeft;
    [view addSubview:m_nameLabel];
    
    m_stepper = [[UIStepper alloc] init];
    m_stepper.minimumValue = 0;
    m_stepper.maximumValue = info.values.size() - 1;
    m_stepper.stepValue = 1;
    auto findIt = std::find(begin(info.values), end(info.values), info.defaultValue);
    m_stepper.value = std::distance(begin(info.values), findIt);
    [view addSubview:m_stepper];
    [m_stepper addTarget:self action:@selector(stepToValue:) forControlEvents:UIControlEventValueChanged];
    
    m_valueLabel = [WidgetGroup createLabelWithText:@""];
    m_valueLabel.textAlignment = NSTextAlignmentRight;
    [view addSubview:m_valueLabel];
    
    m_valueLabel.text = [NSString stringWithUTF8String:info.defaultValue.c_str()];
    
    return self;
}

-(void) layout
{
    int index = m_variable->info().index;
    [WidgetGroup layoutNameLabel:m_nameLabel andStepper:m_stepper andValueLabel:m_valueLabel atIndex:index];
}

- (void)stepToValue:(UIStepper*)stepper
{
    auto info = m_variable->info();
    std::string value = info.values[(int)stepper.value];
    m_valueLabel.text = [NSString stringWithUTF8String:value.c_str()];
    m_variable->setValue(value);
    [[NSNotificationCenter defaultCenter] postNotificationName:@"DatasetChanged" object:self];
}

- (void) dealloc
{
    [m_nameLabel removeFromSuperview];
    [m_stepper removeFromSuperview];
    [m_valueLabel removeFromSuperview];
}

@end


@implementation DynamicUIBuilder

-(id) initWitView:(UIView*)view andVariableMap:(Scene::VariableMap)variableMap
{
    m_floatWidgetGroups = [[NSMutableArray<FloatWidgetGroup*> alloc] init];
    for (const auto& variable : variableMap["dataset3"].floatVariables) { // FIXME
        FloatWidgetGroup* widgetGroup = [[FloatWidgetGroup alloc] initWithVariable:variable andView:view];
        [m_floatWidgetGroups addObject:widgetGroup];
    }

    m_enumWidgetGroups = [[NSMutableArray<EnumWidgetGroup*> alloc] init];
    for (const auto& variable : variableMap["dataset3"].enumVariables) { // FIXME
        EnumWidgetGroup* widgetGroup = [[EnumWidgetGroup alloc] initWithVariable:variable andView:view];
        [m_enumWidgetGroups addObject:widgetGroup];
    }
    
    return self;
}

-(void) layoutUI
{
    for (FloatWidgetGroup* widgetGroup in m_floatWidgetGroups) {
        [widgetGroup layout];
    }

    for (EnumWidgetGroup* widgetGroup in m_enumWidgetGroups) {
        [widgetGroup layout];
    }
}

- (void) dealloc
{
    [m_floatWidgetGroups removeAllObjects];
    [m_enumWidgetGroups removeAllObjects];
}

@end

