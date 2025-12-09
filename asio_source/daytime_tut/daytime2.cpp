#include "asio_headers/daytime_tut/daytime2.h"

using boost::asio::ip::tcp;
using namespace std;
using namespace boost;
using namespace asio;

std::string make_daytime_string() {
	time_t now = time(0);
	char buf[26];
	ctime_s(buf, sizeof(buf), &now);
	return string(buf);
}

int daytime2() {
	try {
		io_context io;

		tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 3333));

		for (;;){ // бесконечный цикл, сервер работает постоянно

			tcp::socket socket(io);
			// создание сокета

			acceptor.accept(socket);
			// ацептор слушает входящие соединения с сокета

			std::string message = make_daytime_string();
			// получаем текущую дату в формате строки типа "Mon Dec 9 12:34:56 2025\n"
			
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
			/*создание хендлера ошибки
			* boost::asio::write() отправляет данные через TCP подключение на наш сокет
			* boost::asio::buffer() превращает параметр в байты для отправки
			* ignored_error создаем что бы временно игнорировать все ошибки
			*/
		}	
	}
	catch (std::exception& e) {
		cerr << e.what() << endl;
	}

	return 0;
}