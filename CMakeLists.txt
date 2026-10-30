cmake_minimum_required(VERSION 3.20)
project(ShopFlow C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra -Werror)

file(GLOB SOURCES "*.c")
file(GLOB HEADERS "*.h")

add_executable(ShopFlow ${SOURCES} ${HEADERS}
        types.h
        produit.h
        produit.c
        client.h
        client.c
        commande.h
        commande.c
        statistique.h
        statistique.c
        utils.h
        utils.c
        initialisation.h
        initialisation.c
)