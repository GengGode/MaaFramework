#include "MaaAPI.h"

#include <meojson/json.hpp>

#include "Controller/AdbController.h"
#include "Controller/CustomController.h"
#include "Instance/InstanceMgr.h"
#include "Option/GlobalOptionMgr.h"
#include "Resource/ResourceMgr.h"
#include "Utils/Logger.hpp"
#include "Utils/Platform.hpp"

MaaBool MaaSetGlobalOption(MaaGlobalOption key, MaaString value)
{
    LogFunc << VAR(key) << VAR(value);

    return MAA_NS::GlobalOptionMgr::get_instance().set_option(key, value);
}

MaaResourceHandle MaaResourceCreate(MaaString user_path, MaaResourceCallback callback, void* callback_arg)
{
    LogFunc << VAR(user_path) << VAR_VOIDP(callback) << VAR_VOIDP(callback_arg);

    return new MAA_RES_NS::ResourceMgr(MAA_NS::path(user_path), callback, callback_arg);
}

void MaaResourceDestroy(MaaResourceHandle* res)
{
    LogFunc << VAR_VOIDP(res);

    if (res == nullptr || *res == nullptr) {
        return;
    }
    delete *res;
    *res = nullptr;
}

MaaResId MaaResourcePostResource(MaaResourceHandle res, MaaString path)
{
    LogFunc << VAR_VOIDP(res) << VAR(path);

    if (!res) {
        return MaaInvalidId;
    }
    return res->post_resource(MAA_NS::path(path));
}

MaaStatus MaaResourceStatus(MaaResourceHandle res, MaaResId id)
{
    // LogFunc << VAR_VOIDP(res) << VAR(id);

    if (!res) {
        return MaaStatus_Invalid;
    }
    return res->status(id);
}

MaaStatus MaaResourceWait(MaaResourceHandle res, MaaResId id)
{
    // LogFunc << VAR_VOIDP(res) << VAR(id);

    if (!res) {
        return MaaStatus_Invalid;
    }
    return res->wait(id);
}

MaaBool MaaResourceLoaded(MaaResourceHandle res)
{
    // LogFunc << VAR_VOIDP(res);

    if (!res) {
        return false;
    }
    return res->loaded();
}

MaaBool MaaResourceSetOption(MaaResourceHandle res, MaaResOption key, MaaString value)
{
    LogFunc << VAR_VOIDP(res) << VAR(key) << VAR(value);

    if (!res) {
        return false;
    }
    return res->set_option(key, value);
}

MaaSize MaaResourceGetHash(MaaResourceHandle res, char* buff, MaaSize buff_size)
{
    LogFunc << VAR_VOIDP(res) << VAR_VOIDP(buff) << VAR(buff_size);

    if (!res || !buff) {
        return MaaNullSize;
    }
    auto hash = res->get_hash();
    size_t size = hash.size();
    if (size >= buff_size) {
        return MaaNullSize;
    }
    memcpy(buff, hash.c_str(), size);
    return size;
}

MaaControllerHandle MaaAdbControllerCreate(MaaString adb_path, MaaString address, MaaJsonString config,
                                           MaaAdbControllerType type, MaaControllerCallback callback,
                                           void* callback_arg)
{
    LogFunc << VAR(adb_path) << VAR(address) << VAR_VOIDP(callback) << VAR_VOIDP(callback_arg);

    auto json_opt = json::parse(std::string_view(config));
    if (!json_opt) {
        LogError << "Parse json failed, invalid config:" << config;
        return nullptr;
    }

    std::shared_ptr<MAA_CTRL_UNIT_NS::TouchInputBase> touch_unit;
    std::shared_ptr<MAA_CTRL_UNIT_NS::KeyInputBase> key_unit;
    std::shared_ptr<MAA_CTRL_UNIT_NS::ScreencapBase> screencap_unit;

    std::shared_ptr<MAA_CTRL_UNIT_NS::MaatouchInput> maatouch_unit;

    auto touch_type = type & MaaAdbControllerType_Touch_Mask;
    auto key_type = type & MaaAdbControllerType_Key_Mask;
    auto screencap_type = type & MaaAdbControllerType_Screencap_Mask;

    if (touch_type == MaaAdbControllerType_Touch_MaaTouch || key_type == MaaAdbControllerType_Key_MaaTouch) {
        if ((touch_type | key_type) != MaaAdbControllerType_Input_Preset_Maatouch) {
            LogWarn << "Using maatouch for partial input!" << VAR((touch_type | key_type));
            // return nullptr;
        }
        maatouch_unit = std::make_shared<MAA_CTRL_UNIT_NS::MaatouchInput>();
    }

    switch (touch_type) {
    case MaaAdbControllerType_Touch_Adb:
        touch_unit = std::make_shared<MAA_CTRL_UNIT_NS::TapTouchInput>();
        break;
    case MaaAdbControllerType_Touch_MiniTouch:
        touch_unit = std::make_shared<MAA_CTRL_UNIT_NS::MinitouchInput>();
        break;
    case MaaAdbControllerType_Touch_MaaTouch:
        touch_unit = maatouch_unit;
        break;
    default:
        LogError << "Unknown touch input type" << VAR(touch_type);
        return nullptr;
    }

    switch (key_type) {
    case MaaAdbControllerType_Key_Adb:
        key_unit = std::make_shared<MAA_CTRL_UNIT_NS::TapKeyInput>();
        break;
    case MaaAdbControllerType_Touch_MaaTouch:
        key_unit = maatouch_unit;
        break;
    default:
        LogError << "Unknown key input type" << VAR(key_type);
        return nullptr;
    }

    switch (screencap_type) {
    case MaaAdbControllerType_Screencap_Auto:
        screencap_unit = std::make_shared<MAA_CTRL_UNIT_NS::Screencap>();
        break;
    case MaaAdbControllerType_Screencap_RawByNetcat:
        screencap_unit = std::make_shared<MAA_CTRL_UNIT_NS::ScreencapRawByNetcat>();
        break;
    case MaaAdbControllerType_Screencap_RawWithGzip:
        screencap_unit = std::make_shared<MAA_CTRL_UNIT_NS::ScreencapRawWithGzip>();
        break;
    case MaaAdbControllerType_Screencap_Encode:
        screencap_unit = std::make_shared<MAA_CTRL_UNIT_NS::ScreencapEncode>();
        break;
    case MaaAdbControllerType_Screencap_EncodeToFile:
        screencap_unit = std::make_shared<MAA_CTRL_UNIT_NS::ScreencapEncodeToFileAndPull>();
        break;
    case MaaAdbControllerType_Screencap_MinicapDirect:
        screencap_unit = std::make_shared<MAA_CTRL_UNIT_NS::MinicapDirect>();
        break;
    case MaaAdbControllerType_Screencap_MinicapStream:
        screencap_unit = std::make_shared<MAA_CTRL_UNIT_NS::MinicapStream>();
        break;
    default:
        LogError << "Unknown screencap type" << VAR(screencap_type);
        return nullptr;
    }

    auto unit_opt = MAA_CTRL_NS::AdbController::parse_config(*json_opt, touch_unit, key_unit, screencap_unit);
    if (!unit_opt) {
        LogError << "Parse config failed, invalid config:" << *json_opt;
        return nullptr;
    }

    return new MAA_CTRL_NS::AdbController(adb_path, address, std::move(*unit_opt), callback, callback_arg);
}

MaaControllerHandle MaaCustomControllerCreate(MaaCustomControllerHandle handle, MaaControllerCallback callback,
                                              void* callback_arg)
{
    LogFunc << VAR(handle) << VAR_VOIDP(callback) << VAR_VOIDP(callback_arg);

    if (!handle) {
        return nullptr;
    }

    return new MAA_CTRL_NS::CustomController(handle, callback, callback_arg);
}

void MaaControllerDestroy(MaaControllerHandle* ctrl)
{
    LogFunc << VAR_VOIDP(ctrl);

    if (ctrl == nullptr || *ctrl == nullptr) {
        return;
    }
    delete *ctrl;
    *ctrl = nullptr;
}

MaaBool MaaControllerSetOption(MaaControllerHandle ctrl, MaaCtrlOption key, MaaString value)
{
    LogFunc << VAR_VOIDP(ctrl) << VAR(key) << VAR(value);

    if (!ctrl) {
        return false;
    }
    return ctrl->set_option(key, value);
}

MaaCtrlId MaaControllerPostConnection(MaaControllerHandle ctrl)
{
    LogFunc << VAR_VOIDP(ctrl);
    if (!ctrl) {
        return false;
    }
    return ctrl->post_connection();
}

MaaCtrlId MaaControllerPostClick(MaaControllerHandle ctrl, int32_t x, int32_t y)
{
    LogFunc << VAR_VOIDP(ctrl) << VAR(x) << VAR(y);

    if (!ctrl) {
        return MaaInvalidId;
    }
    return ctrl->post_click(x, y);
}

MaaCtrlId MaaControllerPostSwipe(MaaControllerHandle ctrl, int32_t* x_steps_buff, int32_t* y_steps_buff,
                                 int32_t* step_delay_buff, MaaSize buff_size)
{
    LogFunc << VAR_VOIDP(ctrl) << VAR_VOIDP(x_steps_buff) << VAR_VOIDP(y_steps_buff) << VAR_VOIDP(step_delay_buff)
            << VAR(buff_size);

    if (!ctrl || !x_steps_buff || !y_steps_buff || !step_delay_buff || buff_size < 2) {
        return MaaInvalidId;
    }

    std::vector<int32_t> x_steps(x_steps_buff, x_steps_buff + buff_size);
    std::vector<int32_t> y_steps(y_steps_buff, y_steps_buff + buff_size);
    std::vector<int32_t> step_delay(step_delay_buff, step_delay_buff + buff_size);

    return ctrl->post_swipe(std::move(x_steps), std::move(y_steps), std::move(step_delay));
}

MaaCtrlId MaaControllerPostScreencap(MaaControllerHandle ctrl)
{
    LogFunc << VAR_VOIDP(ctrl);

    if (!ctrl) {
        return MaaInvalidId;
    }
    return ctrl->post_screencap();
}

MaaStatus MaaControllerStatus(MaaControllerHandle ctrl, MaaCtrlId id)
{
    // LogFunc << VAR_VOIDP(ctrl) << VAR(id);

    if (!ctrl) {
        return MaaStatus_Invalid;
    }
    return ctrl->status(id);
}

MaaStatus MaaControllerWait(MaaControllerHandle ctrl, MaaCtrlId id)
{
    // LogFunc << VAR_VOIDP(ctrl) << VAR(id);

    if (!ctrl) {
        return MaaStatus_Invalid;
    }
    return ctrl->wait(id);
}

MaaBool MaaControllerConnected(MaaControllerHandle ctrl)
{
    LogFunc << VAR_VOIDP(ctrl);
    if (!ctrl) {
        return false;
    }
    return ctrl->connected();
}

MaaSize MaaControllerGetImage(MaaControllerHandle ctrl, void* buff, MaaSize buff_size)
{
    LogFunc << VAR_VOIDP(ctrl) << VAR_VOIDP(buff) << VAR(buff_size);

    if (!ctrl || !buff) {
        return MaaNullSize;
    }
    auto image = ctrl->get_image();
    size_t size = image.size();
    if (size >= buff_size) {
        return MaaNullSize;
    }
    memcpy(buff, image.data(), size);
    return size;
}

MaaSize MaaControllerGetUUID(MaaControllerHandle ctrl, char* buff, MaaSize buff_size)
{
    LogFunc << VAR_VOIDP(ctrl) << VAR_VOIDP(buff) << VAR(buff_size);

    if (!ctrl || !buff) {
        return MaaNullSize;
    }
    auto uuid = ctrl->get_uuid();
    size_t size = uuid.size();
    if (size >= buff_size) {
        return MaaNullSize;
    }
    memcpy(buff, uuid.c_str(), size);
    return size;
}

MaaInstanceHandle MaaCreate(MaaInstanceCallback callback, void* callback_arg)
{
    LogFunc << VAR_VOIDP(callback) << VAR_VOIDP(callback_arg);

    return new MAA_NS::InstanceMgr(callback, callback_arg);
}

void MaaDestroy(MaaInstanceHandle* inst)
{
    LogFunc << VAR_VOIDP(inst);

    if (inst == nullptr || *inst == nullptr) {
        return;
    }
    delete *inst;
    *inst = nullptr;
}

MaaBool MaaSetOption(MaaInstanceHandle inst, MaaInstOption key, MaaString value)
{
    LogFunc << VAR_VOIDP(inst) << VAR(key) << VAR(value);

    if (!inst) {
        return false;
    }
    return inst->set_option(key, value);
}

MaaBool MaaBindResource(MaaInstanceHandle inst, MaaResourceHandle res)
{
    LogFunc << VAR_VOIDP(inst) << VAR_VOIDP(res);

    if (!inst || !res) {
        return false;
    }
    return inst->bind_resource(res);
}

MaaBool MaaBindController(MaaInstanceHandle inst, MaaControllerHandle ctrl)
{
    LogFunc << VAR_VOIDP(inst) << VAR_VOIDP(ctrl);

    if (!inst || !ctrl) {
        return false;
    }
    return inst->bind_controller(ctrl);
}

MaaBool MaaInited(MaaInstanceHandle inst)
{
    LogFunc << VAR_VOIDP(inst);
    if (!inst) {
        return false;
    }
    return inst->inited();
}

MaaTaskId MaaPostTask(MaaInstanceHandle inst, MaaTaskType type, MaaJsonString param)
{
    LogFunc << VAR_VOIDP(inst) << VAR(type) << VAR(param);

    if (!inst) {
        return MaaInvalidId;
    }
    return inst->post_task(type, param);
}

MaaBool MaaSetTaskParam(MaaInstanceHandle inst, MaaTaskId id, MaaJsonString param)
{
    LogFunc << VAR_VOIDP(inst) << VAR(id) << VAR(param);

    if (!inst) {
        return false;
    }
    return inst->set_task_param(id, param);
}

MaaStatus MaaTaskStatus(MaaInstanceHandle inst, MaaTaskId id)
{
    // LogFunc << VAR_VOIDP(inst) << VAR(id);

    if (!inst) {
        return MaaStatus_Invalid;
    }
    return inst->status(id);
}

MaaStatus MaaTaskWait(MaaInstanceHandle inst, MaaTaskId id)
{
    // LogFunc << VAR_VOIDP(inst) << VAR(id);

    if (!inst) {
        return MaaStatus_Invalid;
    }
    return inst->wait(id);
}

MaaBool MaaTaskAllFinished(MaaInstanceHandle inst)
{
    // LogFunc << VAR_VOIDP(inst) << VAR(id);
    if (!inst) {
        return false;
    }
    return inst->all_finished();
}

void MaaStop(MaaInstanceHandle inst)
{
    LogFunc << VAR_VOIDP(inst);

    if (!inst) {
        return;
    }

    inst->stop();
}

MaaSize MaaGetResourceHash(MaaInstanceHandle inst, char* buff, MaaSize buff_size)
{
    LogFunc << VAR_VOIDP(inst) << VAR_VOIDP(buff) << VAR(buff_size);

    if (!inst || !buff) {
        return MaaNullSize;
    }
    auto hash = inst->get_resource_hash();
    size_t size = hash.size();
    if (size >= buff_size) {
        return MaaNullSize;
    }
    memcpy(buff, hash.c_str(), size);
    return size;
}

MaaSize MaaGetControllerUUID(MaaInstanceHandle inst, char* buff, MaaSize buff_size)
{
    LogFunc << VAR_VOIDP(inst) << VAR_VOIDP(buff) << VAR(buff_size);

    if (!inst || !buff) {
        return MaaNullSize;
    }
    auto uuid = inst->get_controller_uuid();
    size_t size = uuid.size();
    if (size >= buff_size) {
        return MaaNullSize;
    }
    memcpy(buff, uuid.c_str(), size);
    return size;
}

MaaString MaaVersion()
{
    return MAA_VERSION;
}
