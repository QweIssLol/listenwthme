#include "asio_headers/timer_tut/tut2.h"

void print(const boost::system::error_code ec) {
	cout << "Async print" << endl;
}

int tut2() {
	io_context io;
	steady_timer t(io, boost::asio::chrono::seconds(1));

	t.async_wait(&print);
	//async_wait() ожидает от нас адресс на функцию которая обьязательно должа параметром получать "const boost::system::error_code ec" для обработки ошибок
	io.run();

	return 0;
}