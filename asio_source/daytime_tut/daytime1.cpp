#include "asio_headers/daytime_tut/daytime1.h"

using boost::asio::ip::tcp;

int daytime1(int argc, char* argv[]) /* аргументы как у main, для возможности запуска с командной строки.argc - кол - во передаваемых аргументов 
argv - массимв строк, argv[0] - название программы, argv[1] - в нашем случае адресс сервера с которого мы будем считывать daytime. */

{
    try
    {
        if (argc != 2) // Ожидаем только 1 аргумент (не считая названия программы)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;

        // ниже - цепочка действий для TCP - клиента: создание резольвера, получение эндпоинтов, создание сокета и подключение сокета.

        tcp::resolver resolver(io_context);
        /* сам по себе резольвер преобразовывает читаемый для человека хост(example.com + daytime) в ip и port
        где ip который резольвер запрашивает у DNS - хост
        а порт - сервис "daytime" (по дефолту 13)

        тут мы только создаем его, передавая контекст
        */

        tcp::resolver::results_type endpoints =
            resolver.resolve(argv[1], "daytime");
        /* выполнение DNS разрешений + поиск сервиса
        argv[1] -> название хоста
        "daytime" -> название сервиса
        
        resolver.resolve() возвращает список возможных эндпоинтов (полный сетевой список ip+port)
        */

        tcp::socket socket(io_context);
        // создание сокета

        boost::asio::connect(socket, endpoints);
        /* попытка установить подключение
        * 
        * перебирает все адресса из эндпоинтов (тоесть резольвер вернул определенные ip адреса для хоста
        * например 93.184.216.34 и 2606:2800:220:1:248:1893:25c8:1946, и он перебирает их все с портом (в нашем случае 13
        * так как мы хотим получить сервис daytime))
        * 
        * пытается подключится к каждому по очереди
        * использует первый успешный
        * 
        * если соединение успешно - сокет активный и готов для читки данных
        */

        for (;;)
        {
            std::array<char, 128> buf;
            boost::system::error_code error;
            // создания буфера и хендлера ошибки на будущее

            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            // socket.read_some() пытается высосать столько доступных данных сколько возможно но не больше чем размер буффера.

            if (error == boost::asio::error::eof) // eof - знак что сокет закончил читку данных успешно и закрылся, сокет всегда в конце закрывается с ошибкой
                break;
            else if (error)
                throw boost::system::system_error(error);

            std::cout.write(buf.data(), len);
            /* buf.data() указатель на начало 
            * len - кол-во байт высосаных из сокета
            * 
            * cout.write() выводит ровно те байты, которые сокет успешно высосал
            */
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
