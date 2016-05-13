#pragma once

#include "Scene/InputParameter.h"
#include "Scene/DataProviderDispatcher.h"

#include <string>

class ParameterManipulator {
public:
    virtual ~ParameterManipulator() {}

    virtual std::string name() const = 0;
    virtual std::vector<InputParameterFloat> floatParameters() const = 0;
    virtual std::vector<InputParameterEnum> enumParameters() const = 0;
    //virtual void setFloatParameter(const std::string& name, float value);
    //virtual void setEnumParameter(const std::string& name, const std::string& value);
};

class ParameterManipulatorCollector : public DataProviderDispatcher {
public:
    void dispatch(DownloadProvider& provider) override;
    void dispatch(SCIRunProvider& provider) override;

    std::vector<ParameterManipulator*> manipulators() const;

private:
    std::vector<ParameterManipulator*> m_manipulators;
};