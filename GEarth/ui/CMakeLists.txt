cmake_minimum_required(VERSION 3.5)
project(ui)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0079 NEW)
# Add sub directories
add_subdirectory(symboldesigner)
set_property(TARGET SymbolDesigner PROPERTY FOLDER ui)

add_subdirectory(EarthViewer)
set_property(TARGET EarthViewer PROPERTY FOLDER ui)


target_link_libraries(EarthViewer EarthCore)
target_link_libraries(SymbolDesigner SymbolSystem)
