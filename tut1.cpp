#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost;
using namespace asio;

int tut1() {

	io_context io;
	//создание контекста asio
	steady_timer t(io, boost::asio::chrono::seconds(1));
	//создание таймера
	cout << "First action" << endl;
	t.wait();
	//wait() вызов блокирующего ожидания, время ожидания указанна в самом таймере
	cout << "Second action" << endl;

	/*
	
	t.expires_at(t.expiry() + boost::asio::chrono::seconds(2));  --  изменение оставшееся времени для таймера 
	(секунда прошла, второй вывод был, добавляем еще 2 секунды)
	t.wait();
	cout << "Third action" << endl;

	*/
	
	return 0;
}