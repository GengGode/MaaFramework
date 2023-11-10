#pragma once

#include "Handle/Dispatcher.hpp"

MAA_RESTFUL_NS_BEGIN

struct DeviceDispatcher : public Dispatcher<void>
{
    virtual bool handle(Context& ctx, std::vector<std::string_view> url_segs) override;
};

MAA_RESTFUL_NS_END
