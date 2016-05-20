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

-(id) initWithObjectName:(NSString *)objectName andVariableInfo:(const InputVariableFloat::Info &)variableInfo andView:(UIView *)view
{
    self = [super init];
 
    m_objectName = objectName;
    m_variableInfo = variableInfo;
    
    m_nameLabel = [WidgetGroup createLabelWithText:[NSString stringWithUTF8String:m_variableInfo.name.c_str()]];
    m_nameLabel.textAlignment = NSTextAlignmentLeft;
    [view addSubview:m_nameLabel];
    
    m_stepper = [[UIStepper alloc] init];
    m_stepper.minimumValue = m_variableInfo.lowerBound;
    m_stepper.maximumValue = m_variableInfo.upperBound;
    m_stepper.stepValue = m_variableInfo.stepSize;
    m_stepper.value = m_variableInfo.defaultValue;
    [view addSubview:m_stepper];
    [m_stepper addTarget:self action:@selector(stepToValue:) forControlEvents:UIControlEventValueChanged];
    
    m_valueLabel = [WidgetGroup createLabelWithText:@""];
    m_valueLabel.textAlignment = NSTextAlignmentRight;
    [view addSubview:m_valueLabel];
    
    m_valueLabel.text = [NSString stringWithFormat:@"%.02f", m_variableInfo.defaultValue];
    
    return self;
}

-(void) layout
{
    int index = m_variableInfo.index;
    [WidgetGroup layoutNameLabel:m_nameLabel andStepper:m_stepper andValueLabel:m_valueLabel atIndex:index];
}

- (void)stepToValue:(UIStepper*)stepper
{
    m_valueLabel.text = [NSString stringWithFormat:@"%.02f", stepper.value];
    NSMutableDictionary* changeData = [[NSMutableDictionary alloc] init];
    [changeData setValue:m_objectName forKey:@"objectName"];
    [changeData setValue:[NSString stringWithUTF8String:m_variableInfo.name.c_str()] forKey:@"variableName"];
    [changeData setValue:[NSNumber numberWithFloat:m_stepper.value] forKey:@"value"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"DatasetChanged" object:self userInfo:changeData];
}

- (void) dealloc
{
    [m_nameLabel removeFromSuperview];
    [m_stepper removeFromSuperview];
    [m_valueLabel removeFromSuperview];
}

@end


@implementation EnumWidgetGroup

-(id) initWithObjectName:(NSString *)objectName andVariableInfo:(const InputVariableEnum::Info &)variableInfo andView:(UIView *)view
{
    self = [super init];
    
    m_objectName = objectName;
    m_variableInfo = variableInfo;
    
    m_nameLabel = [WidgetGroup createLabelWithText:[NSString stringWithUTF8String:m_variableInfo.name.c_str()]];
    m_nameLabel.textAlignment = NSTextAlignmentLeft;
    [view addSubview:m_nameLabel];
    
    m_stepper = [[UIStepper alloc] init];
    m_stepper.minimumValue = 0;
    m_stepper.maximumValue = m_variableInfo.values.size() - 1;
    m_stepper.stepValue = 1;
    auto findIt = std::find(begin(m_variableInfo.values), end(m_variableInfo.values), m_variableInfo.defaultValue);
    m_stepper.value = std::distance(begin(m_variableInfo.values), findIt);
    [view addSubview:m_stepper];
    [m_stepper addTarget:self action:@selector(stepToValue:) forControlEvents:UIControlEventValueChanged];
    
    m_valueLabel = [WidgetGroup createLabelWithText:@""];
    m_valueLabel.textAlignment = NSTextAlignmentRight;
    [view addSubview:m_valueLabel];
    
    m_valueLabel.text = [NSString stringWithUTF8String:m_variableInfo.defaultValue.c_str()];
    
    return self;
}

-(void) layout
{
    int index = m_variableInfo.index;
    [WidgetGroup layoutNameLabel:m_nameLabel andStepper:m_stepper andValueLabel:m_valueLabel atIndex:index];
}

- (void)stepToValue:(UIStepper*)stepper
{
    std::string value = m_variableInfo.values[(int)stepper.value];
    m_valueLabel.text = [NSString stringWithUTF8String:value.c_str()];
    NSMutableDictionary* changeData = [[NSMutableDictionary alloc] init];
    [changeData setValue:m_objectName forKey:@"objectName"];
    [changeData setValue:[NSString stringWithUTF8String:m_variableInfo.name.c_str()] forKey:@"variableName"];
    [changeData setValue:[NSString stringWithUTF8String:m_variableInfo.values[m_stepper.value].c_str()] forKey:@"value"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"DatasetChanged" object:self userInfo:changeData];
}

- (void) dealloc
{
    [m_nameLabel removeFromSuperview];
    [m_stepper removeFromSuperview];
    [m_valueLabel removeFromSuperview];
}

@end


@implementation DynamicUIBuilder

-(id) initWitView:(UIView *)view andVariableMap:(Scene::VariableMap)variableMap
{
    m_floatWidgetGroups = [[NSMutableArray<FloatWidgetGroup*> alloc] init];
    for (const auto& varInfo : variableMap["dataset3"].floatInfos) { // FIXME
        FloatWidgetGroup* widgetGroup = [[FloatWidgetGroup alloc] initWithObjectName:@"dataset3" andVariableInfo:varInfo andView:view];
        [m_floatWidgetGroups addObject:widgetGroup];
    }

    m_enumWidgetGroups = [[NSMutableArray<EnumWidgetGroup*> alloc] init];
    for (const auto& varInfo : variableMap["dataset3"].enumInfos) { // FIXME
        EnumWidgetGroup* widgetGroup = [[EnumWidgetGroup alloc] initWithObjectName:@"dataset3" andVariableInfo:varInfo andView:view];
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

