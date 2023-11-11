#pragma once

#include "Helper/Forward.hpp"

MAA_RESTFUL_NS_BEGIN

struct UrlSegments
{
    UrlSegments(const std::vector<std::string_view>& segs, size_t offset = 0) : segs_(segs), offset_(offset) {}

    bool end() const { return offset_ == segs_.size(); }

    bool end(std::string_view seg) const { return path(seg) && end(); }

    bool path(std::string_view seg) const
    {
        if (end()) {
            return false;
        }
        if (segs_[offset_] != seg) {
            return false;
        }
        return true;
    }

    bool enter_path(std::string_view seg)
    {
        if (end()) {
            return false;
        }
        if (segs_[offset_] != seg) {
            return false;
        }
        offset_++;
        return true;
    }

    bool enter_id(std::string& id)
    {
        if (end()) {
            return false;
        }
        id = segs_[offset_];
        offset_++;
        return true;
    }

    bool enter_id(unsigned long& id)
    {
        if (end()) {
            return false;
        }
        std::string seg(segs_[offset_]);
        // TODO: full check
        if (!isdigit(seg[0])) {
            return false;
        }
        id = std::stoul(seg);
        offset_++;
        return true;
    }

    const std::vector<std::string_view>& segs_;
    size_t offset_;
};

MAA_RESTFUL_NS_END
