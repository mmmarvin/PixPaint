######################
#
# Anti-Aliased Rect Tool
# by Marvin Manese
#
######################
import pixpaint

class AntiAliasedRectTool(pixpaint.PaintTool):
	def __init__(self):
		super().__init__()
		
		self.addIntegerOption("Size", 1, 0, 100)
		self.addFlagOption("Anti-Aliased", False)
		self.addFlagOption("Fill", False)
		
		self._mStartPoint = pixpaint.Point(0, 0)
		self._mEndPoint = pixpaint.Point(0, 0)
        
	def onMousePress(self, currentPoint, previousPoint, globalPoint, color, controlState, previewLayer, currentLayer):
		self._mStartPoint = currentPoint
		return False
  
	def onMouseMove(self, currentPoint, previousPoint, globalPoint, color, controlState, previewLayer, currentLayer):
		self._mEndPoint = currentPoint
	  
		size = self.getIntegerOption("Size")
		antialiased = self.getFlagOption("Anti-Aliased")
		fill = self.getFlagOption("Fill")
		
		previewLayer.clear(pixpaint.Color(0, 0, 0, 0))
		if fill:
			pixpaint.getPreviewDrawer().drawRectSoft(self._mStartPoint, \
													 self._mEndPoint, \
													 color, \
													 color,
													 pixpaint.DrawParam(antialiased, pixpaint.LineStyle(size, 0, False, pixpaint.ELineStyle.SolidLine)))
		else:
			pixpaint.getPreviewDrawer().drawRectSoft(self._mStartPoint, \
													 self._mEndPoint, \
													 color, \
													 pixpaint.Color(0, 0, 0, 0),
													 pixpaint.DrawParam(antialiased, pixpaint.LineStyle(size, 0, False, pixpaint.ELineStyle.SolidLine)))
			
		return True
  
	def onMouseRelease(self, currentPoint, previousPoint, globalPoint, color, controlState, previewLayer, currentLayer):
		size = self.getIntegerOption("Size")
		antialiased = self.getFlagOption("Anti-Aliased")
		fill = self.getFlagOption("Fill")
		
		previewLayer.clear(pixpaint.Color(0, 0, 0, 0))
		if fill:
			pixpaint.getLayerDrawer().drawRectSoft(self._mStartPoint, \
												   self._mEndPoint, \
												   color, \
												   color,
												   pixpaint.DrawParam(antialiased, pixpaint.LineStyle(size, 0, False, pixpaint.ELineStyle.SolidLine)))
		else:
			pixpaint.getLayerDrawer().drawRectSoft(self._mStartPoint, \
												   self._mEndPoint, \
												   color, \
												   pixpaint.Color(0, 0, 0, 0),
												   pixpaint.DrawParam(antialiased, pixpaint.LineStyle(size, 0, False, pixpaint.ELineStyle.SolidLine)))
		pixpaint.getImageManager().getImage().setModified(True)
											     
		return True
    
	def getDrawRect(self):
		image = pixpaint.getImageManager().getImage()
		return pixpaint.IntRect(0, 0, image.getWidth(), image.getHeight())
    
	def getDescription(self):
		return "Anti-Aliased Line Tool"
    
pixpaint.getPaintToolRegistrar().registerTool("Anti-Aliased Rect Tool", "res/icons/rect_tool.png", "aarect", "", AntiAliasedRectTool())
