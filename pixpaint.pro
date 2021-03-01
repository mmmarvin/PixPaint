TARGET = PixPaint
TEMPLATE = app
#CONFIG += console
CONFIG -= app_bundle
CONFIG += no_keywords
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += BOOST_BIND_GLOBAL_PLACEHOLDERS         # Fix for boost python bind

# Include / Library Locations
INCLUDE_PATH_CLIP = # Add include location for clip here
LIB_PATH_CLIP = # Add library location for clip here
LIB_NAME_CLIP = libclip.a

INCLUDE_PATH_PYTHON = # Add include lcoation for python here

# CONFIGS
#CONFIG += CXX_14                    # Use c++14, else use c++17
#CONFIG += USE_MULTI_THREADING       # Use of multi-threading (Experimental)

CONFIG(debug, debug|release) {
}
CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

CXX_14 {
  CONFIG += c++14
  LIBS += -lboost_filesystem
} else {
  CONFIG += c++17
}

USE_MULTI_THREADING {
  DEFINES += PIXPAINT_USE_MULTITHREAD

  HEADERS += \
    src/utility/thread_utility.h

  SOURCES += \
    src/utility/thread_utility.inl \

  LIBS += \
    -pthread
}

HEADERS += \
    src/3rdparty/gengine/base.h \
    src/3rdparty/gengine/configuration.h \
    src/3rdparty/gengine/converter_utility.h \
    src/3rdparty/gengine/string_utility.h \
    src/3rdparty/gengine/type_determine_utility.h \
    src/3rdparty/gengine/variablemanager.h \
    src/3rdparty/gif-h/gif.h \
    src/3rdparty/ska/bytell_hash_map.hpp \
    src/3rdparty/ska/flat_hash_map.hpp \
    src/3rdparty/ska/unordered_map.hpp \
    src/3rdparty/stbi/stb_image.h \
    src/3rdparty/stbi/stb_image_resize.h \
    src/3rdparty/stbi/stb_image_write.h \
    src/3rdparty/high_resolution_clock.h \
    src/3rdparty/time.h \
    src/animationfiletype/animationfiletypebase.h \
    src/animationfiletype/gifanimationfiletype.h \
    src/animationfiletype/pixpaintprojectfiletype.h \
    src/animationfiletype/spritesheetanimationfiletype.h \
    src/dialog/aboutdialog.h \
    src/dialog/boxgriddialog.h \
    src/dialog/effectoptiondialog.h \
    src/dialog/newimagedialog.h \
    src/dialog/optiondialog.h \
    src/dialog/rotateimagedialog.h \
    src/document/animationdocument.h \
    src/drawer/qt/qtdrawer.h \
    src/drawer/qt/qtmask.h \
    src/drawer/drawer.h \
    src/drawer/drawer_config.h \
    src/drawer/drawerholder.h \
    src/effect/brightnesscontrasteffect.h \
    src/effect/effectbase.h \
    src/effect/huesaturationlightness.h \
    src/embed/animationfiletypewrapper.h \
    src/embed/effectwrapper.h \
    src/embed/headerstream.h \
    src/embed/imagefiletypewrapper.h \
    src/embed/painttoolwrapper.h \
    src/embed/script_errors.h \
    src/embed/script_exports.h \
    src/embed/script_utility.h \
    src/env/guienvironment.h \
    src/env/imageenvironment.h \
    src/event/gui/animation_events.h \
    src/event/gui/frame_events.h \
    src/event/gui/history_events.h \
    src/event/gui/image_events.h \
    src/event/gui/tab_events.h \
    src/extension/optional.h \
    src/filetype/bmpimagefiletype.h \
    src/filetype/icoimagefiletype.h \
    src/filetype/imagefiletypebase.h \
    src/filetype/jpgimagefiletype.h \
    src/filetype/pngimagefiletype.h \
    src/filetype/stbiimageholder.h \
    src/helper/image_helpers.h \
    src/helper/paint_helpers.h \
    src/helper/selection_helpers.h \
    src/helper/tool_helpers.h \
    src/history/addframeaction.h \
    src/history/addlayeraction.h \
    src/history/animationresizeaction.h \
    src/history/deselectionaction.h \
    src/history/draweraction.h \
    src/history/hidelayeraction.h \
    src/history/history.h \
    src/history/historyaction.h \
    src/history/imagerestoreaction.h \
    src/history/movelayeraction.h \
    src/history/regionrestoreaction.h \
    src/history/removeframeaction.h \
    src/history/removelayeraction.h \
    src/history/renamelayeraction.h \
    src/history/selectionaction.h \
    src/history/selectioncutaction.h \
    src/history/selectionmoveaction.h \
    src/history/selectionpasteaction.h \
    src/history/selectionregionrestoreaction.h \
    src/history/selectionresize.h \
    src/history/showlayeraction.h \
    src/history/textdeselectionaction.h \
    src/history/textselectionaction.h \
    src/image/animation.h \
    src/image/color.h \
    src/image/image.h \
    src/image/mask.h \
    src/image/maskablepixeldata.h \
    src/image/modifyablepixeldata.h \
    src/image/pixeldata.h \
    src/image/pixelmap.h \
    src/image/point.h \
    src/image/rect.h \
    src/image/size.h \
    src/layout/flowlayout.h \
    src/manager/colormanager.h \
    src/manager/consolemanager.h \
    src/manager/documentmanager.h \
    src/manager/drawermanager.h \
    src/manager/dummyselectionmanager.h \
    src/manager/eventmanager.h \
    src/manager/historymanager.h \
    src/manager/imagemanager.h \
    src/manager/painttoolmanager.h \
    src/manager/previewmanager.h \
    src/manager/selectionmanager.h \
    src/manager/textmanager.h \
    src/manager/textselectionmanager.h \
    src/pattern/observer.h \
    src/pattern/singleton.h \
    src/registrar/information/effectinformation.h \
    src/registrar/information/fileinformation.h \
    src/registrar/information/painttoolinformation.h \
    src/registrar/animationfiletyperegistrar.h \
    src/registrar/effectregistrar.h \
    src/registrar/imagefiletyperegistrar.h \
    src/registrar/painttoolregistrar.h \
    src/registrar/projectfiletyperegistrar.h \
    src/registrar/registrarbase.h \
    src/tool/brushtool.h \
    src/tool/brushtoolbase.h \
    src/tool/circletool.h \
    src/tool/colorpickertool.h \
    src/tool/cursortoolbase.h \
    src/tool/custombrushtool.h \
    src/tool/ditherfilltool.h \
    src/tool/erasertool.h \
    src/tool/filltool.h \
    src/tool/linetool.h \
    src/tool/magicwandtool.h \
    src/tool/painttoolbase.h \
    src/tool/penciltool.h \
    src/tool/rectselectiontool.h \
    src/tool/recttool.h \
    src/tool/textselectiontool.h \
    src/utility/filetype_utility.h \
    src/utility/float_utility.h \
    src/utility/general_utility.h \
    src/utility/geometry_utility.h \
    src/utility/math_utility.h \
    src/utility/qt_utility.h \
    src/utility/settings_file_utility.h \
    src/utility/thread_utility.h \
    src/value/optionvalue.h \
    src/value/optionvaluelist.h \
    src/window/baseimageview.h \
    src/window/colorbutton.h \
    src/window/colortoolbox.h \
    src/window/consoletoolbox.h \
    src/window/customcursorwidget.h \
    src/window/documentpanel.h \
    src/window/dummyselectionwidget.h \
    src/window/framepreviewview.h \
    src/window/frametoolbox.h \
    src/window/frametoolboxitem.h \
    src/window/globalkeyreader.h \
    src/window/horizontalresizehandle.h \
    src/window/hvresizehandle.h \
    src/window/imageeditorview.h \
    src/window/imageview.h \
    src/window/layerpreviewview.h \
    src/window/layerwidget.h \
    src/window/layerwidgetitem.h \
    src/window/lefttoolbox.h \
    src/window/mainmenu.h \
    src/window/mainwindow.h \
    src/window/nonselectionpreviewview.h \
    src/window/observerlabel.h \
    src/window/optioncheckbox.h \
    src/window/optionchoice.h \
    src/window/optionslider.h \
    src/window/optiontextbox.h \
    src/window/painttooloptionframe.h \
    src/window/previewview.h \
    src/window/previewwidget.h \
    src/window/righttoolbox.h \
    src/window/selectionwidget.h \
    src/window/statusbar.h \
    src/window/verticalresizehandle.h \
    src/window/zoomablescrollarea.h \
    src/animation_filetype_importer.h \
    src/assert.h \
    src/colorlist.h \
    src/config.h \
    src/cursor.h \
    src/debug_log.h \
    src/default_color_template.h \
    src/define.h \
    src/document.h \
    src/effect_importer.h \
    src/gui_define.h \
    src/idgenerator.h \
    src/image_filetype_importer.h \
    src/keys.h \
    src/linestyle.h \
    src/macro.h \
    src/os_specific_headers.h \
    src/project_filetype_importer.h \
    src/singleton.h \
    src/tool_importer.h \
    src/type.h

SOURCES += \
    src/3rdparty/gengine/configuration.cpp \
    src/3rdparty/gengine/converter_utility.inl \
    src/3rdparty/gengine/string_utility.cpp \
    src/3rdparty/gengine/string_utility.inl \
    src/3rdparty/gengine/type_determine_utility.cpp \
    src/3rdparty/gengine/variablemanager.cpp \
    src/3rdparty/high_resolution_clock.cpp \
    src/3rdparty/time.cpp \
    src/3rdparty/time.inl \
    src/animationfiletype/animationfiletypebase.cpp \
    src/animationfiletype/gifanimationfiletype.cpp \
    src/animationfiletype/pixpaintprojectfiletype.cpp \
    src/animationfiletype/spritesheetanimationfiletype.cpp \
    src/dialog/aboutdialog.cpp \
    src/dialog/boxgriddialog.cpp \
    src/dialog/effectoptiondialog.cpp \
    src/dialog/newimagedialog.cpp \
    src/dialog/optiondialog.cpp \
    src/dialog/rotateimagedialog.cpp \
    src/document/animationdocument.cpp \
    src/drawer/qt/qtdrawer.cpp \
    src/drawer/qt/qtmask.cpp \
    src/drawer/drawer.cpp \
    src/drawer/drawer_config.cpp \
    src/drawer/drawerholder.cpp \
    src/drawer/drawerholder.inl \
    src/effect/brightnesscontrasteffect.cpp \
    src/effect/effectbase.cpp \
    src/effect/huesaturationlightness.cpp \
    src/embed/animationfiletypewrapper.cpp \
    src/embed/effectwrapper.cpp \
    src/embed/headerstream.cpp \
    src/embed/imagefiletypewrapper.cpp \
    src/embed/painttoolwrapper.cpp \
    src/embed/script_errors.cpp \
    src/embed/script_exports.cpp \
    src/embed/script_utility.cpp \
    src/env/guienvironment.cpp \
    src/env/imageenvironment.cpp \
    src/filetype/bmpimagefiletype.cpp \
    src/filetype/icoimagefiletype.cpp \
    src/filetype/imagefiletypebase.cpp \
    src/filetype/jpgimagefiletype.cpp \
    src/filetype/pngimagefiletype.cpp \
    src/filetype/stbiimageholder.cpp \
    src/helper/image_helpers.cpp \
    src/helper/paint_helpers.cpp \
    src/helper/paint_helpers.inl \
    src/helper/selection_helpers.cpp \
    src/helper/tool_helpers.cpp \
    src/history/addframeaction.cpp \
    src/history/addlayeraction.cpp \
    src/history/animationresizeaction.cpp \
    src/history/deselectionaction.cpp \
    src/history/draweraction.cpp \
    src/history/hidelayeraction.cpp \
    src/history/history.cpp \
    src/history/history.inl \
    src/history/historyaction.cpp \
    src/history/imagerestoreaction.cpp \
    src/history/movelayeraction.cpp \
    src/history/regionrestoreaction.cpp \
    src/history/removeframeaction.cpp \
    src/history/removelayeraction.cpp \
    src/history/renamelayeraction.cpp \
    src/history/selectionaction.cpp \
    src/history/selectioncutaction.cpp \
    src/history/selectionmoveaction.cpp \
    src/history/selectionpasteaction.cpp \
    src/history/selectionregionrestoreaction.cpp \
    src/history/selectionresize.cpp \
    src/history/showlayeraction.cpp \
    src/history/textdeselectionaction.cpp \
    src/history/textselectionaction.cpp \
    src/image/animation.cpp \
    src/image/color.cpp \
    src/image/image.cpp \
    src/image/maskablepixeldata.cpp \
    src/image/modifyablepixeldata.cpp \
    src/image/pixeldata.cpp \
    src/image/pixeldata.inl \
    src/image/pixelmap.cpp \
    src/image/point.cpp \
    src/image/rect.inl \
    src/image/size.cpp \
    src/layout/flowlayout.cpp \
    src/manager/colormanager.cpp \
    src/manager/consolemanager.cpp \
    src/manager/documentmanager.cpp \
    src/manager/drawermanager.cpp \
    src/manager/dummyselectionmanager.cpp \
    src/manager/eventmanager.cpp \
    src/manager/eventmanager.inl \
    src/manager/historymanager.cpp \
    src/manager/historymanager.inl \
    src/manager/imagemanager.cpp \
    src/manager/painttoolmanager.cpp \
    src/manager/previewmanager.cpp \
    src/manager/selectionmanager.cpp \
    src/manager/textmanager.cpp \
    src/manager/textselectionmanager.cpp \
    src/pattern/observer.cpp \
    src/registrar/information/effectinformation.cpp \
    src/registrar/information/fileinformation.inl \
    src/registrar/information/painttoolinformation.cpp \
    src/registrar/animationfiletyperegistrar.cpp \
    src/registrar/effectregistrar.cpp \
    src/registrar/imagefiletyperegistrar.cpp \
    src/registrar/painttoolregistrar.cpp \
    src/registrar/projectfiletyperegistrar.cpp \
    src/registrar/registrarbase.inl \
    src/tool/brushtool.cpp \
    src/tool/brushtoolbase.cpp \
    src/tool/circletool.cpp \
    src/tool/colorpickertool.cpp \
    src/tool/cursortoolbase.cpp \
    src/tool/custombrushtool.cpp \
    src/tool/ditherfilltool.cpp \
    src/tool/erasertool.cpp \
    src/tool/filltool.cpp \
    src/tool/linetool.cpp \
    src/tool/magicwandtool.cpp \
    src/tool/painttoolbase.cpp \
    src/tool/penciltool.cpp \
    src/tool/rectselectiontool.cpp \
    src/tool/recttool.cpp \
    src/tool/textselectiontool.cpp \
    src/utility/filetype_utility.cpp \
    src/utility/filetype_utility.inl \
    src/utility/float_utility.cpp \
    src/utility/general_utility.inl \
    src/utility/geometry_utility.cpp \
    src/utility/math_utility.cpp \
    src/utility/qt_utility.cpp \
    src/utility/settings_file_utility.cpp \
    src/utility/thread_utility.inl \
    src/value/optionvalue.cpp \
    src/value/optionvaluelist.cpp \
    src/window/baseimageview.cpp \
    src/window/colorbutton.cpp \
    src/window/colortoolbox.cpp \
    src/window/consoletoolbox.cpp \
    src/window/customcursorwidget.cpp \
    src/window/customcursorwidget.inl \
    src/window/documentpanel.cpp \
    src/window/dummyselectionwidget.cpp \
    src/window/framepreviewview.cpp \
    src/window/frametoolbox.cpp \
    src/window/frametoolboxitem.cpp \
    src/window/globalkeyreader.cpp \
    src/window/horizontalresizehandle.cpp \
    src/window/hvresizehandle.cpp \
    src/window/imageeditorview.cpp \
    src/window/imageview.cpp \
    src/window/layerpreviewview.cpp \
    src/window/layerwidget.cpp \
    src/window/layerwidgetitem.cpp \
    src/window/lefttoolbox.cpp \
    src/window/mainmenu.cpp \
    src/window/mainmenu_edit.cpp \
    src/window/mainmenu_file.cpp \
    src/window/mainmenu_help.cpp \
    src/window/mainmenu_select.cpp \
    src/window/mainmenu_view.cpp \
    src/window/mainwindow.cpp \
    src/window/mainwindow.inl \
    src/window/mainwindow_shortcuts.cpp \
    src/window/nonselectionpreviewview.cpp \
    src/window/observerlabel.cpp \
    src/window/optioncheckbox.cpp \
    src/window/optionchoice.cpp \
    src/window/optionslider.cpp \
    src/window/optiontextbox.cpp \
    src/window/painttooloptionframe.cpp \
    src/window/previewview.cpp \
    src/window/previewwidget.cpp \
    src/window/righttoolbox.cpp \
    src/window/selectionwidget.cpp \
    src/window/statusbar.cpp \
    src/window/verticalresizehandle.cpp \
    src/window/zoomablescrollarea.cpp \
    src/animation_filetype_importer.cpp \
    src/colorlist.cpp \
    src/cursor.cpp \
    src/debug_log.cpp \
    src/debug_log.inl \
    src/default_color_template.cpp \
    src/document.cpp \
    src/effect_importer.cpp \
    src/idgenerator.cpp \
    src/image_filetype_importer.cpp \
    src/os_specific_headers.cpp \
    src/pixpaint.cpp \
    src/project_filetype_importer.cpp \
    src/singleton.inl \
    src/tool_importer.cpp

INCLUDEPATH += \
    $${INCLUDE_PATH_PYTHON} \
    $${INCLUDE_PATH_CLIP}

LIBS += \
    -lpython3.9 -lboost_python39 -lpng -lxcb -lz \
    $${LIB_PATH_CLIP}/$${LIB_NAME_CLIP}

