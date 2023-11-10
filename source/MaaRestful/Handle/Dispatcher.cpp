#include "Handle/Dispatcher.hpp"
#include "Handle/RootDispatcher.hpp"
#include "MaaFramework/MaaAPI.h"
#include "Utils/Format.hpp"
#include <iostream>
#include <ranges>

MAA_RESTFUL_NS_BEGIN

http::message_generator handle_request(http::request<http::string_body>&& req)
{
    http::response<http::string_body> res { http::status::ok, req.version() };

    std::string url = req.target();
    auto url_segs_rng = std::string_view(url) | std::views::split('/');
    std::vector<std::string_view> url_segs;
    for (auto seg : url_segs_rng) {
        std::string_view part(seg.begin(), seg.end());
        if (part.length() == 0) {
            continue;
        }
        url_segs.push_back(part);
    }

    Context ctx(std::move(req), res);
    ctx.init();

    RootDispatcher rd;
    if (!rd.handle(ctx, url_segs)) {
        ctx.bad_request(MAA_FMT::format("unknown path {}", std::string_view(ctx.req_.target())));
    }

    res.prepare_payload();

    return res;

    // // auto const server_error = [&req](beast::string_view what) {
    // //     http::response<http::string_body> res { http::status::internal_server_error, req.version() };
    // //     res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // //     res.set(http::field::content_type, "text/html");
    // //     res.keep_alive(req.keep_alive());
    // //     res.body() = "An error occurred: '" + std::string(what) + "'";
    // //     res.prepare_payload();
    // //     return res;
    // // };

    // if (req.method() != http::verb::get) {
    //     return bad_request("Unknown HTTP-method");
    // }

    // http::response<http::string_body> res { http::status::ok, req.version() };
    // res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // res.set(http::field::content_type, "text/plain");
    // res.body() = req.target();
    // res.prepare_payload();
    // res.keep_alive(req.keep_alive());
    // return res;
}

MAA_RESTFUL_NS_END
