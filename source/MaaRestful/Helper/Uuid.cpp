#include "Helper/Uuid.hpp"

MAA_RESTFUL_NS_BEGIN

auto uuid_generator = boost::uuids::random_generator();
std::string make_uuid()
{
    return boost::uuids::to_string(uuid_generator());
}

MAA_RESTFUL_NS_END
