cmake_minimum_required(VERSION 3.20)
project(50_10 C)

set(CMAKE_C_STANDARD 11)

add_executable(50_10 Inicio.c)

# Copia o ficheiro de dados para a pasta onde o executavel corre
configure_file(${CMAKE_SOURCE_DIR}/dados_glicemia.txt ${CMAKE_BINARY_DIR}/dados_glicemia.txt COPYONLY)
