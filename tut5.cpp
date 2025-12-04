#include <functional>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using namespace std;
using namespace boost;
using namespace asio;

class printer {
public:
	printer(io_context& io)
		: strand_(make_strand(io)), // strand не дает вызовам выполятся одновременно и не допускае гонки данных (count_)
		timer1_(io, boost::asio::chrono::seconds(1)),
		timer2_(io, boost::asio::chrono::seconds(1)),
		count_(0)

		// список инициализации
	{
		timer1_.async_wait(boost::asio::bind_executor(strand_, 
			std::bind(&printer::print1, this)));
		timer2_.async_wait(boost::asio::bind_executor(strand_,
			std::bind(&printer::print2, this)));

		// создание двух таймеров, каждый вызывает свой метод
	}

	void print1()
	{
		if (count_ < 10)
		{
			std::cout << "Timer 1: " << count_ << std::endl;
			++count_;

			timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

			timer1_.async_wait(boost::asio::bind_executor(strand_,
				std::bind(&printer::print1, this)));
		}
	}

	// счет до 10 с интервалами в секунду, выписывает Timer 1: число счетчика

	void print2()
	{
		if (count_ < 10)
		{
			std::cout << "Timer 2: " << count_ << std::endl;
			++count_;

			timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

			timer2_.async_wait(boost::asio::bind_executor(strand_,
				std::bind(&printer::print2, this)));
		}
	}

	// счет до 10 с интервалами в секунду, выписывает Timer 2: число счетчика

	~printer()
	{
		std::cout << "Final count is " << count_ << std::endl;
	}

	// Вызов при уничтожении конструктора
private:
	strand<io_context::executor_type> strand_;
	steady_timer timer1_;
	steady_timer timer2_;
	int count_;
};

int tut5() {

	io_context io;
	printer p(io);
	thread t([&] { io.run(); });
	io.run();
	t.join();

	return 0;
}