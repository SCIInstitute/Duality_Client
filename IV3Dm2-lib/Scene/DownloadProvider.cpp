//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/DownloadProvider.h"

#include "Scene/DataProviderDispatcher.h"

#include <cassert>

DownloadProvider::DownloadProvider(const ServerAdapter* server, std::string path)
    : m_server(server)
    , m_path(std::move(path)) {}

void DownloadProvider::accept(DataProviderDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

std::shared_ptr<std::vector<uint8_t>> DownloadProvider::download() const {
    assert(m_server != nullptr);
    return m_server->download(m_path);
}

std::string DownloadProvider::path() const {
    return m_path;
}
