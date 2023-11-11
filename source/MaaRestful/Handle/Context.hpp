#pragma once

#include "Helper/Forward.hpp"
#include "Utils/Format.hpp"

#include <meojson/json.hpp>

MAA_RESTFUL_NS_BEGIN

struct Context
{
    Context(http::request<http::string_body>&& req, http::response<http::string_body>& res)
        : req_(std::move(req)), res_(res)
    {}

    void json_body(const json::object& value)
    {
        res_.set(http::field::content_type, "application/json");
        res_.body() = value.to_string();
    }

    void bad_request(const std::string& why)
    {
        res_.result(http::status::bad_request);
        json_body({ { "error", why } });
    }

    void not_found(const std::string& why)
    {
        res_.result(http::status::not_found);
        json_body({ { "error", why } });
    }

    void init()
    {
        res_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res_.keep_alive(req_.keep_alive());
    }

    void match_verb() { bad_request(MAA_FMT::format("bad verb {}", std::string_view(req_.method_string()))); }

    template <typename Func>
    void match_verb(http::verb v, Func func)
    {
        if (req_.method() == v) {
            func(*this);
        }
        else {
            match_verb();
        }
    }

    template <typename Func, typename... Args>
    void match_verb(http::verb v, Func func, Args&&... args)
    {
        if (req_.method() == v) {
            func(*this);
        }
        else {
            match_verb(std::forward(args...));
        }
    }

    http::request<http::string_body> req_;
    http::response<http::string_body>& res_;
};

MAA_RESTFUL_NS_END
