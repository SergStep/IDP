#pragma once

class SharedFromThis
{
public:
    SharedFromThis()
        : m_count( new size_t(0))
    {
    }

protected:
    size_t* m_count;
};
