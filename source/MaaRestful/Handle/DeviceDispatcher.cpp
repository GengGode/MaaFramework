#include "Handle/DeviceDispatcher.hpp"
#include "MaaToolKit/MaaToolKitAPI.h"
#include "Utils/Format.hpp"

MAA_RESTFUL_NS_BEGIN

bool DeviceDispatcher::handle(Context& ctx, std::vector<std::string_view> url_segs)
{
    std::unique_lock<std::mutex> lock(find_mtx_);

    auto seg0 = url_segs[0];

    if (seg0 == "devices") {
        if (url_segs.size() == 1) {
            // switch (ctx.req_.method()) {
            // case http::verb::get:
            //     // TODO: return all devices
            //     ctx.json_body({ { "count", MaaToolKitFindDevice() } });
            //     break;
            // default:
            //     ctx.bad_request(MAA_FMT::format("bad verb {}", std::string_view(ctx.req_.method_string())));
            // }
            // return true;
        }
        else {
            auto seg1 = url_segs[1];

            if (seg1 == "find") {
                switch (ctx.req_.method()) {
                case http::verb::put:
                    find_count_ = MaaToolKitFindDevice();
                    ctx.json_body({ { "count", find_count_ } });
                    break;
                default:
                    ctx.bad_request(MAA_FMT::format("bad verb {}", std::string_view(ctx.req_.method_string())));
                }
                return true;
            }
        }
    }
    else if (seg0 == "device") {
        if (url_segs.size() == 1) {
            ctx.bad_request("id expected");
            return true;
        }
        else {
            auto seg1 = url_segs[1];

            auto id = std::stoul(std::string(seg1));
            if (id >= find_count_) {
                ctx.not_found("id not found");
                return true;
            }

            if (url_segs.size() == 2) {
                switch (ctx.req_.method()) {
                case http::verb::get:
                    ctx.json_body({ { "name", MaaToolKitGetDeviceName(id) },
                                    { "adb_path", MaaToolKitGetDeviceAdbPath(id) },
                                    { "adb_serial", MaaToolKitGetDeviceAdbSerial(id) },
                                    { "controller_type", MaaToolKitGetDeviceAdbControllerType(id) },
                                    { "adb_config", MaaToolKitGetDeviceAdbConfig(id) } });
                    break;
                default:
                    ctx.bad_request(MAA_FMT::format("bad verb {}", std::string_view(ctx.req_.method_string())));
                }
                return true;
            }
            else {
                // TODO: implement partial query
            }
        }
    }

    return false;
}

MAA_RESTFUL_NS_END
