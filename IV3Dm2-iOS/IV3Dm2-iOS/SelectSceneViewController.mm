//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "SelectSceneViewController.h"
#import "AppDelegate.h"

@interface SelectSceneViewController ()

@end

@implementation SelectSceneViewController

- (id)initWithSceneProvider:(std::shared_ptr<SceneProvider>)provider {
    self = [super initWithStyle:UITableViewStyleGrouped];
    m_provider = provider;
    return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return m_provider->listMetadata().size();
}

- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString * CellIdentifier = @"Cell";
    UITableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier];
    }
    
    const SceneMetadata& meta = m_provider->listMetadata()[indexPath.row];
    cell.textLabel.text = [NSString stringWithUTF8String:meta.name().c_str()];
    cell.detailTextLabel.text = [NSString stringWithUTF8String:meta.description().c_str()];
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    auto name = m_provider->listMetadata()[indexPath.row].name();
    Scene* scene = m_provider->getScene(name).release(); // FIXME: ugly
    AppDelegate* app = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    [app changeScene:scene];
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
