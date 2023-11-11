#include "Handle/DeviceDispatcher.hpp"
#include "MaaToolKit/MaaToolKitAPI.h"
#include "Utils/Format.hpp"

MAA_RESTFUL_NS_BEGIN

std::mutex DeviceDispatcher::find_mtx_;
size_t DeviceDispatcher::find_count_;

using namespace boost::beast::http;

bool DeviceDispatcher::handle(Context& ctx, UrlSegments segs)
{
    std::unique_lock<std::mutex> lock(find_mtx_);

    if (segs.enter_path("devices")) {
        if (segs.end()) {
            ctx.match_verb(verb::get, [](Context& ctx) {
                json::array result;
                for (size_t i = 0; i < find_count_; i++) {
                    result.push_back(json::object { { "name", MaaToolKitGetDeviceName(i) },
                                                    { "adb_path", MaaToolKitGetDeviceAdbPath(i) },
                                                    { "adb_serial", MaaToolKitGetDeviceAdbSerial(i) },
                                                    { "controller_type", MaaToolKitGetDeviceAdbControllerType(i) },
                                                    { "adb_config", MaaToolKitGetDeviceAdbConfig(i) } });
                }
                ctx.json_body({ { "device", result } });
            });
            return true;
        }
        else if (segs.end("find")) {
            ctx.match_verb(verb::put, [this](Context& ctx) {
                find_count_ = MaaToolKitFindDevice();
                ctx.json_body({ { "count", find_count_ } });
            });
            return true;
        }
    }
    else if (segs.enter_path("device")) {
        unsigned long id;
        if (!segs.enter_id(id)) {
            ctx.bad_request("id expected");
            return true;
        }
        if (id >= find_count_) {
            ctx.not_found("id not found");
            return true;
        }
        if (segs.end()) {
            ctx.match_verb(verb::get, [id](Context& ctx) {
                ctx.json_body({ { "name", MaaToolKitGetDeviceName(id) },
                                { "adb_path", MaaToolKitGetDeviceAdbPath(id) },
                                { "adb_serial", MaaToolKitGetDeviceAdbSerial(id) },
                                { "controller_type", MaaToolKitGetDeviceAdbControllerType(id) },
                                { "adb_config", MaaToolKitGetDeviceAdbConfig(id) } });
            });
            return true;
        }
    }

    return false;
}

MAA_RESTFUL_NS_END
