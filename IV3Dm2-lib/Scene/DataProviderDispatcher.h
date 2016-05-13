#pragma once

class DownloadProvider;
class SCIRunProvider;

class DataProviderDispatcher {
public:
    virtual void dispatch(DownloadProvider& provider) = 0;
    virtual void dispatch(SCIRunProvider& provider) = 0;
};