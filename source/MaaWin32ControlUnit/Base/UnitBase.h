#pragma once

#include <optional>

#include "Utils/NoWarningCVMat.hpp"

MAA_CTRL_UNIT_NS_BEGIN

class ScreencapBase
{
public:
    virtual ~ScreencapBase() = default;

public:
    virtual std::optional<cv::Mat> screencap() = 0;

protected:
};

class TouchInputBase
{
public:
    virtual ~TouchInputBase() = default;

public:
    virtual bool click(int x, int y) = 0;
    virtual bool swipe(int x1, int y1, int x2, int y2, int duration) = 0;

    virtual bool touch_down(int contact, int x, int y, int pressure) = 0;
    virtual bool touch_move(int contact, int x, int y, int pressure) = 0;
    virtual bool touch_up(int contact) = 0;
};

class KeyInputBase
{
public:
    virtual ~KeyInputBase() = default;

public:
    virtual bool press_key(int key) = 0;
};

MAA_CTRL_UNIT_NS_END
