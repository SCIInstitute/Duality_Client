#pragma once

#include "Scene/InputVariable.h"

#include <memory>
#include <vector>

class DataProvider {
public:
    template <typename T> class InputSetter {
    public:
        InputSetter(const std::string& variable, DataProvider& dataProvider)
            : m_variable(variable)
            , m_dataProvider(dataProvider) {}
        void setInputValue(const T& value) { m_dataProvider.setInputValue(value); }

    private:
        std::string m_variable;
        DataProvider& m_dataProvider;
    };

    virtual ~DataProvider() {}
    virtual std::shared_ptr<std::vector<uint8_t>> fetch() = 0;
    virtual std::vector<InputVariableFloat::Info> floatVariableInfos() const { return{}; }
    virtual std::vector<InputVariableEnum::Info> enumVariableInfos() const { return{}; }
    virtual void setInputValue(const std::string& variable, float value) {}
    virtual void setInputValue(const std::string& variable, const std::string& value) {}
    virtual std::vector<InputSetter<float>> floatSetters() { return {}; };
    virtual std::vector<InputSetter<std::string>> enumSetters() { return{}; };
};
