#pragma once

#include "Handle/Dispatcher.hpp"

#include <mutex>

MAA_RESTFUL_NS_BEGIN

struct DeviceDispatcher : public Dispatcher<void>
{
    virtual bool handle(Context& ctx, std::vector<std::string_view> url_segs) override;

    std::mutex find_mtx_;
    size_t find_count_;
};

MAA_RESTFUL_NS_END
