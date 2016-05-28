//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include <sstream>

class Error : public std::runtime_error {
public:
    Error(const std::string& msg, const std::string& file, int line)
        : std::runtime_error("")
        , m_msg(msg)
        , m_file(file)
        , m_line(line) {
        std::stringstream str;
        str << m_msg << " (" << m_file << ":" << std::to_string(m_line) << ")";
        m_msg = str.str();
    }

    const char* what() const noexcept override { return m_msg.c_str(); }

private:
    std::string m_msg;
    std::string m_file;
    int m_line;
};