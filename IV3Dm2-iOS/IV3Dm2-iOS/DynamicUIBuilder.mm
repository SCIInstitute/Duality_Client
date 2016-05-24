//  Created by David McCann on 5/12/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "DynamicUIBuilder.h"

#include "Scene/InputVariable.h"

@interface FloatStepper : UIStepper
{
@private
    UILabel* m_label;
    NSString* m_objectName;
    InputVariableFloat::Info m_info;
}
-(id) initWithLabel:(UILabel*)label andObjectName:(NSString*)objectName andInfo:(const InputVariableFloat::Info&)info;
-(void) stepToValue:(FloatStepper*)stepper;
@end

@implementation FloatStepper

-(id) initWithLabel:(UILabel *)label andObjectName:(NSString *)objectName andInfo:(const InputVariableFloat::Info &)info
{
    self = [super init];
    m_label = label;
    m_objectName = objectName;
    m_info = info;
    [self addTarget:self action:@selector(stepToValue:) forControlEvents:UIControlEventValueChanged];
    return self;
}

-(void)stepToValue:(FloatStepper *)stepper
{
    m_label.text = [NSString stringWithFormat:@"%.02f", stepper.value];
    NSMutableDictionary* changeData = [[NSMutableDictionary alloc] init];
    [changeData setValue:m_objectName forKey:@"objectName"];
    [changeData setValue:[NSString stringWithUTF8String:m_info.name.c_str()] forKey:@"variableName"];
    [changeData setValue:[NSNumber numberWithFloat:self.value] forKey:@"value"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"DatasetChanged" object:self userInfo:changeData];
}

@end

UIStackView* buildFloatVariableStackView(const std::string& objectName, const InputVariableFloat::Info& info)
{
    UIStackView* stackView = [[UIStackView alloc] init];
    stackView.axis = UILayoutConstraintAxisHorizontal;
    stackView.distribution = UIStackViewDistributionEqualSpacing;
    stackView.alignment = UIStackViewAlignmentBottom;
    stackView.spacing = 10;
    
    UILabel* nameLabel = [[UILabel alloc] init];
    nameLabel.text = [NSString stringWithUTF8String:info.name.c_str()];
    nameLabel.textColor = [UIColor whiteColor];
    nameLabel.backgroundColor = [UIColor colorWithWhite:0 alpha:0];
    [nameLabel.widthAnchor constraintEqualToConstant:200.0].active = true;
    [stackView addArrangedSubview:nameLabel];
    
    UILabel* valueLabel = [[UILabel alloc] init];
    valueLabel.text = [NSString stringWithFormat:@"%.02f", info.defaultValue];
    valueLabel.textColor = [UIColor whiteColor];
    valueLabel.backgroundColor = [UIColor colorWithWhite:0 alpha:0];
    [valueLabel.widthAnchor constraintEqualToConstant:100.0].active = true;
    [stackView addArrangedSubview:valueLabel];
    
    FloatStepper* stepper = [[FloatStepper alloc] initWithLabel:valueLabel
                                                  andObjectName:[NSString stringWithUTF8String:objectName.c_str()]
                                                  andInfo:info];
    stepper.minimumValue = info.lowerBound;
    stepper.maximumValue = info.upperBound;
    stepper.stepValue = info.stepSize;
    stepper.value = info.defaultValue;
    [stackView addArrangedSubview:stepper];

    return stackView;
}

UIStackView* buildObjectStackView(const std::string& name, const Scene::VariableInfos& infos)
{
    UIStackView* stackView = [[UIStackView alloc] init];
    stackView.axis = UILayoutConstraintAxisVertical;
    stackView.distribution = UIStackViewDistributionEqualSpacing;
    stackView.alignment = UIStackViewAlignmentLeading;
    stackView.spacing = 5;
    
    UILabel* objectLabel = [[UILabel alloc] init];
    objectLabel.text = [NSString stringWithUTF8String:name.c_str()];
    objectLabel.textColor = [UIColor whiteColor];
    objectLabel.backgroundColor = [UIColor colorWithWhite:0 alpha:0];
    [stackView addArrangedSubview:objectLabel];
    
    for (const auto& info : infos.floatInfos) {
        UIStackView* variableStackView = buildFloatVariableStackView(name, info);
        variableStackView.translatesAutoresizingMaskIntoConstraints = false;
        [stackView addArrangedSubview:variableStackView];
    }
    return stackView;
}

UIStackView* buildStackViewFromVariableMap(const Scene::VariableMap& variableMap)
{
    UIStackView* stackView = [[UIStackView alloc] init];
    stackView.axis = UILayoutConstraintAxisVertical;
    stackView.distribution = UIStackViewDistributionEqualSpacing;
    stackView.alignment = UIStackViewAlignmentLeading;
    stackView.spacing = 50;
    
    for (const auto& object : variableMap) {
        if (!object.second.floatInfos.empty()) {
            UIStackView* objectStackView = buildObjectStackView(object.first, object.second);
            objectStackView.translatesAutoresizingMaskIntoConstraints = false;
            [stackView addArrangedSubview:objectStackView];
        }
    }
    return stackView;
}
