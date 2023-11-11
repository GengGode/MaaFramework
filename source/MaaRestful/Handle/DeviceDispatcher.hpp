#pragma once

#include "Handle/Context.hpp"
#include "Helper/UrlSegments.hpp"

#include <mutex>

MAA_RESTFUL_NS_BEGIN

struct DeviceDispatcher
{
    bool handle(Context& ctx, UrlSegments segs);

    static std::mutex find_mtx_;
    static size_t find_count_;
};

MAA_RESTFUL_NS_END
