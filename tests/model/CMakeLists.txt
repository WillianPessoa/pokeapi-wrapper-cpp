# tests/model/CMakeLists.txt

enable_testing()

add_executable(test_pokemon
    test_pokemon.cpp
    ${CMAKE_SOURCE_DIR}/model/pokemon.cpp
)

target_include_directories(test_pokemon
    PRIVATE
        ${CMAKE_SOURCE_DIR}/model
)

target_link_libraries(test_pokemon
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Test
)

add_test(NAME PokemonTests COMMAND test_pokemon)
