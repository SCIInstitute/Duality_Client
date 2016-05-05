//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "SelectSceneViewController.h"

#include "SceneDefinitionProvider.h"

@interface SelectSceneViewController ()

@end

@implementation SelectSceneViewController

- (id)initWithSceneDefinitionProvider:(SceneDefinitionProvider*)provider {
    self = [super initWithStyle:UITableViewStyleGrouped];
    m_definitions = provider->fetchDefinitions();
    return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return m_definitions.size();
}

- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString * CellIdentifier = @"Cell";
    UITableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    const SceneDefinition& definition = m_definitions[indexPath.row];
    cell.textLabel.text = [NSString stringWithUTF8String:definition.name().c_str()];
    
    return cell;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"Select Scene";
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
