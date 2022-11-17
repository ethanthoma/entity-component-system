cmake_minimum_required(VERSION 3.21)
project(ecs)

set(CMAKE_CXX_STANDARD 17)

add_executable(ecs src/main.cpp src/entity_manager.h src/entity.h src/entity_manager.cpp src/component_manager.h src/component.h src/component_container.h src/scene.h src/bitmask.h src/archetype.h src/either.h src/command.h src/bitmask_graph.h)
