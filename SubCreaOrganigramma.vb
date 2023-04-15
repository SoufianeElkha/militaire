Option Explicit

Sub CreateOrganigram()
    Dim PowerPointApp As Object
    Dim myPresentation As Object
    Dim mySlide As Object
    Dim myShape As Object
    
    ' Create an instance of PowerPoint
    On Error Resume Next
    Set PowerPointApp = GetObject(class:="PowerPoint.Application")
    If PowerPointApp Is Nothing Then
        Set PowerPointApp = CreateObject(class:="PowerPoint.Application")
    End If
    PowerPointApp.Visible = True
    On Error GoTo 0
    
    ' Create a new presentation
    Set myPresentation = PowerPointApp.Presentations.Add
    
    ' Add a new slide and set the layout
    Set mySlide = myPresentation.Slides.Add(1, ppLayoutBlank)
    
    ' Add the main rectangle for the put function
    Set myShape = mySlide.Shapes.AddShape(Type:=msoShapeRectangle, Left:=100, Width:=200, Top:=50, Height:=100)
    myShape.TextFrame.TextRange.Text = "put function"
    
    ' Add the rectangles for the inner logic of the put function
    Set myShape = mySlide.Shapes.AddShape(Type:=msoShapeRectangle, Left:=350, Width:=200, Top:=50, Height:=50)
    myShape.TextFrame.TextRange.Text = "Find first empty row"
    
    Set myShape = mySlide.Shapes.AddShape(Type:=msoShapeRectangle, Left:=350, Width:=200, Top:=150, Height:=50)
    myShape.TextFrame.TextRange.Text = "Calculate total weight and set convoi_id"
    
    ' Add the main rectangle for the producer function
    Set myShape = mySlide.Shapes.AddShape(Type:=msoShapeRectangle, Left:=100, Width:=200, Top:=200, Height:=100)
    myShape.TextFrame.TextRange.Text = "producer function"
    
    ' Add the rectangles for the inner logic of the producer function
    Set myShape = mySlide.Shapes.AddShape(Type:=msoShapeRectangle, Left:=350, Width:=200, Top:=200, Height:=50)
    myShape.TextFrame.TextRange.Text = "Determine producer type"
    
    Set myShape = mySlide.Shapes.AddShape(Type:=msoShapeRectangle, Left:=350, Width:=200, Top:=300, Height:=50)
    myShape.TextFrame.TextRange.Text = "Call put function"
    
    ' Add connectors
    Set myShape = mySlide.Shapes.AddConnector(Type:=msoConnectorStraight, BeginX:=300, BeginY:=100, EndX:=350, EndY:=100)
    Set myShape = mySlide.Shapes.AddConnector(Type:=msoConnectorStraight, BeginX:=300, BeginY:=250, EndX:=350, EndY:=250)
End Sub
