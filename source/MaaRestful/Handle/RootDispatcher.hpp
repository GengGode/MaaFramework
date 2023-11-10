#pragma once

#include "Handle/DeviceDispatcher.hpp"

MAA_RESTFUL_NS_BEGIN

struct RootDispatcher : public Dispatcher<void>
{
    virtual bool handle(Context& ctx, std::vector<std::string_view> url_segs) override;

    DeviceDispatcher device_;
};

MAA_RESTFUL_NS_END
