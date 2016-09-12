cmake_minimum_required (VERSION 3.0)
project ( piwebcam )

FILE( GLOB ROOT_INC_FILES "${CMAKE_SOURCE_DIR}/src/*.h" )
FILE( GLOB ROOT_SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" )

source_group( include FILES ${ROOT_INC_FILES} )
source_group( src FILES ${ROOT_SRC_FILES} )

include_directories( 
    ${INCLUDE_DIRS}
)

link_directories( 
	debug ${LIB_DIRS_DBG} optimized ${LIB_DIRS_REL}
)

add_executable( 
	piwebcam 
	${ROOT_INC_FILES} 
	${ROOT_SRC_FILES} 
)

target_link_libraries( 
	piwebcam
	${LIBS}
)

