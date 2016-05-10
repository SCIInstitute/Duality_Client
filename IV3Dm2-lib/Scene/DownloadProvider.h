//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Communication/ServerAdapter.h"
#include "Communication/DataProvider.h"

#include <string>
#include <memory>

class DownloadProvider : public DataProvider {
public:
    DownloadProvider(std::shared_ptr<ServerAdapter> server, std::string path);

    // DataProvider interface
    std::shared_ptr<std::vector<uint8_t>> fetch() override;

    std::string path() const;

private:
    std::shared_ptr<ServerAdapter> m_server;
    std::string m_path;
};