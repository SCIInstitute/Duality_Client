#pragma once

#include "Scene/InputVariable.h"

#include <memory>
#include <vector>

class DataProvider {
public:
    template <typename T> class InputSetter {
    public:
        InputSetter(const T& variable, DataProvider* dataProvider)
            : m_variable(variable)
            , m_dataProvider(dataProvider) {}
        void setValue(const typename T::ValueType& value) {
            m_variable.setValue(value);
            m_dataProvider->setVariable(m_variable);
        }
        typename T::Info info() const { return m_variable.info(); }

    private:
        T m_variable;
        DataProvider* m_dataProvider;
    };

    virtual ~DataProvider() {}
    virtual std::shared_ptr<std::vector<uint8_t>> fetch() = 0;
    virtual std::vector<InputVariableFloat::Info> floatVariableInfos() const { return {}; }
    virtual std::vector<InputVariableEnum::Info> enumVariableInfos() const { return {}; }
    virtual void setVariable(const InputVariableFloat& variable) {}
    virtual void setVariable(const InputVariableEnum& variable) {}
    virtual std::vector<InputSetter<InputVariableFloat>> floatSetters() { return {}; };
    virtual std::vector<InputSetter<InputVariableEnum>> enumSetters() { return {}; };
};
