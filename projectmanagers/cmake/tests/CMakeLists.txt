set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
include_directories(
    ${CMAKE_SOURCE_DIR}/projectmanagers/cmake
    ${CMAKE_SOURCE_DIR}/projectmanagers/cmake/parser
    )

set( cmakeparsertest_SRCS cmakeparsertest.cpp )

kde4_add_unit_test(cmake-parsertest  ${cmakeparsertest_SRCS} )
target_link_libraries( cmake-parsertest
    ${QT_QTTEST_LIBRARY}
	${QT_QTCORE_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    kdev4cmakecommon
)


set( cmakecompliance_SRCS cmakecompliance.cpp )

kde4_add_unit_test(cmakecompliance  ${cmakecompliance_SRCS} )
target_link_libraries( cmakecompliance
    ${QT_QTTEST_LIBRARY}
	${QT_QTCORE_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    kdev4cmakecommon
)

set( astfactorytest_SRCS astfactorytest.cpp )

kde4_add_unit_test(cmake-astfactorytest  ${astfactorytest_SRCS} )
target_link_libraries( cmake-astfactorytest
    ${QT_QTTEST_LIBRARY}
	${QT_QTCORE_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    kdev4cmakecommon
)

set( cmakeasttest_SRCS cmakeast_test.cpp )

kde4_add_unit_test(cmakeast-test 
                     ${cmakeasttest_SRCS} )
target_link_libraries( cmakeast-test
    ${QT_QTTEST_LIBRARY}
	${QT_QTCORE_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    kdev4cmakecommon
)

set ( cmakecondition_SRCS cmake_cmakecondition_test.cpp )
kde4_add_unit_test(cmake-cmakecondition  ${cmakecondition_SRCS} )
target_link_libraries( cmake-cmakecondition
    ${QT_QTTEST_LIBRARY}
	${KDEVPLATFORM_LANGUAGE_LIBRARY}
	${QT_QTCORE_LIBRARY}
    ${KDE_KDECORE_LIBS}
    kdev4cmakecommon )
  
set ( cmakeduchaintest_SRCS cmakeduchaintest.cpp )
kde4_add_unit_test( cmakeduchaintest  ${cmakeduchaintest_SRCS} )
target_link_libraries( cmakeduchaintest 
        ${QT_QTTEST_LIBRARY}
        ${KDE_KDECORE_LIBS}
		${QT_QTCORE_LIBRARY}
		${KDEVPLATFORM_LANGUAGE_LIBRARY}
        ${KDE4_KTEXTEDITOR_LIBS}
        kdev4cmakecommon )

set ( cmakeprojectvisitortest_SRCS cmake_cmakeprojectvisitor_test.cpp )
kde4_add_unit_test( cmakeprojectvisitortest  ${cmakeprojectvisitortest_SRCS} )
target_link_libraries( cmakeprojectvisitortest 
        ${QT_QTTEST_LIBRARY}
		${QT_QTCORE_LIBRARY}
        ${KDE_KDECORE_LIBS}
		${KDEVPLATFORM_LANGUAGE_LIBRARY}
        ${KDE4_KTEXTEDITOR_LIBS}
        kdev4cmakecommon )

set ( cmakeparserutilstest_SRCS cmakeparserutilstest.cpp )
kde4_add_unit_test( cmakeparserutilstest ${cmakeparserutilstest_SRCS} )
target_link_libraries( cmakeparserutilstest 
        ${QT_QTTEST_LIBRARY}
        ${KDE_KDECORE_LIBS}
		${QT_QTCORE_LIBRARY}
        ${KDE4_KTEXTEDITOR_LIBS}
        kdev4cmakecommon )
