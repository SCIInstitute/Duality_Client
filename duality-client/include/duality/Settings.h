#pragma once

#include <string>
#include <array>

class Settings {
public:
    virtual std::string serverIP() const {
        return "127.0.0.1";
    }
    virtual void setServerIP(const std::string& ip) {}
    
    virtual std::string serverPort() const {
        return "10123";
    }
    virtual void setServerPort(const std::string& port) {}
    
    virtual bool anatomicalTerms() const {
        return false;
    }
    virtual void setAnatomicalTerms(bool enabled) {}
    
    virtual bool cachingEnabled() const {
        return true;
    }
    virtual void setCachingEnabled(bool enabled) {}
    
    virtual std::array<float, 3> backgroundColor() const {
        return {0.0f, 0.0f, 0.0f};
    }
    virtual void setBackgroundColor(const std::array<float, 3>& color) {}
};