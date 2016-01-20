CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(libtools VERSION 0.0.1)

INCLUDE(GNUInstallDirs)
INCLUDE(CMakePackageConfigHelpers)

SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
SET(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${PROJECT_VERSION}")
SET(INSTALL_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
SET(INSTALL_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")

FIND_PACKAGE(Doxygen)
FIND_PACKAGE(MySQL REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)

# catch lazy errors during compilation and enable GNU extensions
ADD_DEFINITIONS(-pedantic -std=gnu99 -Wall -Wextra -Werror -D_GNU_SOURCE)

INCLUDE_DIRECTORIES(include)

SET(TOOLS_HEADERS
include/tools/closeall.h
include/tools/daemonize.h
include/tools/directory.h
include/tools/dir_remove.h
include/tools/dump.h
include/tools/env.h
include/tools/executefile.h
include/tools/file.h
include/tools/flock.h
include/tools/gethostaddr.h
include/tools/hex.h
include/tools/http.h
include/tools/iface.h
include/tools/list.h
include/tools/mailbox.h
include/tools/mkpasswd.h
include/tools/movefile.h
include/tools/mysql.h
include/tools/recvall.h
include/tools/recvfile.h
include/tools/recvline.h
include/tools/re.h
include/tools/sendfile.h
include/tools/sha1.h
include/tools/string.h
include/tools/tools.h
include/tools/trim.h
include/tools/url.h
)

SET(TOOLS_SOURCES
source/closeall.c
source/daemonize.c
source/directory.c
source/dir_remove.c
source/dump.c
source/env.c
source/executefile.c
source/file.c
source/flock.c
source/gethostaddr.c
source/hex.c
source/http.c
source/iface.c
source/mailbox.c
source/mkpasswd.c
source/movefile.c
source/mysql.c
source/re.c
source/recvall.c
source/recvfile.c
source/recvline.c
source/sendfile.c
source/sha1.c
source/string.c
source/trim.c
source/url.c
)

ADD_LIBRARY(tools OBJECT
${TOOLS_HEADERS}
${TOOLS_SOURCES}
)

# share object files between shard and static libraries
SET_PROPERTY(TARGET tools PROPERTY COMPILE_FLAGS "-fPIC")

# define static library
ADD_LIBRARY(libtools_static STATIC $<TARGET_OBJECTS:tools>)

SET_TARGET_PROPERTIES(libtools_static PROPERTIES OUTPUT_NAME "tools")
SET_TARGET_PROPERTIES(libtools_static PROPERTIES VERSION "${PROJECT_VERSION}")
SET_TARGET_PROPERTIES(libtools_static PROPERTIES SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

TARGET_INCLUDE_DIRECTORIES(libtools_static
PUBLIC
	$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# define shared library
ADD_LIBRARY(libtools_shared SHARED $<TARGET_OBJECTS:tools>)

SET_TARGET_PROPERTIES(libtools_shared PROPERTIES OUTPUT_NAME "tools")
SET_TARGET_PROPERTIES(libtools_shared PROPERTIES VERSION "${PROJECT_VERSION}")
SET_TARGET_PROPERTIES(libtools_shared PROPERTIES SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

TARGET_INCLUDE_DIRECTORIES(libtools_shared
PUBLIC
	$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

TARGET_LINK_LIBRARIES(libtools_shared
PRIVATE
	${MYSQL_LIBRARY}
	${OPENSSL_LIBRARIES}
	pthread
)

# generate package version and configuration files
WRITE_BASIC_PACKAGE_VERSION_FILE("${INSTALL_VERSION_FILE}"
VERSION
	${PROJECT_VERSION}
COMPATIBILITY
	ExactVersion
)

CONFIGURE_PACKAGE_CONFIG_FILE("cmake/${PROJECT_NAME}-config.cmake.in"
"${INSTALL_CONFIG_FILE}"
INSTALL_DESTINATION
	"${CMAKE_INSTALL_LIBDIR}"
)

# install Runtime
INSTALL(TARGETS libtools_shared
EXPORT
	"${PROJECT_NAME}"
LIBRARY DESTINATION
	"${CMAKE_INSTALL_LIBDIR}"
COMPONENT
	Runtime
)

# install Devel
INSTALL(TARGETS libtools_static
EXPORT
	"${PROJECT_NAME}"
ARCHIVE DESTINATION
	"${CMAKE_INSTALL_LIBDIR}"
COMPONENT
	Devel
)

INSTALL(DIRECTORY "include/tools"
DESTINATION
	"${INSTALL_INCLUDE_DIR}"
COMPONENT
	Devel
PATTERN *.h
)

INSTALL(FILES
"${INSTALL_VERSION_FILE}"
"${INSTALL_CONFIG_FILE}"
DESTINATION
	"${CMAKE_INSTALL_LIBDIR}/cmake"
COMPONENT
	Devel
)

INSTALL(EXPORT "${PROJECT_NAME}"
FILE
	${PROJECT_NAME}-targets.cmake
DESTINATION
	"${CMAKE_INSTALL_LIBDIR}/cmake"
COMPONENT Devel
)

# generate man pages, if Doxygen available
IF(DOXYGEN_FOUND)
	CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/Doxyfile.in"
	"${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
	@ONLY
	)

	ADD_CUSTOM_TARGET(doc
	"${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
	WORKING_DIRECTORY
		${CMAKE_CURRENT_BINARY_DIR}
	COMMENT
		"Generating API documentation with Doxygen"
	SOURCES
		${TOOLS_HEADERS}
	)

	INSTALL(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man/"
	DESTINATION
		"${CMAKE_INSTALL_MANDIR}"
	COMPONENT
		Devel
	FILES_MATCHING
		PATTERN *.3
	)
ENDIF()
