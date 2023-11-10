#include "Handle/RootDispatcher.hpp"
#include "MaaFramework/MaaAPI.h"
#include "Utils/Format.hpp"

MAA_RESTFUL_NS_BEGIN

bool RootDispatcher::handle(Context& ctx, std::vector<std::string_view> url_segs)
{
    if (url_segs.size() == 0) {
        switch (ctx.req_.method()) {
        case http::verb::get:
            ctx.json_body({ { "msg", "hello world!" } });
            break;
        default:
            ctx.bad_request(MAA_FMT::format("bad verb {}", std::string_view(ctx.req_.method_string())));
        }
        return true;
    }
    else {
        auto seg0 = url_segs[0];

        if (seg0 == "version") {
            switch (ctx.req_.method()) {
            case http::verb::get:
                ctx.json_body({ { "version", MaaVersion() } });
                break;
            default:
                ctx.bad_request(MAA_FMT::format("bad verb {}", std::string_view(ctx.req_.method_string())));
            }
            return true;
        }

        return device_.handle(ctx, url_segs);
    }
}

MAA_RESTFUL_NS_END
