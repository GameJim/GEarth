cmake_minimum_required(VERSION 3.5)
# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project (GEarh)

# Add sub directories
add_subdirectory(plugs)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(ui)

# set_property(TARGET "src" PROPERTY FOLDER core)
# set_property(TARGET "plugs" PROPERTY FOLDER plugs)
# set_property(TARGET "test" PROPERTY FOLDER test)
# set_property(TARGET "ui" PROPERTY FOLDER ui)