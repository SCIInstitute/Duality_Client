#include "src/duality/Dataset.h"

Dataset::Dataset(std::unique_ptr<DataProvider> provider, Visibility visibility)
    : m_provider(std::move(provider)) {}

void Dataset::fetch() {
    readData(*m_provider->fetch());
}

std::vector<FloatVariableInfo> Dataset::floatVariableInfos() const {
    return m_provider->floatVariableInfos();
}

std::vector<EnumVariableInfo> Dataset::enumVariableInfos() const {
    return m_provider->enumVariableInfos();
}

void Dataset::setVariable(const std::string& variableName, float value) {
    m_provider->setVariable(variableName, value);
}

void Dataset::setVariable(const std::string& variableName, const std::string& value) {
    m_provider->setVariable(variableName, value);
}
