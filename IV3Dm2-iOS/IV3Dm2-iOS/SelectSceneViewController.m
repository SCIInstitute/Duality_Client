//
//  FirstViewController.m
//  IV3Dm2-iOS
//
//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "SelectSceneViewController.h"

@interface SelectSceneViewController ()

@end

@implementation SelectSceneViewController

- (id) init {
    self = [super initWithStyle:UITableViewStyleGrouped];
    return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 50;
}

- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString * CellIdentifier = @"Cell";
    UITableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    cell.textLabel.text = @"All work and no play makes David a dull boy";
    
    return cell;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"Hello World";
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
