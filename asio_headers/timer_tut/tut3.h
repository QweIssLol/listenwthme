#pragma once

#include "asio_headers/main.h"

using namespace std;
using namespace boost;
using namespace asio;

#include "asio_headers/timer_tut/tut3.h"

void print(const boost::system::error_code& ec, steady_timer* t, int* counter);
int tut3();