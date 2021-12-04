cmake_minimum_required(VERSION 3.10)
project(aoc_c LANGUAGES C)

foreach(year 2021)
    file(GLOB SOURCES src/*.c src/${year}/*.c)
    file(GLOB INPUTS inputs/${year}/input*.txt)

    foreach(input ${INPUTS})
        get_filename_component(BASENAME_WE ${input} NAME_WE)
        get_filename_component(BASENAME ${input} NAME)
        get_filename_component(DIR ${input} DIRECTORY)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${year})
        set(INPUT_SRC ${CMAKE_BINARY_DIR}/${year}/${BASENAME_WE}.c)
        list(APPEND SOURCES ${INPUT_SRC})

        add_custom_command(
            OUTPUT ${INPUT_SRC}
            COMMAND xxd -i ${BASENAME} ${INPUT_SRC}
            WORKING_DIRECTORY ${DIR})
    endforeach()

    add_executable(aoc${year} ${SOURCES})
    target_include_directories(aoc${year} PUBLIC src src/${year})

endforeach()
