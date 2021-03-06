cmake_minimum_required (VERSION 3.0)
project ( piwebcam )

set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_MULTITHREADED ON )
set( Boost_USE_STATIC_RUNTIME OFF )

find_package( OpenCV )
find_package( Boost COMPONENTS thread date_time system chrono )

FILE( GLOB ROOT_INC_FILES "${CMAKE_SOURCE_DIR}/src/*.h" )
FILE( GLOB ROOT_SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" )

source_group( include FILES ${ROOT_INC_FILES} )
source_group( src FILES ${ROOT_SRC_FILES} )

include_directories(
    ${INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
)

link_directories(
	debug ${LIB_DIRS_DBG} optimized ${LIB_DIRS_REL}
	${BOOST_THREAD_LIBRARY}
)

add_executable(
	piwebcam
	${ROOT_INC_FILES}
	${ROOT_SRC_FILES}
)

target_link_libraries(
	piwebcam
	${LIBS}
	debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE}
	debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE}
	debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE}
	debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE}
	opencv_core
	opencv_videoio
)

