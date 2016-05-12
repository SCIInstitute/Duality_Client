//  Created by David McCann on 5/12/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "DynamicUIBuilder.h"

@implementation DynamicUIBuilder

-(id) initWitView:(UIView*)view andFloatParams:(const std::vector<InputParameterFloat>&)floatParams
{
    m_view = view;
    m_floatParams = floatParams;
    m_labels = [[NSMutableArray<UILabel*> alloc] init];
    m_steppers = [[NSMutableArray<UIStepper*> alloc] init];
    return self;
}

-(void) generateUI
{
    [self clearUI];
    for (const auto& floatParam : m_floatParams) {
        UILabel* label = [self createLabel:floatParam];
        [m_labels addObject:label];
        [m_view addSubview:label];
        
        UIStepper* stepper = [self createStepper:floatParam];
        [m_steppers addObject:stepper];
        [m_view addSubview:stepper];
    }
}

-(void) layoutUI
{
    CGFloat const topMargin = 50;
    CGFloat const leftMargin = 60;
    CGFloat const height = 40;
    CGFloat const spacing = 2;
    
    CGFloat const buttonWidth = 120;
    CGFloat const labelWidth = 120;

    for (int i = 0; i < [m_labels count]; ++i) {
        CGFloat top = topMargin + i * (height + spacing);
        m_labels[i].frame = CGRectMake(leftMargin, top, labelWidth, height);
    }
    
    for (int i = 0; i < [m_steppers count]; ++i) {
        CGFloat top = topMargin + i * (height + spacing);
        m_steppers[i].frame = CGRectMake(leftMargin + labelWidth, top, buttonWidth, height);
    }
}

- (void) clearUI
{
    for (UILabel* label in m_labels) {
        [label removeFromSuperview];
    }
    [m_labels removeAllObjects];
    
    for (UIStepper* stepper in m_steppers) {
        [stepper removeFromSuperview];
    }
    [m_steppers removeAllObjects];
}

-(UILabel*) createLabel:(const InputParameterFloat&)floatParam
{
    UILabel* label = [[UILabel alloc] init];
    label.text = [NSString stringWithUTF8String:floatParam.name.c_str()];
    [label setTextColor:[UIColor whiteColor]];
    [label setBackgroundColor:[UIColor colorWithWhite:0 alpha:0.8]];
    [label setTextAlignment:NSTextAlignmentCenter];
    return label;
}

-(UIStepper*) createStepper:(const InputParameterFloat&)floatParam
{
    UIStepper* stepper = [[UIStepper alloc] init];
    stepper.minimumValue = floatParam.lowerBound;
    stepper.maximumValue = floatParam.upperBound;
    stepper.stepValue = floatParam.stepSize;
    stepper.value = floatParam.defaultValue;
    return stepper;
}


@end

