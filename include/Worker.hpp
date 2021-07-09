#pragma once

#include "Connection.hpp"

class Worker
{
public:
    Worker();
    ~Worker() noexcept;

    Worker(const Worker& other);
    Worker& operator=(const Worker& other);

    Worker(Worker&& other);
    Worker& operator=(Worker&& other);

public:
    


private:
    

};