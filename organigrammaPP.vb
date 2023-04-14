Option Explicit

Sub CreateOrganigram()
    Dim pptApp As PowerPoint.Application
    Dim pptPres As PowerPoint.Presentation
    Dim pptSlide As PowerPoint.Slide
    Dim producerShape As PowerPoint.Shape
    Dim consumerShape As PowerPoint.Shape
    
    ' Crea una nuova applicazione PowerPoint e una nuova presentazione
    Set pptApp = New PowerPoint.Application
    pptApp.Visible = True
    Set pptPres = pptApp.Presentations.Add
    Set pptSlide = pptPres.Slides.Add(1, ppLayoutBlank)
    
    ' Aggiungi la forma Producers e il testo
    Set producerShape = pptSlide.Shapes.AddTextbox(msoTextOrientationHorizontal, 50, 50, 300, 100)
    With producerShape.TextFrame.TextRange
        .Text = "Producers" & vbCrLf & _
                "a. Creazione di 3 produttori: driver, military e material" & vbCrLf & _
                "b. Ogni produttore attende il semaforo corrispondente (sem_driver, sem_military o sem_material)" & vbCrLf & _
                "c. Una volta ottenuto il semaforo, il produttore chiama la funzione put(weight, range, col_index) con i parametri appropriati" & vbCrLf & _
                "d. La funzione put:"
        .ParagraphFormat.SpaceAfter = 10
    End With
    
    ' Aggiungi la forma Consumers e il testo
    Set consumerShape = pptSlide.Shapes.AddTextbox(msoTextOrientationHorizontal, 50, 200, 300, 100)
    With consumerShape.TextFrame.TextRange
        .Text = "Consumers" & vbCrLf & _
                "a. Creazione di 3 consumatori: plane, truck e boat" & vbCrLf & _
                "b. Ogni consumatore attende il semaforo corrispondente (sem_plane, sem_truck o sem_boat)" & vbCrLf & _
                "c. Una volta ottenuto il semaforo, il consumatore chiama la funzione get(const char *name) con il nome appropriato" & vbCrLf & _
                "d. La funzione get:"
        .ParagraphFormat.SpaceAfter = 10
    End With
End Sub
