//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Communication/DatasetProvider.h"
#include "DownloadNode.h"

DownloadNode::DownloadNode(std::string path)
    : m_path(std::move(path)) {}

DownloadNode::DownloadNode(const DownloadNode& other)
    : m_path(other.m_path)
    , m_dataset(other.m_dataset ? other.m_dataset->clone() : nullptr) {}

void DownloadNode::updateDataset(const DatasetProvider& datasetProvider) {
    if (m_dataset == nullptr) {
        m_dataset = datasetProvider.downloadDataset(m_path);
    }
}

std::unique_ptr<SceneNode> DownloadNode::clone() const {
    return std::make_unique<DownloadNode>(*this);
}

std::string DownloadNode::path() const {
    return m_path;
}
