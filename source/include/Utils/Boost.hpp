#pragma once

#include "Conf/Conf.h"

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 // Win7
#endif
#endif

#define BOOST_PROCESS_USE_STD_FS 1
#define BOOST_DLL_USE_STD_FS 1

MAA_SUPPRESS_BOOST_WARNINGS_BEGIN
#include <boost/asio.hpp>
#include <boost/config.hpp>
#include <boost/dll.hpp>
#include <boost/process.hpp>

#ifdef _WIN32
#include <boost/process/windows.hpp>
#endif

#ifdef BOOST_WANT_UUID
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#endif

#ifdef BOOST_WANT_BEAST
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/url.hpp>
#endif

MAA_SUPPRESS_BOOST_WARNINGS_END
