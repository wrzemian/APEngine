# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	 *.c
	 *.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	 *.h
	 *.hpp)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES}
		Engine.cpp ../include/Engine.h
		Game.cpp ../include/Game.h
		Renderer.cpp
		../include/Shader.h
		Sierpinski.cpp
		../include/Mesh.h ../include/Model.h lights/AmbientLight.cpp ../include/lights/AmbientLight.h
		lights/DirectionalLight.cpp ../include/lights/DirectionalLight.h lights/PointLight.cpp
		../include/lights/PointLight.h lights/SpotLight.cpp ../include/lights/SpotLight.h ../include/Transform.h
		../include/Scene.h ../include/Object3D.h Object3D.cpp ../include/Camera.h MovingObject.cpp ../include/MovingObject.h Hitbox.cpp ../include/Hitbox.h ../include/DebugShape.h DebugShape.cpp InputSystem.cpp ../include/InputSystem.h Parser.cpp ../include/Parser.h ../include/HUD.h ../include/IHitbox.h ../include/IGui.h IGui.cpp IHitbox.cpp Wall.cpp ../include/Wall.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
												  ${glad_SOURCE_DIR}
												  ${stb_image_SOURCE_DIR}
												  ${imgui_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} stb_image)
target_link_libraries(${PROJECT_NAME} assimp)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} spdlog)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} rapidjson)
target_link_libraries(${PROJECT_NAME} random)
target_link_libraries(${PROJECT_NAME} eventpp)
target_link_libraries(${PROJECT_NAME} magic_enum)
target_link_libraries(${PROJECT_NAME} fmt)
target_link_libraries(${PROJECT_NAME} freetype)
target_link_libraries(${PROJECT_NAME} OpenAL::OpenAL)
target_link_libraries(${PROJECT_NAME} AudioFile)
#target_link_libraries(${PROJECT_NAME} glut)
#target_link_libraries(${PROJECT_NAME} openal)
#target_link_libraries(${OPENAL_DIR}/resources/openal32.rc)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
				   COMMAND ${CMAKE_COMMAND} -E create_symlink 
				   ${CMAKE_SOURCE_DIR}/res 
				   ${CMAKE_CURRENT_BINARY_DIR}/res)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
endif()
