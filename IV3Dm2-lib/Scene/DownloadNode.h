//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "SceneNode.h"

#include <string>
#include <memory>

class Dataset;

class DownloadNode : public SceneNode {
public:
    DownloadNode(std::string path);
    DownloadNode(const DownloadNode& other);

    void updateDataset(const DatasetProvider& datasetProvider) override;

    std::unique_ptr<SceneNode> clone() const override;

    std::string path() const;

private:
    std::string m_path;
    std::unique_ptr<Dataset> m_dataset;
};