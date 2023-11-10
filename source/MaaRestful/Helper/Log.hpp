#pragma once

#include "Helper/Forward.hpp"
#include "Utils/Logger.h"

MAA_RESTFUL_NS_BEGIN

inline void fail(beast::error_code ec, char const* what)
{
    LogError << what << ec.message();
}

MAA_RESTFUL_NS_END
