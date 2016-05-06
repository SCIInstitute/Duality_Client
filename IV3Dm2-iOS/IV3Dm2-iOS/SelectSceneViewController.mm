//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "SelectSceneViewController.h"

#include "SceneProvider.h"

@interface SelectSceneViewController ()

@end

@implementation SelectSceneViewController

- (id)initWithSceneProvider:(SceneProvider*)provider {
    self = [super initWithStyle:UITableViewStyleGrouped];
    m_scenes = provider->listScenes();
    return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return m_scenes.size();
}

- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString * CellIdentifier = @"Cell";
    UITableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier];
    }
    
    SceneMetadata meta = m_scenes[indexPath.row].metadata();
    cell.textLabel.text = [NSString stringWithUTF8String:meta.name().c_str()];
    cell.detailTextLabel.text = [NSString stringWithUTF8String:meta.description().c_str()];
    
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
