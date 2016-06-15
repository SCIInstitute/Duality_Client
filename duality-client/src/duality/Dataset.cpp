#include "src/duality/Dataset.h"

Dataset::Dataset(std::unique_ptr<DataProvider> provider)
    : m_provider(std::move(provider)) {}

void Dataset::fetch() {
    auto data = m_provider->fetch();
    if (data != nullptr) {
        readData(*data);
    }
}

std::vector<FloatVariableInfo> Dataset::floatVariableInfos() const {
    return m_provider->floatVariableInfos();
}

std::vector<EnumVariableInfo> Dataset::enumVariableInfos() const {
    return m_provider->enumVariableInfos();
}

bool Dataset::hasFloatVariable(const std::string& variableName) const {
    return m_provider->hasFloatVariable(variableName);
}

bool Dataset::hasEnumVariable(const std::string& variableName) const {
    return m_provider->hasFloatVariable(variableName);
}

void Dataset::setVariable(const std::string& variableName, float value) {
    m_provider->setVariable(variableName, value);
}

void Dataset::setVariable(const std::string& variableName, const std::string& value) {
    m_provider->setVariable(variableName, value);
}
