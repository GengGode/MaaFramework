#pragma once

#include "Handle/Context.hpp"
#include "Helper/UrlSegments.hpp"

#include <functional>
#include <map>
#include <meojson/json.hpp>
#include <mutex>

MAA_RESTFUL_NS_BEGIN

struct CallbackDispatcher
{
    bool handle(Context& ctx, UrlSegments segs);

    static std::mutex mtx_;
    static std::map<std::string, std::function<void(json::value&&)>> callbacks_;
};

MAA_RESTFUL_NS_END
