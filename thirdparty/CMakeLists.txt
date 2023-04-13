# CPM - package manager
include(CPM)

# OpenGL
find_package(OpenGL REQUIRED)

# glad
set(GLAD_DIR ${CMAKE_CURRENT_LIST_DIR}/glad)
set(glad_SOURCE_DIR ${GLAD_DIR}/include CACHE INTERNAL "")
add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

# stb_image
set(STB_IMAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_image)
set(stb_image_SOURCE_DIR ${STB_IMAGE_DIR} CACHE INTERNAL "")
add_library(stb_image STATIC ${STB_IMAGE_DIR}/stb_image.cpp)
target_include_directories(stb_image PRIVATE ${STB_IMAGE_DIR})

# rapidjson
set(RAPIDJSON_DIR ${CMAKE_CURRENT_LIST_DIR}/rapidjson)
set(rapidjson_SOURCE_DIR ${RAPIDJSON_DIR} CACHE INTERNAL "")
add_library(rapidjson INTERFACE ${RAPIDJSON_DIR}/rapidjson.h)
target_include_directories(rapidjson INTERFACE ${RAPIDJSON_DIR})

# effolkronium (random)
set(EFFOLKRONIUM_DIR ${CMAKE_CURRENT_LIST_DIR}/effolkronium)
set(effolkronium_SOURCE_DIR ${EFFOLKRONIUM_DIR} CACHE INTERNAL "")
add_library(random INTERFACE ${EFFOLKRONIUM_DIR}/random.hpp)
target_include_directories(random INTERFACE ${EFFOLKRONIUM_DIR})

# eventpp
set(EVENTPP_DIR ${CMAKE_CURRENT_LIST_DIR}/eventpp)
set(eventpp ${EVENTPP_DIR} CACHE INTERNAL "")
add_library(eventpp INTERFACE ${EVENTPP_DIR}/eventqueue.h)
target_include_directories(eventpp INTERFACE ${EVENTPP_DIR})

# magic_enum
set(MAGIC_ENUM_DIR ${CMAKE_CURRENT_LIST_DIR}/magic_enum)
set(magic_enum ${MAGIC_ENUM_DIR} CACHE INTERNAL "")
add_library(magic_enum INTERFACE ${MAGIC_ENUM_DIR}/magic_enum.hpp)
target_include_directories(magic_enum INTERFACE ${MAGIC_ENUM_DIR})

# fmt
set(FMT_DIR ${CMAKE_CURRENT_LIST_DIR}/fmt)
set(fmt ${FMT_DIR} CACHE INTERNAL "")
add_library(fmt INTERFACE ${FMT_DIR}/core.h)
target_include_directories(fmt INTERFACE ${FMT_DIR})

# openal
#set(OPENAL_DIR ${CMAKE_CURRENT_LIST_DIR}/openal)
#set(openal ${OPENAL_DIR} CACHE INTERNAL "")
#add_library(openal INTERFACE ${OPENAL_DIR}/include/AL/al.h)
#target_include_directories(openal INTERFACE ${OPENAL_DIR})
#add_subdirectory(openal)
#set(FMT_DIR ${CMAKE_CURRENT_LIST_DIR}/fmt)
#set(fmt ${FMT_DIR}/include CACHE INTERNAL "")
#add_library(fmt STATIC ${FMT_DIR}/src/fmt.cc)
#target_include_directories(fmt PRIVATE ${FMT_DIR}/include)

# other
CPMAddPackage("gh:assimp/assimp@5.2.5")
CPMAddPackage("gh:glfw/glfw#3.3.8")
CPMAddPackage("gh:g-truc/glm#0.9.9.8")
CPMAddPackage("gh:ocornut/imgui@1.88")
CPMAddPackage("gh:gabime/spdlog@1.10.0")
CPMAddPackage("gh:aseprite/freetype2#VER-2-10-0")
#CPMAddPackage("gh:kcat/openal-soft#1.23.0")


set(imgui_SOURCE_DIR ${imgui_SOURCE_DIR} CACHE INTERNAL "")
add_library(imgui STATIC ${imgui_SOURCE_DIR}/imgui.cpp
					     ${imgui_SOURCE_DIR}/imgui_demo.cpp
					     ${imgui_SOURCE_DIR}/imgui_draw.cpp
						 ${imgui_SOURCE_DIR}/imgui_tables.cpp
					     ${imgui_SOURCE_DIR}/imgui_widgets.cpp)
						 
set_target_properties(glad
                      stb_image 
                      assimp 
                      glfw 
                      glm 
                      imgui 
                      spdlog
		              rapidjson
		random
		eventpp
		magic_enum
		fmt
		freetype
#		OpenAL
		PROPERTIES FOLDER "thirdparty")

if (TARGET zlibstatic)
    set_target_properties(zlibstatic PROPERTIES FOLDER "thirdparty")
endif()