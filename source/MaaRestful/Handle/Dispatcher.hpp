#pragma once

#include "Handle/Context.hpp"

#include <string>
#include <vector>

MAA_RESTFUL_NS_BEGIN

http::message_generator handle_request(http::request<http::string_body>&& req);

MAA_RESTFUL_NS_END
