#pragma once

class SliderParameter {
public:
    explicit SliderParameter(int slice, float depth);
    explicit SliderParameter(float depth);
    
    bool hasSlice() const;
    int slice() const;
    float depth() const;

private:
    bool m_hasSlice;
    int m_slice;
    float m_depth;
};

bool operator==(const SliderParameter& lhs, const SliderParameter& rhs);
bool operator!=(const SliderParameter& lhs, const SliderParameter& rhs);