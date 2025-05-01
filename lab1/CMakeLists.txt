cmake_minimum_required(VERSION 3.11)

project(Matrix CXX)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Специфичные флаги для MSVC
if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

    # Выровнять Warning level на /W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    # Отключаем inline-оптимизации (/Ob0) для упрощённой отладки
    set(CMAKE_CXX_FLAGS " /Ob0 ${CMAKE_CXX_FLAGS} ")
endif()

# Ищем поддержку OpenMP
find_package(OpenMP REQUIRED)

# Собираем динамическую библиотеку Matrix
add_library(Matrix SHARED
    include/Matrix.h
    src/Matrix.cc
)

# Директория для include-файлов
target_include_directories(Matrix
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Подключаем OpenMP к библиотеке Matrix
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP support found: ${OpenMP_CXX_FLAGS}")
    # Связываем с рантаймом OpenMP и добавляем флаги компиляции
    target_link_libraries(Matrix
        PUBLIC OpenMP::OpenMP_CXX
    )
    target_compile_options(Matrix
        PUBLIC ${OpenMP_CXX_FLAGS}
    )
endif()

# Собираем исполняемый тестовый файл
add_executable(MatrixExe
    src/MatrixTests.cc
)

# Линкуем тестовый exe с библиотекой Matrix (она уже тянет OpenMP)
target_link_libraries(MatrixExe
    PRIVATE Matrix
)
