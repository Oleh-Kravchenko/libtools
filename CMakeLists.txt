CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(libtools VERSION 0.0.4)

INCLUDE(GNUInstallDirs)
INCLUDE(CMakePackageConfigHelpers)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
SET(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${PROJECT_VERSION}")
SET(INSTALL_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
SET(INSTALL_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")

FIND_PACKAGE(OpenSSL REQUIRED)

# configure MySQL support
IF(DEFINED WITH_MYSQL)
	IF(WITH_MYSQL)
		FIND_PACKAGE(MySQL REQUIRED)
	ENDIF()
ELSE()
	FIND_PACKAGE(MySQL)
ENDIF()

# configure uriparser support
IF(DEFINED WITH_URIPARSER)
	IF(WITH_URIPARSER)
		FIND_PACKAGE(PkgConfig REQUIRED)
		PKG_CHECK_MODULES(URIPARSER REQUIRED liburiparser)
	ENDIF()
ELSE()
	FIND_PACKAGE(PkgConfig)
	IF (PKG_CONFIG_FOUND)
		PKG_CHECK_MODULES(URIPARSER liburiparser)
	ENDIF()
ENDIF()

# configure documentation support
IF(DEFINED WITH_DOC)
	IF(WITH_DOC)
		FIND_PACKAGE(Doxygen REQUIRED)
	ENDIF()
ELSE()
	FIND_PACKAGE(Doxygen)
ENDIF()

# catch lazy errors during compilation and enable GNU extensions
ADD_DEFINITIONS(-pedantic -std=gnu99 -Wall -Wextra -Werror -D_GNU_SOURCE)

INCLUDE_DIRECTORIES(
include
${OPENSSL_INCLUDE_DIR}
)

SET(TOOLS_HEADERS
include/${PROJECT_NAME}/closeall.h
include/${PROJECT_NAME}/daemonize.h
include/${PROJECT_NAME}/directory.h
include/${PROJECT_NAME}/dir_remove.h
include/${PROJECT_NAME}/dump.h
include/${PROJECT_NAME}/env.h
include/${PROJECT_NAME}/executefile.h
include/${PROJECT_NAME}/file.h
include/${PROJECT_NAME}/flock.h
include/${PROJECT_NAME}/gethostaddr.h
include/${PROJECT_NAME}/hex.h
include/${PROJECT_NAME}/http.h
include/${PROJECT_NAME}/iface.h
include/${PROJECT_NAME}/input.h
include/${PROJECT_NAME}/list.h
include/${PROJECT_NAME}/mailbox.h
include/${PROJECT_NAME}/mkpasswd.h
include/${PROJECT_NAME}/movefile.h
include/${PROJECT_NAME}/popen2.h
include/${PROJECT_NAME}/recvall.h
include/${PROJECT_NAME}/recvfile.h
include/${PROJECT_NAME}/recvline.h
include/${PROJECT_NAME}/re.h
include/${PROJECT_NAME}/sendfile.h
include/${PROJECT_NAME}/sha1.h
include/${PROJECT_NAME}/strexp.h
include/${PROJECT_NAME}/string.h
include/${PROJECT_NAME}/tools.h
include/${PROJECT_NAME}/trim.h
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
source/input.c
source/mailbox.c
source/mkpasswd.c
source/movefile.c
source/popen2.c
source/re.c
source/recvall.c
source/recvfile.c
source/recvline.c
source/sendfile.c
source/sha1.c
source/strexp.c
source/string.c
source/trim.c
)

IF(MYSQL_FOUND)
	INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
	LIST(APPEND TOOLS_HEADERS include/${PROJECT_NAME}/mysql.h)
	LIST(APPEND TOOLS_SOURCES source/mysql.c)
ENDIF()

IF(URIPARSER_FOUND)
	INCLUDE_DIRECTORIES(${URIPARSER_INCLUDE_DIRS})
	LIST(APPEND TOOLS_HEADERS include/${PROJECT_NAME}/url.h)
	LIST(APPEND TOOLS_SOURCES source/url.c)
ENDIF()

ADD_LIBRARY(libtools_objects OBJECT
${TOOLS_HEADERS}
${TOOLS_SOURCES}
)

# share object files between shard and static libraries
SET_PROPERTY(TARGET libtools_objects PROPERTY COMPILE_FLAGS "-fPIC")

# define static library
ADD_LIBRARY(libtools_static STATIC $<TARGET_OBJECTS:libtools_objects>)

SET_TARGET_PROPERTIES(libtools_static PROPERTIES OUTPUT_NAME "tools")
SET_TARGET_PROPERTIES(libtools_static PROPERTIES VERSION "${PROJECT_VERSION}")
SET_TARGET_PROPERTIES(libtools_static PROPERTIES SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

TARGET_INCLUDE_DIRECTORIES(libtools_static
INTERFACE
	$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

TARGET_LINK_LIBRARIES(libtools_static
PRIVATE
	${MYSQL_LIBRARY}
	${OPENSSL_LIBRARIES}
	${URIPARSER_LIBRARIES}
	pthread
)

# define shared library
ADD_LIBRARY(libtools SHARED $<TARGET_OBJECTS:libtools_objects>)

SET_TARGET_PROPERTIES(libtools PROPERTIES OUTPUT_NAME "tools")
SET_TARGET_PROPERTIES(libtools PROPERTIES VERSION "${PROJECT_VERSION}")
SET_TARGET_PROPERTIES(libtools PROPERTIES SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

TARGET_INCLUDE_DIRECTORIES(libtools
INTERFACE
	$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

TARGET_LINK_LIBRARIES(libtools
PRIVATE
	${MYSQL_LIBRARY}
	${OPENSSL_LIBRARIES}
	${URIPARSER_LIBRARIES}
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
INSTALL(TARGETS libtools
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

INSTALL(FILES
	${TOOLS_HEADERS}
DESTINATION
	"${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}"
COMPONENT
	Devel
)

INSTALL(FILES
"${INSTALL_VERSION_FILE}"
"${INSTALL_CONFIG_FILE}"
DESTINATION
	"${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}"
COMPONENT
	Devel
)

INSTALL(EXPORT "${PROJECT_NAME}"
FILE
	${PROJECT_NAME}-targets.cmake
DESTINATION
	"${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}"
COMPONENT Devel
)

# generate man pages, if Doxygen available
IF(DOXYGEN_FOUND)
	SET(TOOLS_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/doxygen.h")
	FOREACH(TOOLS_HEADER ${TOOLS_HEADERS})
		SET(TOOLS_HEADER_FILES "${TOOLS_HEADER_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${TOOLS_HEADER}")
	ENDFOREACH()

	CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/Doxyfile.in"
	"${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
	@ONLY
	)

	ADD_CUSTOM_TARGET(${PROJECT_NAME}_doc ALL
	"${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
	WORKING_DIRECTORY
		${CMAKE_CURRENT_BINARY_DIR}
	COMMENT
		"Generating API documentation with Doxygen"
	SOURCES
		${TOOLS_HEADERS}
	)

	INSTALL(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/"
	DESTINATION
		"${CMAKE_INSTALL_DOCDIR}"
	COMPONENT
		Devel
	)
ENDIF()
