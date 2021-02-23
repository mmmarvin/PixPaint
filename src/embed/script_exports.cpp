/**********
 *
 *     PixPaint
 *     Copyright 2020-2021 Marvin Manese
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 **********/
#include "script_exports.h"

#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../drawer/drawer.h"
#include "../effect/effectbase.h"
#include "../image/animation.h"
#include "../image/color.h"
#include "../image/image.h"
#include "../image/mask.h"
#include "../image/maskablepixeldata.h"
#include "../image/modifyablepixeldata.h"
#include "../image/pixeldata.h"
#include "../image/point.h"
#include "../image/rect.h"
#include "../manager/colormanager.h"
#include "../manager/documentmanager.h"
#include "../manager/drawermanager.h"
#include "../manager/imagemanager.h"
#include "../registrar/animationfiletyperegistrar.h"
#include "../registrar/imagefiletyperegistrar.h"
#include "../registrar/painttoolregistrar.h"
#include "../tool/painttoolbase.h"
#include "../document.h"
#include "../os_specific_headers.h"
#include "animationfiletypewrapper.h"
#include "effectwrapper.h"
#include "imagefiletypewrapper.h"
#include "painttoolwrapper.h"
#include "headerstream.h"

#define IMPORT_RECT(type, rectName) \
  bp::class_<Rect<type>>(rectName, bp::init<>()) \
    .def(bp::init<type, type, type, type>()) \
    .def_readwrite("x", &Rect<type>::x) \
    .def_readwrite("y", &Rect<type>::y) \
    .def_readwrite("width", &Rect<type>::width) \
    .def_readwrite("height", &Rect<type>::height);

namespace bp = boost::python;
using namespace pixpaint;

BOOST_PYTHON_MODULE(pixpaint)
{
  bp::class_<ControlState>("ControlState")
    .def_readwrite("shiftDown", &ControlState::shiftDown)
    .def_readwrite("controlDown", &ControlState::controlDown);

  bp::class_<Color>("Color", bp::init<>())
    .def(bp::init<color_channel_t, color_channel_t, color_channel_t>())
    .def_readwrite("r", &Color::r)
    .def_readwrite("g", &Color::g)
    .def_readwrite("b", &Color::b)
    .def_readwrite("a", &Color::a);

  bp::class_<Point>("Point", bp::init<>())
    .def(bp::init<position_t, position_t>())
    .def_readwrite("x", &Point::x)
    .def_readwrite("y", &Point::y);

  IMPORT_RECT(int, "IntRect")
  IMPORT_RECT(float, "FloatRect")

  bp::class_<PixelData>("Layer", bp::init<dimension_t, dimension_t, const Color&>())
    .def("clear", static_cast<void(PixelData::*)(const Color&)>(&PixelData::clear))
    .def("clear", static_cast<void(PixelData::*)(position_t, position_t, dimension_t, dimension_t, const Color&)>(&PixelData::clear))
    .def("resize", &PixelData::resize)
    .def("getWidth", &PixelData::getWidth)
    .def("getHeight", &PixelData::getHeight)
    .def("combine", static_cast<void(PixelData::*)(const PixelData&, bool)>(&PixelData::combine))
    .def("combine", static_cast<void(PixelData::*)(const PixelData&, position_t, position_t, bool)>(&PixelData::combine))
    .def("copy", &PixelData::copy)
    .def("cut", &PixelData::cut)
    .def("setPixelSoft", &PixelData::setPixelSoft)
    .def("setPixel", &PixelData::setPixel)
    .def("setPixelRowSoft", static_cast<void(PixelData::*)(position_t, const Color*, size_t)>(&PixelData::setPixelRowSoft))
    .def("setPixelRow", static_cast<void(PixelData::*)(position_t, const Color*, size_t)>(&PixelData::setPixelRow))
    .def("setPixelRowSoft", static_cast<void(PixelData::*)(position_t, position_t, const Color*, size_t)>(&PixelData::setPixelRowSoft))
    .def("setPixelRow", static_cast<void(PixelData::*)(position_t, position_t, const Color*, size_t)>(&PixelData::setPixelRow))
//    .def("getPixelRow", &PixelData::getPixelRow, bp::return_value_policy<bp::return_opaque_pointer>())
    .def("rotate", &PixelData::rotate)
    .def("flipHorizontally", &PixelData::flipHorizontally)
    .def("flipVertically", &PixelData::flipVertically)
    .def("invert", &PixelData::invert)
    .def("toFitted", &PixelData::toFitted);
//    .def("getData", static_cast<color_channel_t*(PixelData::*)()>(&PixelData::getData), bp::return_value_policy<bp::return_opaque_pointer>());

  bp::class_<MaskablePixelData>("MaskableLayer", bp::init<dimension_t, dimension_t, const Color&>())
    .def("combine", static_cast<void(MaskablePixelData::*)(const MaskablePixelData&, bool)>(&MaskablePixelData::combine))
    .def("combine", static_cast<void(MaskablePixelData::*)(const MaskablePixelData&, position_t, position_t, bool)>(&MaskablePixelData::combine))
    .def("setMask", &MaskablePixelData::setMask)
    .def("getMask", &MaskablePixelData::getMask)
    .def("clearMask", &MaskablePixelData::clearMask)
    .def("isMaskSet", &MaskablePixelData::isMaskSet)
    .def("setPixelSoft", &MaskablePixelData::setPixelSoft)
    .def("setPixel", &MaskablePixelData::setPixel);

  bp::class_<ModifyablePixelData>("ModifyableLayer", bp::init<dimension_t, dimension_t, const Color&>())
    .def("clear", &ModifyablePixelData::clear)
    .def("resize", &ModifyablePixelData::resize)
    .def("setPixelSoft", &ModifyablePixelData::setPixelSoft)
    .def("setPixel", &ModifyablePixelData::setPixel);

  bp::class_<Image>("Image", bp::init<position_t, position_t, const Color&>())
    .def("clear", &Image::clear)
    .def("resize", &Image::resize)
    .def("getWidth", &Image::getWidth)
    .def("getHeight", &Image::getHeight)
    .def("addLayer", static_cast<void(Image::*)(const Color&)>(&Image::addLayer))
    .def("addLayer", static_cast<void(Image::*)(size_t, const Color&)>(&Image::addLayer))
    .def("removeLayer", &Image::removeLayer)
    .def("renameLayer", &Image::renameLayer)
    .def("moveLayer", &Image::moveLayer)
    .def("setVisible", &Image::setVisible)
    .def("rotate", &Image::rotate)
    .def("getLayerName", &Image::getLayerName, bp::return_value_policy<bp::return_by_value>())
    .def("getLayer", static_cast<MaskablePixelData&(Image::*)(size_t)>(&Image::getLayer), bp::return_value_policy<bp::reference_existing_object>())
    .def("getCurrentLayerName", &Image::getCurrentLayerName, bp::return_value_policy<bp::return_by_value>())
    .def("getCurrentLayer", static_cast<MaskablePixelData&(Image::*)()>(&Image::getCurrentLayer), bp::return_value_policy<bp::return_by_value>())
    .def("setCurrentLayerIndex", &Image::setCurrentLayerIndex)
    .def("getCurrentLayerIndex", &Image::getCurrentLayerIndex)
    .def("getLayerCount", &Image::getLayerCount)
    .def("isVisible", &Image::isVisible)
    .def("isCurrentLayerVisible", &Image::isCurrentLayerVisible)
    .def("flattenLayers", &Image::flattenLayers)
    .def("setModified", &Image::setModified);

  bp::class_<Animation>("Animation", bp::init<>())
    .def("addFrame", static_cast<void(Animation::*)(size_t, position_t, position_t, int)>(&Animation::addFrame))
    .def("removeFrame", &Animation::removeFrame)
    .def("getFrame", static_cast<Image&(Animation::*)(size_t)>(&Animation::getFrame), bp::return_value_policy<bp::reference_existing_object>())
    .def("getCurrentFrame", static_cast<Image&(Animation::*)()>(&Animation::getCurrentFrame), bp::return_value_policy<bp::reference_existing_object>())
    .def("setFrameDuration", &Animation::setFrameDuration)
    .def("setCurrentFrameDuration", &Animation::setCurrentFrameDuration)
    .def("getFrameDuration", &Animation::getFrameDuration)
    .def("getCurrentFrameDuration", &Animation::getCurrentFrameDuration)
    .def("setCurrentFrameIndex", &Animation::setCurrentFrameIndex)
    .def("getCurrentFrameIndex", &Animation::getCurrentFrameIndex)
    .def("getFrameCount", &Animation::getFrameCount);

  bp::class_<Document, boost::noncopyable>("Document", bp::no_init)
    .def("getAnimation", static_cast<Animation&(Document::*)()>(&Document::getAnimation), bp::return_value_policy<bp::reference_existing_object>())
    .def("setFilename", &Document::setFilename)
    .def("getFilename", &Document::getFilename, bp::return_value_policy<bp::return_by_value>());

  /// Exported Base Type Helper Classes
  bp::class_<HeaderStream, boost::noncopyable>("HeaderStream", bp::no_init)
    .def("read", &HeaderStream::read)
    .def("readCount", &HeaderStream::readCount);

  /// Exported Base Types
  bp::class_<PaintToolWrapper, boost::noncopyable>("PaintTool")
    .def("addFlagOption", &PaintToolWrapper::addFlagOptionWrapper)
    .def("addIntegerOption", &PaintToolWrapper::addIntegerOptionWrapper)
    .def("addStringOption", &PaintToolWrapper::addStringOptionWrapper)
    .def("addStringChoice", &PaintToolWrapper::addStringChoiceWrapper)
    .def("getFlagOption", &PaintToolWrapper::getFlagOptionWrapper)
    .def("getIntegerOption", &PaintToolWrapper::getIntegerOptionWrapper)
    .def("getStringOption", &PaintToolWrapper::getStringOptionWrapper, bp::return_value_policy<bp::reference_existing_object>())
    .def("getStringChoice", &PaintToolWrapper::getStringChoiceWrapper, bp::return_value_policy<bp::reference_existing_object>())
    .def("onKeyPress", &PaintToolBase::onKeyPress)
    .def("onKeyRelease", &PaintToolBase::onKeyRelease)
    .def("onPreMousePress", &PaintToolBase::onPreMousePress)
    .def("onMousePress", &PaintToolBase::onMousePress)
    .def("onMouseMove", &PaintToolBase::onMouseMove)
    .def("onMouseRelease", &PaintToolBase::onMouseRelease)
    .def("onFinalize", &PaintToolBase::onFinalize)
    .def("onResize", &PaintToolBase::onResize)
    .def("onToolChange", &PaintToolBase::onToolChange)
    .def("onColorChange", &PaintToolBase::onColorChange)
    .def("onOptionChange", &PaintToolBase::onOptionChange)
    .def("hasFinalize", &PaintToolBase::hasFinalize)
    .def("hasRightClick", &PaintToolBase::hasRightClick)
    .def("isInstant", &PaintToolBase::isInstant)
    .def("getDrawRect", bp::pure_virtual(&PaintToolBase::getDrawRect))
    .def("getOldDrawRect", &PaintToolBase::getOldDrawRect)
    .def("getHistoryRect", &PaintToolBase::getHistoryRect)
    .def("getDescription", bp::pure_virtual(&PaintToolBase::getDescription))
    .def("getCursor", &PaintToolBase::getCursor);

  bp::class_<ImageFileTypeWrapper, boost::noncopyable>("ImageFileType")
    .def("onPreSave", &ImageFileTypeBase::onPreSave)
    .def("canReadHeader", &ImageFileTypeBase::canReadHeader)
    .def("saveImage", &ImageFileTypeBase::saveImage)
    .def("openImage", &ImageFileTypeBase::openImage);

  bp::class_<AnimationFileTypeWrapper, boost::noncopyable>("AnimationFileType")
    .def("onPreSave", &AnimationFileTypeBase::onPreSave)
    .def("canReadHeader", &AnimationFileTypeBase::canReadHeader)
    .def("saveAnimation", &AnimationFileTypeBase::saveAnimation)
    .def("openAnimation", &AnimationFileTypeBase::openAnimation);

  bp::class_<EffectWrapper, boost::noncopyable>("Effect")
    .def("applyLayerEffect", &EffectBase::applyLayerEffect)
    .def("applySelectionEffect", &EffectBase::applySelectionEffect);

  /// Drawer
  bp::enum_<LineStyle::ELineStyle>("ELineStyle")
    .value("SolidLine", LineStyle::ELineStyle::SolidLine)
    .value("DashedLine", LineStyle::ELineStyle::DashedLine);

  bp::class_<LineStyle>("LineStyle", bp::init<int, int, bool, LineStyle::ELineStyle>())
    .def_readwrite("width", &LineStyle::width)
    .def_readwrite("lineOffset", &LineStyle::lineOffset)
    .def_readwrite("rounded", &LineStyle::rounded)
    .def_readwrite("style", &LineStyle::style);

  bp::class_<DrawParam>("DrawParam", bp::init<bool, LineStyle>())
    .def_readwrite("antialiased", &DrawParam::antialiased)
    .def_readwrite("lineStyle", &DrawParam::lineStyle);

  bp::class_<Drawer, boost::noncopyable>("Drawer", bp::no_init)
    .def("drawLine", &Drawer::drawLine)
    .def("drawLineSoft", &Drawer::drawLineBlend)
    .def("drawCircleSoft", &Drawer::drawCircleBlend)
    .def("drawRectSoft", &Drawer::drawRectBlend)
    .def("drawText", &Drawer::drawText);

  /// Managers
  bp::class_<ImageManager, boost::noncopyable>("ImageManager", bp::no_init)
    .def("refresh", static_cast<void(ImageManager::*)()>(&ImageManager::refresh))
    .def("refresh", static_cast<void(ImageManager::*)(const IntRect&)>(&ImageManager::refresh))
    .def("getImage", static_cast<Image&(ImageManager::*)()>(&ImageManager::getImage), bp::return_value_policy<bp::reference_existing_object>());

  bp::class_<DocumentManager, boost::noncopyable>("DocumentManager", bp::no_init)
    .def("getDocument", static_cast<Document&(DocumentManager::*)()>(&DocumentManager::getDocument), bp::return_value_policy<bp::reference_existing_object>());

  bp::class_<ColorManager, boost::noncopyable>("ColorManager", bp::no_init)
    .def("getForegroundColor", &ColorManager::getForegroundColor, bp::return_value_policy<bp::return_by_value>())
    .def("getBackgroundColor", &ColorManager::getBackgroundColor, bp::return_value_policy<bp::return_by_value>());

  /// Registrars
  bp::class_<PaintToolRegistrar, boost::noncopyable>("PaintToolRegistrar", bp::no_init)
    .def("registerTool", &PaintToolRegistrar::registerCustomTool);
  bp::class_<ImageFileTypeRegistrar, boost::noncopyable>("ImageFileTypeRegistrar", bp::no_init)
    .def("registerFileType", &ImageFileTypeRegistrar::registerCustomFileType);
  bp::class_<AnimationFileTypeRegistrar, boost::noncopyable>("AnimationFileTypeRegistrar", bp::no_init)
    .def("registerFileType", &AnimationFileTypeRegistrar::registerCustomFileType);

  /// Getters
  bp::def("getImageManager", &getImageManager, bp::return_value_policy<bp::reference_existing_object>());
  bp::def("getDocumentManager", &getDocumentManager, bp::return_value_policy<bp::reference_existing_object>());
  bp::def("getColorManager", &getColorManager, bp::return_value_policy<bp::reference_existing_object>());

  bp::def("getLayerDrawer", &getLayerDrawer, bp::return_value_policy<bp::reference_existing_object>());
  bp::def("getPreviewDrawer", &getPreviewDrawer, bp::return_value_policy<bp::reference_existing_object>());

  bp::def("getPaintToolRegistrar", &getPaintToolRegistrar, bp::return_value_policy<bp::reference_existing_object>());
  bp::def("getImageFileTypeRegistrar", &getImageFileTypeRegistrar, bp::return_value_policy<bp::reference_existing_object>());
  bp::def("getAnimationFileTypeRegistrar", &getAnimationFileTypeRegistrar, bp::return_value_policy<bp::reference_existing_object>());
}

namespace pixpaint
{
namespace
{
  void loadScriptsImpl(const os_specific::filesystem::path& location, bp::object& mainNamespace)
  {
    if(os_specific::filesystem::exists(location)) {
      for(const auto& tool_script_file : os_specific::filesystem::directory_iterator(location)) {
        if(tool_script_file.path().filename().extension().string() == ".py") {
          bp::exec_file(tool_script_file.path().string().c_str(), mainNamespace, mainNamespace);
        }
      }
    }
  }

  void loadToolScripts(bp::object& mainNamespace)
  {
    auto location = os_specific::filesystem::current_path() /
                    os_specific::filesystem::path("scripts") /
                    os_specific::filesystem::path("tools");
    loadScriptsImpl(location, mainNamespace);
  }

  void loadImageFileTypeScripts(bp::object& mainNamespace)
  {
    auto location = os_specific::filesystem::current_path() /
                    os_specific::filesystem::path("scripts") /
                    os_specific::filesystem::path("image_filetypes");
    loadScriptsImpl(location, mainNamespace);
  }

  void loadAnimationFileTypeScripts(bp::object& mainNamespace)
  {
    auto location = os_specific::filesystem::current_path() /
                    os_specific::filesystem::path("scripts") /
                    os_specific::filesystem::path("animation_filetypes");
    loadScriptsImpl(location, mainNamespace);
  }

  void loadEffectScripts(bp::object& mainNamespace)
  {
    auto location = os_specific::filesystem::current_path() /
        os_specific::filesystem::path("scripts") /
        os_specific::filesystem::path("effects");
    loadScriptsImpl(location, mainNamespace);
  }
}
  bool initPythonExports()
  {
    try {
      PyImport_AppendInittab((char*)"pixpaint", PyInit_pixpaint);
      Py_Initialize();

      bp::object main_module = bp::import("__main__");
      bp::object main_namespace = main_module.attr("__dict__");
      bp::import("pixpaint");

      loadToolScripts(main_namespace);
      loadImageFileTypeScripts(main_namespace);
      loadAnimationFileTypeScripts(main_namespace);
      loadEffectScripts(main_namespace);
    } catch(const bp::error_already_set&) {
      PyErr_Print();
      if(Py_IsInitialized()) {
        Py_Finalize();
      }

      return false;
    }

    return true;
  }
}
