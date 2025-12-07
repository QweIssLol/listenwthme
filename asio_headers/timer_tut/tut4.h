#pragma once

#include "asio_headers/main.h"

using namespace std;
using namespace boost;
using namespace asio;

class printer {
public:
	printer(io_context& io);
	void print();
	~printer();
private:
	steady_timer timer_;
	int count_;
};


int tut4();