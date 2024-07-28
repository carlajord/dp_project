project(DeltaP LANGUAGES CXX)

set (SOURCES
    baseobj.h
    fluid.h
    fluid.cpp
    pipe.h
    pipe.cpp
    case.h
    case.cpp
    main.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PUBLIC DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../test/")