#pragma once

#include "Handle/Context.hpp"

#include <string>
#include <vector>

MAA_RESTFUL_NS_BEGIN

struct DispatcherBase
{
    virtual ~DispatcherBase() {}
};

template <typename Env>
struct Dispatcher
{
    Dispatcher(Env env) : env_(env) {}

    virtual bool handle(Context& ctx, std::vector<std::string_view> url_segs) = 0;

    Env env_;
};

template <>
struct Dispatcher<void>
{
    virtual bool handle(Context& ctx, std::vector<std::string_view> url_segs) = 0;
};

http::message_generator handle_request(http::request<http::string_body>&& req);

MAA_RESTFUL_NS_END
