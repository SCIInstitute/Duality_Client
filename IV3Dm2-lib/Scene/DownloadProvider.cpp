//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "DownloadProvider.h"

#include "Communication/DatasetProvider.h"

DownloadProvider::DownloadProvider(std::shared_ptr<ServerAdapter> server, std::string path)
    : m_server(server)
    , m_path(std::move(path)) {}

std::shared_ptr<std::vector<uint8_t>> DownloadProvider::fetch() {
    m_server->download(m_path);
}

std::string DownloadProvider::path() const {
    return m_path;
}
