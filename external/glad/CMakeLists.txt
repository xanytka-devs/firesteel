cmake_minimum_required(VERSION 3.12)
project(glad)

option(GLAD_VULKAN "Include Vulkan context" OFF)
option(GLAD_GLES "Include GLES context" OFF)
option(GLAD_XLIB "Include GLX context (Needed for X11 in Linux)" OFF)

set(GLAD_HEADERS
	include/KHR/khrplatform.h
	include/glad/gl.h
)
if(GLAD_GLES)
	set(GLAD_HEADERS ${GLAD_HEADERS}
		include/glad/gles2.h
	)
endif()
if(GLAD_XLIB)
	set(GLAD_HEADERS ${GLAD_HEADERS}
		include/glad/glx.h
	)
endif()
if(GLAD_VULKAN)
	set(GLAD_HEADERS ${GLAD_HEADERS}
		include/vk_platform.h
		include/glad/vulkan.h
	)
endif()
set(GLAD_SRCS
	src/gl.c
)
if(GLAD_GLES)
	set(GLAD_SRCS ${GLAD_SRCS}
		src/gles2.c
	)
endif()
if(GLAD_XLIB)
	set(GLAD_SRCS ${GLAD_SRCS}
		src/glx.c
	)
endif()
if(GLAD_VULKAN)
	set(GLAD_SRCS ${GLAD_SRCS}
		src/vulkan.c
	)
endif()

add_library(glad STATIC
	${GLAD_HEADERS}
	${GLAD_SRCS}	
)

target_include_directories(glad PUBLIC include)