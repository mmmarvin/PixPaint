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

INCLUDE_PATH_LEPTONICA = # Add include location for leptonica here
LIB_PATH_LEPTONICA = # Add lib location for leptonica here
LIB_NAME_LEPTONICA = libleptonica.a

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
    src/dialog/effectoptiondialog.h \
    src/dialog/optiondialog.h \
    src/effect/brightnesscontrasteffect.h \
    src/effect/effectbase.h \
    src/effect/huesaturationlightness.h \
    src/effect_importer.h \
    src/registrar/information/effectinformation.h \
    src/registrar/effectregistrar.h \
    src/embed/headerstream.h \
    src/filetype/stbiimageholder.h \
    src/helper/image_helpers.h \
    src/image/rect.inl \
    src/keys.h \
    src/registrar/projectfiletyperegistrar.h \
    src/registrar/registrarbase.h \
    src/registrar/registrarbase.inl \
    src/manager/textmanager.h \
    src/manager/textselectionmanager.h \
    src/tool/ditherfilltool.h \
    src/tool/textselectiontool.h \
    src/value/optionvalue.h \
    src/value/optionvaluelist.h \
    src/window/horizontalresizehandle.h \
    src/window/hvresizehandle.h \
    src/window/mainwindow.h \
    src/image/color.h \
    src/type.h \
    src/utility/general_utility.h \
    src/image/pixelmap.h \
    src/image/image.h \
    src/assert.h \
    src/window/imageview.h \
    src/config.h \
    src/window/imageeditorview.h \
    src/window/documentpanel.h \
    src/image/point.h \
    src/3rdparty/high_resolution_clock.h \
    src/3rdparty/time.h \
    src/singleton.h \
    src/manager/colormanager.h \
    src/default_color_template.h \
    src/manager/painttoolmanager.h \
    src/registrar/information/painttoolinformation.h \
    src/tool_importer.h \
    src/registrar/painttoolregistrar.h \
    src/tool/painttoolbase.h \
    src/tool/penciltool.h \
    src/image/rect.h \
    src/tool/filltool.h \
    src/tool/linetool.h \
    src/window/colorbutton.h \
    src/image/modifyablepixeldata.h \
    src/window/optioncheckbox.h \
    src/window/optionchoice.h \
    src/window/optionslider.h \
    src/window/optiontextbox.h \
    src/window/painttooloptionframe.h \
    src/tool/erasertool.h \
    src/window/previewview.h \
    src/window/righttoolbox.h \
    src/window/previewwidget.h \
    src/window/lefttoolbox.h \
    src/window/layerwidget.h \
    src/manager/imagemanager.h \
    src/layout/flowlayout.h \
    src/window/verticalresizehandle.h \
    src/window/zoomablescrollarea.h \
    src/dialog/newimagedialog.h \
    src/window/mainmenu.h \
    src/utility/filetype_utility.h \
    src/tool/circletool.h \
    src/tool/recttool.h \
    src/manager/selectionmanager.h \
    src/tool/rectselectiontool.h \
    src/debug_log.h \
    src/window/selectionwidget.h \
    src/3rdparty/gengine/configuration.h \
    src/3rdparty/gengine/variablemanager.h \
    src/3rdparty/gengine/type_determine_utility.h \
    src/3rdparty/gengine/string_utility.h \
    src/3rdparty/gengine/base.h \
    src/3rdparty/gengine/converter_utility.h \
    src/define.h \
    src/helper/selection_helpers.h \
    src/dialog/rotateimagedialog.h \
    src/utility/float_utility.h \
    src/env/guienvironment.h \
    src/window/statusbar.h \
    src/dialog/aboutdialog.h \
    src/tool/colorpickertool.h \
    src/extension/optional.h \
    src/cursor.h \
    src/tool/cursortoolbase.h \
    src/helper/paint_helpers.h \
    src/env/imageenvironment.h \
    src/window/customcursorwidget.h \
    src/linestyle.h \
    src/tool/magicwandtool.h \
    src/image/maskablepixeldata.h \
    src/image/pixeldata.h \
    src/window/frametoolbox.h \
    src/window/frametoolboxitem.h \
    src/3rdparty/gif-h/gif.h \
    src/3rdparty/stbi/stb_image.h \
    src/3rdparty/stbi/stb_image_write.h \
    src/image/animation.h \
    src/registrar/information/fileinformation.h \
    src/manager/previewmanager.h \
    src/animationfiletype/animationfiletypebase.h \
    src/animation_filetype_importer.h \
    src/registrar/animationfiletyperegistrar.h \
    src/animationfiletype/gifanimationfiletype.h \
    src/filetype/imagefiletypebase.h \
    src/image_filetype_importer.h \
    src/registrar/imagefiletyperegistrar.h \
    src/filetype/bmpimagefiletype.h \
    src/filetype/jpgimagefiletype.h \
    src/filetype/pngimagefiletype.h \
    src/animationfiletype/pixpaintprojectfiletype.h \
    src/manager/eventmanager.h \
    src/3rdparty/ska/bytell_hash_map.hpp \
    src/3rdparty/ska/flat_hash_map.hpp \
    src/3rdparty/ska/unordered_map.hpp \
    src/event/gui/tab_events.h \
    src/idgenerator.h \
    src/window/layerwidgetitem.h \
    src/event/gui/frame_events.h \
    src/event/gui/animation_events.h \
    src/event/gui/image_events.h \
    src/window/layerpreviewview.h \
    src/window/framepreviewview.h \
    src/window/nonselectionpreviewview.h \
    src/os_specific_headers.h \
    src/window/colortoolbox.h \
    src/helper/tool_helpers.h \
    src/image/size.h \
    src/tool/brushtoolbase.h \
    src/tool/brushtool.h \
    src/utility/math_utility.h \
    src/drawer/drawer.h \
    src/drawer/qt/qtdrawer.h \
    src/manager/drawermanager.h \
    src/drawer/drawerholder.h \
    src/image/mask.h \
    src/drawer/qt/qtmask.h \
    src/utility/qt_utility.h \
    src/drawer/drawer_config.h \
    src/manager/historymanager.h \
    src/history/historyaction.h \
    src/history/draweraction.h \
    src/history/selectionaction.h \
    src/history/deselectionaction.h \
    src/history/history.h \
    src/document.h \
    src/document/animationdocument.h \
    src/manager/documentmanager.h \
    src/project_filetype_importer.h \
    src/window/globalkeyreader.h \
    src/animationfiletype/spritesheetanimationfiletype.h \
    src/event/gui/history_events.h \
    src/history/selectionmoveaction.h \
    src/history/addlayeraction.h \
    src/history/removelayeraction.h \
    src/history/renamelayeraction.h \
    src/history/movelayeraction.h \
    src/history/addframeaction.h \
    src/history/removeframeaction.h \
    src/embed/script_exports.h \
    src/embed/painttoolwrapper.h \
    src/embed/script_utility.h \
    src/embed/script_errors.h \
    src/embed/imagefiletypewrapper.h \
    src/embed/animationfiletypewrapper.h \
    src/embed/effectwrapper.h \
    src/gui_define.h \
    src/history/selectioncutaction.h \
    src/history/selectionpasteaction.h \
    src/history/regionrestoreaction.h \
    src/history/selectionregionrestoreaction.h \
    src/history/imagerestoreaction.h \
    src/history/animationresizeaction.h \
    src/history/textselectionaction.h \
    src/history/textdeselectionaction.h \
    src/window/consoletoolbox.h \
    src/utility/settings_file_utility.h \
    src/pixpaint_macro.h \
    src/macro.h

SOURCES += \
    src/dialog/effectoptiondialog.cpp \
    src/dialog/optiondialog.cpp \
    src/effect/brightnesscontrasteffect.cpp \
    src/effect/effectbase.cpp \
    src/effect/huesaturationlightness.cpp \
    src/effect_importer.cpp \
    src/registrar/information/effectinformation.cpp \
    src/registrar/effectregistrar.cpp \
    src/embed/headerstream.cpp \
    src/filetype/stbiimageholder.cpp \
    src/helper/image_helpers.cpp \
    src/manager/textmanager.cpp \
    src/manager/textselectionmanager.cpp \
    src/registrar/projectfiletyperegistrar.cpp \
    src/tool/ditherfilltool.cpp \
    src/tool/textselectiontool.cpp \
    src/value/optionvalue.cpp \
    src/value/optionvaluelist.cpp \
    src/window/horizontalresizehandle.cpp \
    src/window/hvresizehandle.cpp \
    src/window/mainwindow.cpp \
    src/image/color.cpp \
    src/utility/general_utility.inl \
    src/image/pixelmap.cpp \
    src/image/image.cpp \
    src/window/imageview.cpp \
    src/window/imageeditorview.cpp \
    src/window/documentpanel.cpp \
    src/image/point.cpp \
    src/3rdparty/high_resolution_clock.cpp \
    src/3rdparty/time.cpp \
    src/3rdparty/time.inl \
    src/singleton.inl \
    src/manager/colormanager.cpp \
    src/default_color_template.cpp \
    src/manager/painttoolmanager.cpp \
    src/registrar/information/painttoolinformation.cpp \
    src/tool_importer.cpp \
    src/registrar/painttoolregistrar.cpp \
    src/tool/penciltool.cpp \
    src/tool/filltool.cpp \
    src/tool/linetool.cpp \
    src/pixpaint.cpp \
    src/window/colorbutton.cpp \
    src/tool/painttoolbase.cpp \
    src/image/modifyablepixeldata.cpp \
    src/window/optioncheckbox.cpp \
    src/window/optionchoice.cpp \
    src/window/optionslider.cpp \
    src/window/optiontextbox.cpp \
    src/window/painttooloptionframe.cpp \
    src/tool/erasertool.cpp \
    src/window/previewview.cpp \
    src/window/righttoolbox.cpp \
    src/window/previewwidget.cpp \
    src/window/lefttoolbox.cpp \
    src/window/layerwidget.cpp \
    src/manager/imagemanager.cpp \
    src/layout/flowlayout.cpp \
    src/window/verticalresizehandle.cpp \
    src/window/zoomablescrollarea.cpp \
    src/dialog/newimagedialog.cpp \
    src/window/mainmenu.cpp \
    src/utility/filetype_utility.cpp \
    src/tool/circletool.cpp \
    src/tool/recttool.cpp \
    src/manager/selectionmanager.cpp \
    src/tool/rectselectiontool.cpp \
    src/debug_log.inl \
    src/debug_log.cpp \
    src/window/selectionwidget.cpp \
    src/3rdparty/gengine/configuration.cpp \
    src/3rdparty/gengine/variablemanager.cpp \
    src/3rdparty/gengine/type_determine_utility.cpp \
    src/3rdparty/gengine/string_utility.cpp \
    src/3rdparty/gengine/string_utility.inl \
    src/3rdparty/gengine/converter_utility.inl \
    src/helper/selection_helpers.cpp \
    src/dialog/rotateimagedialog.cpp \
    src/utility/float_utility.cpp \
    src/env/guienvironment.cpp \
    src/window/statusbar.cpp \
    src/dialog/aboutdialog.cpp \
    src/tool/colorpickertool.cpp \
    src/cursor.cpp \
    src/tool/cursortoolbase.cpp \
    src/helper/paint_helpers.inl \
    src/helper/paint_helpers.cpp \
    src/env/imageenvironment.cpp \
    src/window/customcursorwidget.inl \
    src/tool/magicwandtool.cpp \
    src/image/maskablepixeldata.cpp \
    src/image/pixeldata.cpp \
    src/window/frametoolbox.cpp \
    src/window/frametoolboxitem.cpp \
    src/image/animation.cpp \
    src/registrar/information/fileinformation.inl \
    src/animationfiletype/animationfiletypebase.cpp \
    src/animation_filetype_importer.cpp \
    src/registrar/animationfiletyperegistrar.cpp \
    src/animationfiletype/gifanimationfiletype.cpp \
    src/filetype/imagefiletypebase.cpp \
    src/image_filetype_importer.cpp \
    src/registrar/imagefiletyperegistrar.cpp \
    src/filetype/bmpimagefiletype.cpp \
    src/filetype/jpgimagefiletype.cpp \
    src/filetype/pngimagefiletype.cpp \
    src/animationfiletype/pixpaintprojectfiletype.cpp \
    src/manager/eventmanager.inl \
    src/manager/eventmanager.cpp \
    src/manager/previewmanager.cpp \
    src/idgenerator.cpp \
    src/window/layerwidgetitem.cpp \
    src/window/layerpreviewview.cpp \
    src/window/framepreviewview.cpp \
    src/window/nonselectionpreviewview.cpp \
    src/window/colortoolbox.cpp \
    src/helper/tool_helpers.cpp \
    src/image/size.cpp \
    src/tool/brushtoolbase.cpp \
    src/tool/brushtool.cpp \
    src/utility/math_utility.cpp \
    src/drawer/qt/qtdrawer.cpp \
    src/manager/drawermanager.cpp \
    src/drawer/drawerholder.cpp \
    src/drawer/qt/qtmask.cpp \
    src/utility/qt_utility.cpp \
    src/drawer/drawer_config.cpp \
    src/drawer/drawerholder.inl \
    src/drawer/drawer.cpp \
    src/manager/historymanager.cpp \
    src/history/draweraction.cpp \
    src/history/historyaction.cpp \
    src/history/selectionaction.cpp \
    src/history/deselectionaction.cpp \
    src/history/history.cpp \
    src/history/history.inl \
    src/document.cpp \
    src/document/animationdocument.cpp \
    src/manager/documentmanager.cpp \
    src/window/mainwindow.inl \
    src/project_filetype_importer.cpp \
    src/window/globalkeyreader.cpp \
    src/window/mainwindow_shortcuts.cpp \
    src/animationfiletype/spritesheetanimationfiletype.cpp \
    src/utility/filetype_utility.inl \
    src/window/customcursorwidget.cpp \
    src/history/selectionmoveaction.cpp \
    src/manager/historymanager.inl \
    src/history/addlayeraction.cpp \
    src/history/removelayeraction.cpp \
    src/history/renamelayeraction.cpp \
    src/history/movelayeraction.cpp \
    src/history/addframeaction.cpp \
    src/history/removeframeaction.cpp \
    src/embed/script_exports.cpp \
    src/os_specific_headers.cpp \
    src/embed/painttoolwrapper.cpp \
    src/embed/script_utility.cpp \
    src/embed/script_errors.cpp \
    src/embed/imagefiletypewrapper.cpp \
    src/embed/animationfiletypewrapper.cpp \
    src/embed/effectwrapper.cpp \
    src/history/selectioncutaction.cpp \
    src/history/selectionpasteaction.cpp \
    src/history/regionrestoreaction.cpp \
    src/history/selectionregionrestoreaction.cpp \
    src/history/imagerestoreaction.cpp \
    src/history/animationresizeaction.cpp \
    src/history/textselectionaction.cpp \
    src/history/textdeselectionaction.cpp \
    src/window/consoletoolbox.cpp \
    src/utility/settings_file_utility.cpp

INCLUDEPATH += \
    $${INCLUDE_PATH_PYTHON} \
    $${INCLUDE_PATH_CLIP} \
    $${INCLUDE_PATH_LEPTONICA}

LIBS += \
    -lpython3.9 -lboost_python39 -lpng -lxcb -lz \
    $${LIB_PATH_CLIP}/$${LIB_NAME_CLIP} \
    $${LIB_PATH_LEPTONICA}/$${LIB_NAME_LEPTONICA}

