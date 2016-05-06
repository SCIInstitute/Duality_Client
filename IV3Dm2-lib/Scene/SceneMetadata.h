#pragma once

#include "jsoncpp/json.h"

#include <string>

class SceneMetadata {
public:
    SceneMetadata(std::string name, std::string description);
    static SceneMetadata fromJson(const JsonCpp::Value& root);

    std::string name() const;
    std::string description() const;

private:
    std::string m_name;
    std::string m_description;
};