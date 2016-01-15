#.rst:
# FindMySQL
# --------
#
# Find mysqlclient
#
# Find the native MySQL includes and library.
#
# ::
#
#  MYSQL_INCLUDE_DIR - where to find mysql/mysql.h, etc.
#  MYSQL_LIBRARY     - the name of the library to link against.
#  MYSQL_FOUND       - true if MySQL found.
#
# Example usage:
#
# ::
#
#    FIND_PACKAGE(MySQL)
#    or
#    FIND_PACKAGE(MySQL REQUIRED)

IF(MYSQL_INCLUDE_DIR)
	SET(MYSQL_FIND_QUIETLY TRUE)
ENDIF()

SET(MYSQL_LIBRARY_NAME mysqlclient)

FIND_PATH(MYSQL_INCLUDE_DIR mysql/mysql.h
/usr/local/include
/usr/include
)

FIND_LIBRARY(MYSQL_LIBRARY ${MYSQL_LIBRARY_NAME}
PATHS
	/usr/lib
	/usr/local/lib
)

IF(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
	SET(MYSQL_FOUND TRUE)
ELSE()
	SET(MYSQL_FOUND FALSE)
ENDIF()

IF(MYSQL_FOUND)
	IF(NOT MYSQL_FIND_QUIETLY)
		MESSAGE(STATUS "Found MySQL: ${MYSQL_LIBRARY}")
	ENDIF()
ELSEIF(MySQL_FIND_REQUIRED)
	MESSAGE(STATUS "Looked for MySQL libraries named ${MYSQL_LIBRARY_NAME}.")
	MESSAGE(FATAL_ERROR "Could NOT find MySQL library")
ENDIF()

MARK_AS_ADVANCED(MYSQL_INCLUDE_DIR MYSQL_LIBRARY)
