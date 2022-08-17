add_executable(opengl-fun main.cpp glad.c)

find_package(OpenGL REQUIRED)
find_package(GLEW 2.0 REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

include_directories(/opt/homebrew/include ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} )
link_directories(/opt/homebrew/lib/ ../lib)
include_directories(../include)

target_link_libraries(opengl-fun ${OPENGL_LIBRARIES} glfw)
