cmake_minimum_required(VERSION 3.13)

project(dgus_hmi C)

file(GLOB_RECURSE DGUS_HMI_WIDGET_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/widgets/*.c
)

add_library(dgus_hmi STATIC
    src/dgus.c
    src/dgus_ui.c
    ${DGUS_HMI_WIDGET_SOURCES}
    src/dgus_anim.c
    src/dgus_runtime.c
)

target_include_directories(dgus_hmi
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/widgets
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/widgets/common
)

target_compile_features(dgus_hmi PUBLIC c_std_99)

option(DGUS_HMI_BUILD_EXAMPLES "Build the DGUS HMI example application" OFF)

if(DGUS_HMI_BUILD_EXAMPLES)
    file(GLOB_RECURSE DGUS_HMI_EXAMPLE_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/examples/*.c
    )

    add_executable(dgus_hmi_examples ${DGUS_HMI_EXAMPLE_SOURCES})
    target_include_directories(dgus_hmi_examples PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/examples
    )
    target_link_libraries(dgus_hmi_examples PRIVATE dgus_hmi)
endif()
