#include <functional>
#include <boost/asio.hpp>
#include <iostream>

using namespace std;
using namespace boost;
using namespace asio;

class printer {
public:
	printer(io_context& io)
		: timer_(io, boost::asio::chrono::seconds(1)),
		counter_(0)

		/*
		список инициализации для конструктора printer
		сами timer_ и counter_ создаются во время создания класса в private
		*/
	{
		timer_.async_wait(std::bind(&printer::print, this));
	}

	void print()
	{
		if (counter_ < 5)
		{
			std::cout << counter_ << std::endl;
			++counter_;

			timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
			timer_.async_wait(std::bind(&printer::print, this));

			// при отработке таймера вызывает у класса printer метод print(), но не у рандомного а у того который передается (this), тоесть вызов идет this->print();
		}
	}
private:
	steady_timer timer_;
	int counter_;
};

/*
создания класса с конструктором принимающий адрес асио контекста при вызове и метода print() который каждую секунду выписывает счетчик до 5
*/

int tut4() {
	io_context io;

	printer p(io);

	io.run();

	return 0;
}