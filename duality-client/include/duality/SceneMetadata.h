#pragma once

#include <string>

class SceneMetadata {
public:
    SceneMetadata(std::string name, std::string description, std::string url);

    std::string name() const;
    std::string description() const;
    std::string url() const;
    
private:
    std::string m_name;
    std::string m_description;
    std::string m_url;
};