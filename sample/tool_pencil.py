######################
# Example Pencil Tool
######################
import pixpaint

class ExampleBrushTool(pixpaint.PaintTool):
  def __init__(self):
    super().__init__()
    
    # Add a size option, so that users can adjust the size / thickness of the
    # brush strokes
    self.addIntegerOption("Size", 1, 0, 100)
        
  # This is the function called when ever the mouse button is pressed on the drawing canvas
  def onMousePress(self, currentPoint, previousPoint, globalPoint, color, controlState, previewLayer, currentLayer):
    # Get the size of the brush, by getting the size option
    size = self.getIntegerOption("Size")
    
    # Get the layer drawer, and draw a soft line
    pixpaint.getLayerDrawer().drawLineSoft(currentPoint, \
                                           currentPoint, \
                                           color, \
                                           pixpaint.DrawParam(False, pixpaint.LineStyle(size, 0, False, pixpaint.ELineStyle.SolidLine)))
    return True
  
  # This is the function called when ever the mouse moves after the mouse pressed event occurs
  def onMouseMove(self, currentPoint, previousPoint, globalPoint, color, controlState, previewLayer, currentLayer):
    size = self.getIntegerOption("Size")
    pixpaint.getLayerDrawer().drawLineSoft(currentPoint, \
                                           previousPoint, \
                                           color, \
                                           pixpaint.DrawParam(False, pixpaint.LineStyle(size, 0, False, pixpaint.ELineStyle.SolidLine)))
    return True
  
  # This is the function called when ever a mouse button is released inside the drawing canvas
  def onMouseRelease(self, currentPoint, previousPoint, globalPoint, color, controlState, previewLayer, currentLayer):
    pixpaint.getImageManager().getImage().setModified(True)
    return True
    
  # The DrawRect is the rectangle area in the canvas that is redrawn / refreshed after the mouse release or key release event
  # have occured
  def getDrawRect(self):
    # Refresh the whole image
    image = pixpaint.getImageManager().getImage()
    return pixpaint.IntRect(0, 0, image.getWidth(), image.getHeight())
    
  def getDescription(self):
    return "Example Brush Tool"
    
pixpaint.getPaintToolRegistrar().registerTool("Example Brush Tool", "", ExampleBrushTool())
