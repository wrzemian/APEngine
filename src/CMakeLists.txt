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
		../include/Shader.h
		lights/AmbientLight.cpp ../include/lights/AmbientLight.h
		lights/DirectionalLight.cpp ../include/lights/DirectionalLight.h
		lights/PointLight.cpp ../include/lights/PointLight.h
		lights/SpotLight.cpp ../include/lights/SpotLight.h
		../include/Objects/Mesh.h
		../include/Objects/Model.h
		../include/Objects/Transform.h
		../include/Objects/Object3D.h Objects/Object3D.cpp
		../include/Camera.h
		Objects/MovingObject.cpp ../include/Objects/MovingObject.h
		Objects/Hitbox.cpp ../include/Objects/Hitbox.h
		../include/Objects/DebugShape.h Objects/DebugShape.cpp
		User/InputSystem.cpp ../include/User/InputSystem.h
		Parser.cpp ../include/Parser.h
		../include/User/HUD.h
		../include/IGui.h IGui.cpp
        Objects/Level.cpp ../include/Objects/Level.h
		User/Animation.cpp
        ../include/User/HudAnimation.h
		User/Constant.cpp ../include/User/Constant.h
		../include/FrustumOpitmizer.h FrustumOptimizer.cpp
		Camera.cpp
		Background/Rock.cpp ../include/Background/Rock.h
		../include/Background/Rock.h Background/Rock.cpp
		Background/Background.cpp ../include/Background/Background.h
		Background/BackgroundTile.cpp ../include/Background/BackgroundTile.h
		Background/Cactus.cpp ../include/Background/Cactus.h ../include/AssetManager.h AssetManager.cpp Objects/Ant.cpp ../include/Objects/Ant.h
		Objects/Platform.cpp ../include/Objects/Platform.h ../include/Objects/Platform.h Objects/Battery.cpp ../include/Objects/Battery.h ../include/Objects/Battery.h Objects/Button.cpp ../include/Objects/Button.h ../include/Objects/Button.h Objects/PlayerJumper.cpp ../include/Objects/PlayerJumper.h ../include/Objects/PlayerJumper.h Objects/SimpleHitbox.cpp ../include/Objects/SimpleHitbox.h Objects/PlayerGrabber.cpp ../include/Objects/PlayerGrabber.h ../include/Objects/PlayerGrabber.h Objects/Grabber.cpp ../include/Objects/Grabber.h ../include/Objects/Grabber.h Objects/Box.cpp ../include/Objects/Box.h ../include/Objects/Box.h Animations/AnimationModel.cpp ../include/Animations/AnimationModel.h ../include/Animations/Bone.cpp ../include/Animations/Bone.h ../include/Animations/Animation.cpp ../include/Animations/Animation.h ../include/Animations/GLConverter.cpp ../include/Animations/Animator.cpp ../include/Animations/Animator.h ../include/Animations2/Bone.cpp ../include/Animations2/Bone.h ../include/Animations2/AssimpGLMHelpers.h ../include/Animations2/Animation.cpp ../include/Animations2/Animation.h ../include/Animations2/Animator.cpp ../include/Animations2/Animator.h lights/Shadows.cpp ../include/lights/Shadows.h Objects/WinArea.cpp ../include/Objects/WinArea.h ../include/Objects/WinArea.h LevelManager.cpp ../include/LevelManager.h Audio/AudioLoader.cpp ../include/Audio/AudioLoader.h Audio/AudioListener.cpp ../include/Audio/AudioListener.h Audio/AudioSource.cpp ../include/Audio/AudioSource.h Audio/AudioManager.cpp ../include/Audio/AudioManager.h Audio/AudioSource.cpp ../include/Audio/AudioSource.h Audio/AudioListener.cpp ../include/Audio/AudioListener.h Objects/LightBulb.cpp ../include/Objects/LightBulb.h ../include/Objects/LightBulb.h Objects/LightBulb.cpp Objects/Cable.cpp ../include/Objects/Cable.h)



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

set_target_properties(OpenAL
		PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(OpenAL
		PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
				   COMMAND ${CMAKE_COMMAND} -E create_symlink 
				   ${CMAKE_SOURCE_DIR}/res 
				   ${CMAKE_CURRENT_BINARY_DIR}/res)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
endif()
