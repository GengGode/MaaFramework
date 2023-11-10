file(GLOB_RECURSE maa_restful_src *.h *.hpp *.cpp)

add_executable(MaaRestful ${maa_restful_src})

target_include_directories(
    MaaRestful
    INTERFACE ../../include
    PRIVATE . ../include ../../include)

target_link_libraries(MaaRestful MaaFramework MaaToolKit MaaUtils HeaderOnlyLibraries Boost::system)

install(
    TARGETS MaaRestful
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${maa_restful_src})
