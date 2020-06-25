cmake_minimum_required(VERSION 3.5)
project(pixivtime)
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

include_directories(extern/nlohmann-json/include)

include(conan_build/conanbuildinfo.cmake)
conan_basic_setup()

include(extern/cqcppsdk/cqcppsdk.cmake)
cq_set_app_id("top.sevenshape.pixivtime")

if (CQ_CAN_BUILD_STD_MODE)
    cq_add_std_app(app.cpp http.cpp api.cpp regex.cpp)
    target_link_libraries(app ${CONAN_LIBS})
    cq_install_std_app()
endif ()

cq_add_dev_app(app.cpp http.cpp api.cpp regex.cpp)
target_link_libraries(app_dev ${CONAN_LIBS})

add_executable(pixivtime main.cpp http.cpp api.cpp regex.cpp)
target_link_libraries(pixivtime ${CONAN_LIBS})