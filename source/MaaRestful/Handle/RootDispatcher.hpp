#pragma once

#include "Handle/DeviceDispatcher.hpp"

MAA_RESTFUL_NS_BEGIN

struct RootDispatcher
{
    bool handle(Context& ctx, UrlSegments segs);

    DeviceDispatcher device_;
};

MAA_RESTFUL_NS_END
