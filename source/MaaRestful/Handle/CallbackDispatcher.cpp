#include "Handle/CallbackDispatcher.hpp"
#include "Utils/Format.hpp"

MAA_RESTFUL_NS_BEGIN

std::mutex CallbackDispatcher::mtx_;
std::map<std::string, std::function<void(json::value&&)>> CallbackDispatcher::callbacks_;

using namespace boost::beast::http;

bool CallbackDispatcher::handle(Context& ctx, UrlSegments segs)
{
    std::ignore = ctx;
    std::ignore = segs;
    return false;
}

MAA_RESTFUL_NS_END
