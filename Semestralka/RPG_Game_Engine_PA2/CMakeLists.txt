cmake_minimum_required(VERSION 3.16)
project(RPG_Game_Engine_PA2)

# Compiler flags
set(CMAKE_CXX_STANDARD 14)
set(GXX_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -Wno-long-long -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GXX_COVERAGE_COMPILE_FLAGS}" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/jorge/Documents/School/CVUT/2020_LS/PA2/RPG_Game_Engine_PA2/build")

set(MAIN_SOURCE
        src/main.cpp)

set(GAME_SOURCE
        src/CGame.cpp           src/CGame.hpp)

set(ENGINE_SOURCE
        src/CMap.cpp            src/CMap.hpp
        src/CVector2D.cpp       src/CVector2D.hpp
        src/CCollisions.cpp     src/CCollisions.hpp
        src/CAssetFactory.cpp   src/CAssetFactory.hpp)

set(INTERFACE_SOURCES
        src/interface/CFpsCapper.cpp    src/interface/CFpsCapper.hpp
        src/CTextureManager.cpp         src/CTextureManager.hpp
        src/interface/CLogger.cpp       src/interface/CLogger.hpp
        src/interface/CMainMenu.cpp     src/interface/CMainMenu.hpp
        src/interface/CExitScreen.cpp   src/interface/CExitScreen.hpp)

set(ECS_SOURCE
        src/EntityComponents/EntityComponent.hpp    src/EntityComponents/Components.hpp
        src/EntityComponents/CSpriteComponent.hpp   src/EntityComponents/CTransformationComponent.hpp
        src/EntityComponents/CKeyboardComponent.hpp src/EntityComponents/CCollisionComponent.hpp
        src/EntityComponents/CTileComponent.hpp     src/EntityComponents/EntityComponent.cpp
        src/EntityComponents/CAIComponent.hpp       src/EntityComponents/CHealthComponent.hpp
        src/EntityComponents/CUIComponent.hpp src/EntityComponents/CItemComponent.hpp)

set(CONFIG_SOURCE
        src/RPGconfig.hpp)

set(SOURCE_FILES ${MAIN_SOURCE} ${GAME_SOURCE} ${INTERFACE_SOURCES} ${ENGINE_SOURCE} ${ECS_SOURCE} ${CONFIG_SOURCE})

add_executable(RPG_Game_Engine_PA2 ${SOURCE_FILES})

# Link SDL library
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image SDL2_ttf)