cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(core)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add sub directories
add_subdirectory(common)
add_subdirectory(SymbolSystem)
add_subdirectory(RenderSystem)

set_property(TARGET common PROPERTY FOLDER core)
set_property(TARGET SymbolSystem PROPERTY FOLDER core)
set_property(TARGET RenderSystem PROPERTY FOLDER core)