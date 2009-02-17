
add_definitions( -DKDE_DEFAULT_DEBUG_AREA=9032 )
include_directories(
    ${CMAKE_SOURCE_DIR}/projectbuilders/makebuilder
    ${CMAKE_SOURCE_DIR}/projectmanagers/cmake
    )


########### next target ###############

set(cmakebuilder_LIB_SRCS
    cmakebuilder.cpp
    cmakejob.cpp
    configureandbuildjob.cpp
)

kde4_add_plugin(kdevcmakebuilder ${cmakebuilder_LIB_SRCS})
target_link_libraries(
    kdevcmakebuilder
    kdev4cmakecommon
        ${KDE4_KIO_LIBS}
        ${KDE4_KDEUI_LIBS}
        ${QT_QTDESIGNER_LIBRARY}
        ${KDEVPLATFORM_INTERFACES_LIBRARY}
        ${KDEVPLATFORM_OUTPUTVIEW_LIBRARY}
        ${KDEVPLATFORM_UTIL_LIBRARY}
        ${KDEVPLATFORM_PROJECT_LIBRARY}
)

install(TARGETS kdevcmakebuilder DESTINATION ${PLUGIN_INSTALL_DIR} )

########### install files ###############
install( FILES kdevcmakebuilder.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install( FILES icmakebuilder.h DESTINATION ${INCLUDE_INSTALL_DIR}/kdevelop/cmake COMPONENT Devel )
install( FILES data.kdev4 DESTINATION ${DATA_INSTALL_DIR}/kdevcmakebuilder )
