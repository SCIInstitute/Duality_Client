//
//  TabBarViewController.h
//  IV3Dm2-iOS
//
//  Created by David McCann on 5/4/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import "SelectSceneViewController.h"

#import <UIKit/UIKit.h>

@interface TabBarViewController : UITabBarController {
@private
    SelectSceneViewController* m_selectSceneViewController;
}

- (id)init;

@end
