//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/ServerAdapter.h"
#include "Scene/DataProvider.h"

#include <string>
#include <memory>
#include <vector>

class DownloadProvider : public DataProvider {
public:
    DownloadProvider(const ServerAdapter* server, std::string path);

    // DataProvider interface
    std::shared_ptr<std::vector<uint8_t>> fetch() override;

    std::string path() const;

private:
    const ServerAdapter* m_server;
    std::string m_path;
};