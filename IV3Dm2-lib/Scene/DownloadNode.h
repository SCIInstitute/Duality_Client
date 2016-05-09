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

    void updateDatasets(const DatasetProvider& datasetProvider) override;
    void render(AbstractRenderer& renderer) const;

    std::unique_ptr<SceneNode> clone() const override;

    std::string path() const;

private:
    std::string m_path;
    std::shared_ptr<Dataset> m_dataset;
};