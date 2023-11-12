#pragma once

#define BOOST_WANT_UUID
#define BOOST_WANT_BEAST

#include "Conf/Conf.h"
#include "Utils/Boost.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;
