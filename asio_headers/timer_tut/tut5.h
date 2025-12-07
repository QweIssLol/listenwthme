#pragma once

#include "asio_headers/main.h"

using namespace boost;
using namespace asio;

class printer2 {
public:
    printer2(io_context& io);
    void print1();
    void print2();
    ~printer2();

private:
    strand<io_context::executor_type> strand_;
    steady_timer timer1_;
    steady_timer timer2_;
    int count_;
};

int tut5();