
include_directories(
    
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
    ${CMAKE_SOURCE_DIR}/projectbuilders/cmakebuilder/
)
add_subdirectory(tests)

# enable this if you want to have the cmake debug visitor run on each CMakeLists.txt
# the debug visitor prints out the Ast for the CMakeLists.txt file.
#add_definitions( -DCMAKEDEBUGVISITOR )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=9032 )

set( cmakecommon_SRCS
  parser/cmListFileLexer.c
  parser/astfactory.cpp
  parser/cmakelistsparser.cpp
  parser/cmakeast.cpp
  parser/cmakecondition.cpp
  parser/cmakeprojectvisitor.cpp 
  parser/variablemap.cpp
  parser/cmakedebugvisitor.cpp
  parser/cmakecachereader.cpp
  parser/cmakeparserutils.cpp
  cmakebuilddirchooser.cpp
  cmakeutils.cpp
 )

set( cmakecommon_UI
  cmakebuilddirchooser.ui
)

set( cmakemanager_SRCS
  cmakemodelitems.cpp
  cmakemanager.cpp
  cmakehighlighting.cpp
  cmakecodecompletionmodel.cpp
  applychangeswidget.cpp
  settings/cmakebuilddircreator.cpp
)

set( cmakemanager_UI
  cmakepossibleroots.ui
  settings/cmakebuilddircreator.ui
)

set( cmakesettings_SRCS
  settings/cmakepreferences.cpp
  settings/cmakecachemodel.cpp
  settings/cmakecachedelegate.cpp
  settings/cmakebuilddircreator.cpp
)

set( cmakesettings_UI
  settings/cmakebuildsettings.ui
  settings/cmakebuilddircreator.ui
)

remove_definitions( -DQT_NO_STL )

# Note: This library doesn't follow API/ABI/BC rules and shouldn't have a SOVERSION
#       Its only purpose is to support the plugin without needing to add all source files
#       to the plugin target

kde4_add_ui_files( cmakecommon_SRCS ${cmakecommon_UI} )
kde4_add_library( kdev4cmakecommon SHARED ${cmakecommon_SRCS} )
target_link_libraries( kdev4cmakecommon ${KDE4_KDEUI_LIBS}
                        ${KDE4_KTEXTEDITOR_LIBS} ${KDEVPLATFORM_INTERFACES_LIBRARY} ${KDEVPLATFORM_PROJECT_LIBRARY}
                        ${QT_QTSCRIPT_LIBRARY} ${KDEVPLATFORM_LANGUAGE_LIBRARY}
                        )

kde4_add_ui_files( cmakemanager_SRCS ${cmakemanager_UI} )
kde4_add_plugin( kdevcmakemanager ${cmakemanager_SRCS} )
target_link_libraries( kdevcmakemanager ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
                        ${KDEVPLATFORM_INTERFACES_LIBRARY} kdev4cmakecommon
                        ${KDEVPLATFORM_PROJECT_LIBRARY} ${KDEVPLATFORM_LANGUAGE_LIBRARY}
                        ${KDE4_KTEXTEDITOR_LIBS})

kde4_add_kcfg_files( cmakesettings_SRCS cmakeconfig.kcfgc )
kde4_add_ui_files( cmakesettings_SRCS ${cmakesettings_UI} )
kde4_add_plugin( kcm_kdevcmake_settings ${cmakesettings_SRCS} )
target_link_libraries( kcm_kdevcmake_settings ${KDE4_KIO_LIBS}
                       ${KDEVPLATFORM_INTERFACES_LIBRARY} ${KDEVPLATFORM_PROJECT_LIBRARY} ${KDE4_KIO_LIBRARY} kdev4cmakecommon )

install(TARGETS kdevcmakemanager DESTINATION ${PLUGIN_INSTALL_DIR} )
install(TARGETS kcm_kdevcmake_settings DESTINATION ${PLUGIN_INSTALL_DIR} )
install(TARGETS kdev4cmakecommon ${INSTALL_TARGETS_DEFAULT_ARGS} )
install(FILES kdevcmakemanager.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install(FILES kcm_kdevcmake_settings.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install(FILES data.kdev4 DESTINATION ${DATA_INSTALL_DIR}/kdevcmakemanager )
