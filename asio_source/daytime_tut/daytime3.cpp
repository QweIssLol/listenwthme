#include "asio_headers/main.h"

using boost::asio::ip::tcp;

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
    using pointer = std::shared_ptr<tcp_connection>;

    static pointer create(boost::asio::io_context& io_context)
    {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& socket() { return socket_; }

    void start()
    {
        message_ = make_daytime_string();
        
		// создаем переменную message_ и заполняем её текущей датой в строковом формате

        boost::asio::async_write(socket_, boost::asio::buffer(message_),
            std::bind(&tcp_connection::handle_write, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));

		/* запускаем асинхронную запись в сокет с помощью boost::asio::async_write. передаем туда сокет,
		* буфер с сообщением (тоесть само сообщение в байтах) и хендлер handle_write который будет вызван после завершения записи.
		* тут мы используем shared_from_this() для std::bind что бы получить shared_ptr на текущий обьект tcp_connection
		* (ибо если мы передадим просто this, то в момент вызова handle_write обьект может уже быть уничтожен, что вызовет
		* ошибку доступа к памяти).
        */
    }

private:
    explicit tcp_connection(boost::asio::io_context& io_context)
        : socket_(io_context) {
    }

	/* запрещаем неявное преобразование типов для создания объекта tcp_connection
    * например нельзя было бы просто написать tcp_connection con = io_context.
	* нужно вызвать конструктор явно tcp_connection con(io_context);
	* сам конструктор принимает ссылку на io_context и инициализирует сокет с его помощью.
	* все это используется для создания new_connection в tcp_server где мы вызываем tcp_connection::create(io_context)
    */

    void handle_write(const boost::system::error_code& /*ec*/, std::size_t /*bytes_transferred*/)
    {
        // После отправки данных мы просто завершаем работу с этим соединением.
        // Можно закрыть сокет явно или позволить dtor освободить ресурсы.
    }

    tcp::socket socket_;
    std::string message_;
};

class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_context)
        : io_context_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 13)) // порт 13 — daytime

        // сразу для конструктора инициаизируем io_context и acceptor, accpetor слушает порт 13
    {
        start_accept();
    }

private:
    void start_accept()
    {
        auto new_connection = tcp_connection::create(io_context_);

		// создаем новое соединение и начинаем асинхронный приём с помощью acceptor_.async_accept

        acceptor_.async_accept(new_connection->socket(),
            std::bind(&tcp_server::handle_accept, this, new_connection, std::placeholders::_1));

        /*new_connection сам по себе это shared_ptr который мы проинициализировали в tcp_connection с помощью typedef
        для std::shared_ptr<tcp_connection> pointer. тут мы создали указатель shared_ptr на обьект типа tcp_connection
        и теперь спокойно может вызывать методы tcp_connection через ->.
		
        дальше мы вызываем acceptor_.async_accept и передаем туда сокет нового соединения, а также хендлер 
		handle_accept который будет вызван после установки соединения. сам хедлер вызывается с помощью std::bind и принимает
		три параметра: this - указатель на текущий обьект tcp_server, new_connection - указатель на новое соединение,
		и std::placeholders::_1 - зарезервированное место для ошибки, которая будет передана в хендлер автоматически.
        
        так как handle_accept не является статическим методом, то мы должны передать указатель на текущий обьект tcp_server
		(сам this) что бы иметь доступ к его методам и переменным внутри handle_accept.

		this мы передаем потому что handle_accept (и вообще все методы класса) не являются статическими методами, и ожидают
		его по дефолту как первый параметр. (тоесть сигнатура этого метода на самом деле выглядит так:
        void handle_accept(tcp_server* this, tcp_connection::pointer new_connection, const boost::system::error_code& error);)
        */
    }

    void handle_accept(tcp_connection::pointer new_connection,
        const boost::system::error_code& error)
    {
        if (!error)
        {
			// проверяем ошибки, если их нет, запускаем новое соединение
            new_connection->start();
        }
        // продолжить приём следующих соединений
        start_accept();
    }

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

int daytime3()
{
    try
    {
        boost::asio::io_context io_context;
        tcp_server server(io_context);

        /*создание переменной server и запуск цепочки создания сервера, передаем io_context*/

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
