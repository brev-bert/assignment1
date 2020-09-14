#--------------------------------------------------------------------------
# Lighting project
#--------------------------------------------------------------------------

PROJECT(Lighting)
FILE(GLOB Lighting_headers code/*.h)
FILE(GLOB Lighting_sources code/*.cc)

SET(files_Lighting ${Lighting_headers} ${Lighting_sources})
SOURCE_GROUP("Lighting" FILES ${files_Lighting})

ADD_EXECUTABLE(Lighting ${files_Lighting})
TARGET_LINK_LIBRARIES(Lighting core render)
ADD_DEPENDENCIES(Lighting core render)

add_custom_command(
    TARGET Lighting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/
    ${CMAKE_SOURCE_DIR}/bin/resources/
)

set_target_properties(Lighting PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

