cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(core)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add sub directories
add_subdirectory(common)
set_property(TARGET common PROPERTY FOLDER core)

add_subdirectory(SymbolSystem)
set_property(TARGET SymbolSystem PROPERTY FOLDER core)

add_subdirectory(RenderSystem)
set_property(TARGET RenderSystem PROPERTY FOLDER core)

add_subdirectory(EarthCore)
set_property(TARGET EarthCore PROPERTY FOLDER core)




