#include "asio_headers/timer_tut/tut5.h"

#include <iostream>
#include <thread>
#include <functional>

using namespace std;
using namespace boost;
using namespace asio;

printer2::printer2(io_context& io) :
    strand_(make_strand(io)),
    timer1_(io, boost::asio::chrono::seconds(1)),
    timer2_(io, boost::asio::chrono::seconds(1)),
    count_(0) {

    timer1_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer2::print1, this)));
    timer2_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer2::print2, this)));
}

/*Конструктор printer2 который принимает адресс контекст io, инициализирует список переменных
strand_, timer1_, timer2_, count_ и создает два таймера. bind_executor благодаря strand_ заставляет callback`у выполняться
строго последовательно (сначала отработает print1(), после print2())*/

void printer2::print1() {
    if (count_ < 10) {
        cout << "Timer 1 -> " << count_ << endl;
        ++(count_);

        timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

        timer1_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer2::print1, this)));
    }
}

//первый принт, выводит каждый тик таймера (в секунду) и значение count_

void printer2::print2() {
    if (count_ < 10) {
        cout << "Timer 2 -> " << count_ << endl;
        ++(count_);
        
        timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

        timer2_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer2::print2, this)));
    }
}

//второй принт, выводит каждый тик таймера (в секунду) и значение count_

printer2::~printer2() {
    cout << "Final count -> " << count_ << endl;
}

//метод который вызывается при уничтожении конструктора, выводит окончательное значение count_

int tut5()
{
    io_context io;
    printer2 p(io);
    thread t([&] { io.run(); });
    io.run();
    t.join();

    return 0;
}

// создание контекста, потока, их обьеденение и запуск