#include "Handle/RootDispatcher.hpp"
#include "MaaFramework/MaaAPI.h"
#include "Utils/Format.hpp"

MAA_RESTFUL_NS_BEGIN

using namespace beast::http;

bool RootDispatcher::handle(Context& ctx, UrlSegments segs)
{
    if (segs.end()) {
        ctx.match_verb(verb::get, [](Context& ctx) { ctx.json_body({ { "msg", "hello world!" } }); });
        return true;
    }
    else if (segs.end("version")) {
        ctx.match_verb(verb::get, [](Context& ctx) { ctx.json_body({ { "version", MaaVersion() } }); });
        return true;
    }
    else {
        return device_.handle(ctx, segs);
    }
}

MAA_RESTFUL_NS_END
