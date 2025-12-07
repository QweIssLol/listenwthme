#include "asio_headers/timer_tut/tut3.h"

void print(const boost::system::error_code& ec, steady_timer* t, int* counter) { // для передачи сюда не только ошибки а и таймера со счетчиком используем std::bind();
	if (*counter < 5) {
		cout << *counter << endl;
		++(*counter);
		
		t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
		// продливаем таймер устанавливая значения для таймера expieries_at (истекает в) оставшееся время до смерти таймера + 1 секунда

		t->async_wait(std::bind(
			print,
			boost::asio::placeholders::error,
			t,
			counter
		));
		// t-> (используем лямбду вместо обычного обращения к объекту через точку) потому что мы передаем поинтер на таймер а не его обычную версию
		// создаем новый таймер с тем же использованием std::bind();
	}
}

int tut3() {
	io_context io;
	steady_timer t(io, boost::asio::chrono::seconds(5));

	int counter = 0;

	t.async_wait(std::bind(
		print,
		boost::asio::placeholders::error,
		&t,
		&counter
	)); 

	/* async_wait() принимает только ссылку на функцию которая должна содержать "const boost::system::error_code& ec"
	но если параметров/аргументов больше 0 то тогда используем std::bind();
	передаем адресса на таймер и счетчик потому что в обработчике нужно получать актуальные данные а не копию изменяя которую основной объект/переменная не будет извенятся */

	io.run();

	cout << "Final count " << counter << endl;

	return 0;
}