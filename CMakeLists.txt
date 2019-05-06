cmake_minimum_required(VERSION 3.4.1)

project (OpenGL-Learn)

# find_package查找时需要用到CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
	# exe输出路径
	#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

set(Include_Path 
	${CMAKE_SOURCE_DIR}/thirdParty/GLFW/include
	${CMAKE_SOURCE_DIR}/thirdParty/glad/include
)

set(Lib_Path
	${CMAKE_SOURCE_DIR}/thirdParty/GLFW/lib
)

if(WIN32)
  set(LIBS glfw3 opengl32)
endif(WIN32)

# glad
add_library(GLAD "thirdParty/glad/src/glad.c")
set(LIBS ${LIBS} GLAD)

set(CHAPTERS
    basic
)

set(basic
	init_window
)

include_directories(${Include_Path})
link_directories(${Lib_Path})

foreach(CHAPTER ${CHAPTERS})
    foreach(DEMO ${${CHAPTER}})
        file(GLOB SOURCE
            "src/${CHAPTER}/${DEMO}/*.h"
            "src/${CHAPTER}/${DEMO}/*.cpp"
            "src/${CHAPTER}/${DEMO}/*.vs"
            "src/${CHAPTER}/${DEMO}/*.fs"
            "src/${CHAPTER}/${DEMO}/*.gs"
        )
		set(NAME "${CHAPTER}__${DEMO}")
		add_executable(${NAME} ${SOURCE})
        target_link_libraries(${NAME} ${LIBS})
		if(WIN32)
            #set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CHAPTER}")
		endif(WIN32)

	endforeach(DEMO)
endforeach(CHAPTER)