#pragma once

#include "Connection.hpp"

class ProcessPool
{
public:
    ProcessPool();
    ~ProcessPool();

    ProcessPool(const ProcessPool&) = delete;
    ProcessPool& operator=(const ProcessPool&) = delete;
    
    ProcessPool(ProcessPool&&) = delete;
    ProcessPool& operator=(ProcessPool&&) = delete;

public:
    int get_cpu_cores() const;
    void close_pool();

private:
    int m_cpu_cores;
};
