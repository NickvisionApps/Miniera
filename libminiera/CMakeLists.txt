﻿if(WIN32)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_library(libminiera
    "src/controllers/mainwindowcontroller.cpp"
    "src/controllers/newserverdialogcontroller.cpp"
    "src/controllers/preferencesviewcontroller.cpp"
    "src/controllers/serverviewcontroller.cpp"
    "src/events/serverinitializationprogresschangedeventargs.cpp"
    "src/events/serverloadedeventargs.cpp"
    "src/events/serverversionsloadedeventargs.cpp"
    "src/models/broadcaster.cpp"
    "src/models/configuration.cpp"
    "src/models/server.cpp"
    "src/models/serveraddress.cpp"
    "src/models/servermanager.cpp"
    "src/models/serverproperties.cpp"
    "src/models/serverversion.cpp"
    "src/models/startupinformation.cpp"
    "src/models/zipfile.cpp")
target_include_directories(libminiera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(libminiera PROPERTIES VERSION "${PROJECT_VERSION}" SOVERSION "${PROJECT_VERSION}")
if(MSVC)
    target_compile_options(libminiera PRIVATE /W4)
else()
    target_compile_options(libminiera PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    if(USING_VCPKG)
        find_package(unofficial-wintoast CONFIG REQUIRED)
        target_link_libraries(libminiera PRIVATE unofficial::wintoast::wintoast)
    endif()
endif()
find_package(libnick CONFIG REQUIRED)
find_package(libzip CONFIG REQUIRED)
target_link_libraries(libminiera PUBLIC libnick::libnick libzip::zip)

add_custom_target(shared_commands ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/__shared.h")
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/__shared.h" "${CMAKE_CURRENT_BINARY_DIR}/shared.h" COMMAND "${CMAKE_COMMAND}" -E touch "${CMAKE_CURRENT_BINARY_DIR}/shared.h")
if(HAS_GETTEXT)
    add_custom_command(TARGET shared_commands PRE_BUILD COMMAND "${GETTEXT_XGETTEXT_EXECUTABLE}" --from-code=utf-8 --add-comments --c++ --force-po --output="${POT_FILE}" --keyword=_ --keyword=_n:1,2 --keyword=_p:1c,2 --keyword=_pn:1c,2,3 --keyword=C_:1c,2 --width=80 ${TRANSLATE_FILES} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    foreach(LINGUA IN ITEMS ${LINGUAS})
        add_custom_command(TARGET shared_commands PRE_BUILD COMMAND "${GETTEXT_MSGMERGE_EXECUTABLE}" --backup=off -U "${CMAKE_SOURCE_DIR}/resources/po/${LINGUA}.po" "${POT_FILE}")
    endforeach()
else()
    message(WARNING "gettext not found, translations will not be generated")
endif()
