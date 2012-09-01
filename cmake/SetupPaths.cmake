#################################################
#
#  (C) 2010-2012 Serghei Amelian
#  serghei (DOT) amelian (AT) gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

#################################################
#####
##### kde_internal_setup_path

macro( _kde_internal_setup_path _path _default _comment )
  if( DEFINED ${_path} )
    set( ${_path} "${${_path}}" CACHE PATH "${_comment}" )
  else( DEFINED ${_path} )
    set( ${_path} "${_default}" )
  endif( DEFINED ${_path} )
endmacro( _kde_internal_setup_path )


# install paths
_kde_internal_setup_path( EXEC_INSTALL_PREFIX       "${CMAKE_INSTALL_PREFIX}"                     "Base directory for executables and libraries" )
_kde_internal_setup_path( SHARE_INSTALL_PREFIX      "${CMAKE_INSTALL_PREFIX}/share"               "Base directory for files which go to share/" )
_kde_internal_setup_path( BIN_INSTALL_DIR           "${EXEC_INSTALL_PREFIX}/bin"                  "The install dir for executables (default ${EXEC_INSTALL_PREFIX}/bin)" )
_kde_internal_setup_path( SBIN_INSTALL_DIR          "${EXEC_INSTALL_PREFIX}/sbin"                 "The install dir for system executables (default ${EXEC_INSTALL_PREFIX}/sbin)" )
_kde_internal_setup_path( LIB_INSTALL_DIR           "${EXEC_INSTALL_PREFIX}/lib${LIB_SUFFIX}"     "The subdirectory relative to the install prefix where libraries will be installed (default is ${EXEC_INSTALL_PREFIX}/lib${LIB_SUFFIX})" )
_kde_internal_setup_path( LIBEXEC_INSTALL_DIR       "${LIB_INSTALL_DIR}/kde3/libexec"             "The subdirectory relative to the install prefix where libraries will be installed (default is ${LIB_INSTALL_DIR}/kde3/libexec)" )
_kde_internal_setup_path( INCLUDE_INSTALL_DIR       "${CMAKE_INSTALL_PREFIX}/include"             "The subdirectory to the header prefix" )

_kde_internal_setup_path( CMAKE_INSTALL_DIR         "${SHARE_INSTALL_PREFIX}/cmake"               "The install dir for cmake import modules" )
_kde_internal_setup_path( PLUGIN_INSTALL_DIR        "${LIB_INSTALL_DIR}/kde3"                     "The subdirectory relative to the install prefix where plugins will be installed (default is ${LIB_INSTALL_DIR}/kde3)" )
_kde_internal_setup_path( CONFIG_INSTALL_DIR        "${SHARE_INSTALL_PREFIX}/config"              "The config file install dir" )
_kde_internal_setup_path( DATA_INSTALL_DIR          "${SHARE_INSTALL_PREFIX}/apps"                "The parent directory where applications can install their data" )
_kde_internal_setup_path( HTML_INSTALL_DIR          "${SHARE_INSTALL_PREFIX}/doc/HTML"            "The HTML install dir for documentation" )
_kde_internal_setup_path( ICON_INSTALL_DIR          "${SHARE_INSTALL_PREFIX}/icons"               "The icon install dir (default ${SHARE_INSTALL_PREFIX}/share/icons/)" )
_kde_internal_setup_path( KCFG_INSTALL_DIR          "${SHARE_INSTALL_PREFIX}/config.kcfg"         "The install dir for kconfig files" )
_kde_internal_setup_path( LOCALE_INSTALL_DIR        "${SHARE_INSTALL_PREFIX}/locale"              "The install dir for translations" )
_kde_internal_setup_path( APPS_INSTALL_DIR          "${SHARE_INSTALL_PREFIX}/applnk"              "The install dir for the application desktop files" )
_kde_internal_setup_path( MIME_INSTALL_DIR          "${SHARE_INSTALL_PREFIX}/mimelnk"             "The install dir for the mimetype desktop files" )
_kde_internal_setup_path( SERVICES_INSTALL_DIR      "${SHARE_INSTALL_PREFIX}/services"            "The install dir for service (desktop, protocol, ...) files" )
_kde_internal_setup_path( SERVICETYPES_INSTALL_DIR  "${SHARE_INSTALL_PREFIX}/servicetypes"        "The install dir for servicestypes desktop files" )
_kde_internal_setup_path( SOUND_INSTALL_DIR         "${SHARE_INSTALL_PREFIX}/sounds"              "The install dir for sound files" )
_kde_internal_setup_path( TEMPLATES_INSTALL_DIR     "${SHARE_INSTALL_PREFIX}/templates"           "The install dir for templates (Create new file...)" )
_kde_internal_setup_path( WALLPAPER_INSTALL_DIR     "${SHARE_INSTALL_PREFIX}/wallpapers"          "The install dir for wallpapers" )
_kde_internal_setup_path( KCONF_UPDATE_INSTALL_DIR  "${DATA_INSTALL_DIR}/kconf_update"            "The kconf_update install dir" )
_kde_internal_setup_path( AUTOSTART_INSTALL_DIR     "${SHARE_INSTALL_PREFIX}/autostart"           "The install dir for autostart files" )

_kde_internal_setup_path( SYSCONF_INSTALL_DIR       "${CMAKE_INSTALL_PREFIX}/etc"                 "The sysconfig install dir (default ${CMAKE_INSTALL_PREFIX}/etc)" )
_kde_internal_setup_path( MAN_INSTALL_DIR           "${SHARE_INSTALL_PREFIX}/man"                 "The man install dir (default ${SHARE_INSTALL_PREFIX}/man/)" )
_kde_internal_setup_path( INFO_INSTALL_DIR          "${SHARE_INSTALL_PREFIX}/info"                "The info install dir (default ${SHARE_INSTALL_PREFIX}/info)" )

_kde_internal_setup_path( XDG_MENU_INSTALL_DIR      "${SYSCONF_INSTALL_DIR}/xdg/menus"            "The XDG menus dir" )
_kde_internal_setup_path( XDG_APPS_INSTALL_DIR      "${SHARE_INSTALL_PREFIX}/applications/kde"    "The XDG apps dir" )
_kde_internal_setup_path( XDG_DIRECTORY_INSTALL_DIR "${SHARE_INSTALL_PREFIX}/desktop-directories" "The XDG directory" )
_kde_internal_setup_path( XDG_MIME_INSTALL_DIR      "${SHARE_INSTALL_PREFIX}/mime/packages"       "The install dir for the xdg mimetypes" )
