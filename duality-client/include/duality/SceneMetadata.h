#pragma once

#include <string>

class SceneMetadata {
public:
    SceneMetadata(std::string name, std::string description);

    std::string name() const;
    std::string description() const;

private:
    std::string m_name;
    std::string m_description;
};