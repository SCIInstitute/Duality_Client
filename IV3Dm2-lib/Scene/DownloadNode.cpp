//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "DownloadNode.h"
#include "DatasetProvider.h"

DownloadNode::DownloadNode(std::string path)
    : m_path(std::move(path)) {}

void DownloadNode::updateDataset(const DatasetProvider& datasetProvider)
{
    if (m_dataset == nullptr) {
        m_dataset = datasetProvider.downloadDataset(m_path);
    }
}

std::string DownloadNode::path() const {
    return m_path;
}
