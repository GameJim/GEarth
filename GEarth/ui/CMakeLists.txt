cmake_minimum_required(VERSION 3.5)
project(ui)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add sub directories
add_subdirectory(symboldesigner)
set_property(TARGET SymbolDesigner PROPERTY FOLDER ui)



