#include "asio_headers/main.h"

std::string make_daytime_string() {
    time_t now = time(0);
    char buf[26];
    ctime_s(buf, sizeof(buf), &now);
    return std::string(buf);
}