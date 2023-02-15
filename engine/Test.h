#pragma once

class Test
{
public:
    Test();
    int GetValue() const { return mValue; }
    void IncrementValue() { mValue++; }

private:
    int mValue;
};