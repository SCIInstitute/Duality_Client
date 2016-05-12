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
    m_nameLabels = [[NSMutableArray<UILabel*> alloc] init];
    m_valueLabels = [[NSMutableArray<UILabel*> alloc] init];
    m_steppers = [[NSMutableArray<UIStepper*> alloc] init];
    return self;
}

-(void) generateUI
{
    [self clearUI];
    for (const auto& floatParam : m_floatParams) {
        UILabel* nameLabel = [self createNameLabel:floatParam];
        [m_nameLabels addObject:nameLabel];
        [m_view addSubview:nameLabel];
        
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

    for (int i = 0; i < [m_nameLabels count]; ++i) {
        CGFloat top = topMargin + i * (height + spacing);
        m_nameLabels[i].frame = CGRectMake(leftMargin, top, labelWidth, height);
    }
    
    for (int i = 0; i < [m_steppers count]; ++i) {
        CGFloat top = topMargin + i * (height + spacing);
        m_steppers[i].frame = CGRectMake(leftMargin + labelWidth, top, buttonWidth, height);
    }
}

- (void) clearUI
{
    for (UILabel* label in m_nameLabels) {
        [label removeFromSuperview];
    }
    [m_nameLabels removeAllObjects];
    
    for (UIStepper* stepper in m_steppers) {
        [stepper removeFromSuperview];
    }
    [m_steppers removeAllObjects];
}

-(UILabel*) createNameLabel:(const InputParameterFloat&)floatParam
{
    UILabel* label = [[UILabel alloc] init];
    label.text = [NSString stringWithUTF8String:floatParam.name.c_str()];
    label.textColor = [UIColor whiteColor];
    label.backgroundColor = [UIColor colorWithWhite:0 alpha:0.8];
    label.textAlignment = NSTextAlignmentCenter;
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

