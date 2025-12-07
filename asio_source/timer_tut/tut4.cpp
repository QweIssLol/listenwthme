#include "asio_headers/timer_tut/tut4.h"

printer::printer(io_context& io) :
	timer_(io, boost::asio::chrono::seconds(1)),
	count_(0) 
	{

	timer_.async_wait(std::bind(&printer::print, this));
}

/*создание класса printer, сразу определение ему конструктора с листом инициализации переменных timer_ и count_.
создание внутри конструктора таймера и вызова callback`а this->print(). при использовании std::bind можно игнорировать 
const boost::system::error_code& ec ибо он не используется в print(), соответственно игнорируется.
*/

void printer::print() {
	if (count_ < 5) {
		cout << "Count -> " << count_ << endl;
		++count_;

		timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
		timer_.async_wait(std::bind(&printer::print, this));
	}
}

//метод print() у класса printer() который ничего не возвращает. отработка до значения 5 у count_, продление таймера и 
//повторный запуск метода

printer::~printer() {
	cout << "Final count -> " << count_ << endl;
}

//метод который вызывается при уничтожении конструктора, выводит окончательное значение count_


int tut4() {
	io_context io;
	printer p(io);
	io.run();

	return 0;
}

//создание контекста и запуск механизма