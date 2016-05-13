#include "Scene/ParameterManipulator.h"

#include "Scene/DownloadProvider.h"
#include "Scene/SCIRunProvider.h"

void ParameterManipulatorCollector::dispatch(DownloadProvider& provider) {
    // nothing to do
}

void ParameterManipulatorCollector::dispatch(SCIRunProvider& provider) {
    m_manipulators.push_back(&provider);
}

std::vector<ParameterManipulator*> ParameterManipulatorCollector::manipulators() const {
    return m_manipulators;
}
