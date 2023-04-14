Option Explicit

Sub CreateOrganigram()
    Dim pptApp As PowerPoint.Application
    Dim pptPres As PowerPoint.Presentation
    Dim pptSlide As PowerPoint.Slide
    Dim smartArtLayout As SmartArtLayout
    Dim smartArt As PowerPoint.Shape
    Dim node As SmartArtNode
    
    ' Crea una nuova applicazione PowerPoint e una nuova presentazione
    Set pptApp = New PowerPoint.Application
    pptApp.Visible = True
    Set pptPres = pptApp.Presentations.Add
    Set pptSlide = pptPres.Slides.Add(1, ppLayoutBlank)
    
    ' Scegli un layout di organigramma SmartArt
    Set smartArtLayout = pptApp.SmartArtLayouts(88) 'Organigramma orizzontale
    
    ' Aggiungi lo SmartArt all'interno della diapositiva
    Set smartArt = pptSlide.Shapes.AddSmartArt(smartArtLayout, 50, 50, 600, 400)
    
    ' Modifica il testo delle forme dello SmartArt
    With smartArt.SmartArt.Nodes(1)
        .TextFrame2.TextRange.Text = "Producers"
        Set node = .Nodes.Add
        node.TextFrame2.TextRange.Text = "a. Creazione di 3 produttori: driver, military e material"
        Set node = .Nodes.Add
        node.TextFrame2.TextRange.Text = "b. Ogni produttore attende il semaforo corrispondente (sem_driver, sem_military o sem_material)"
        Set node = .Nodes.Add
        node.TextFrame2.TextRange.Text = "c. Una volta ottenuto il semaforo, il produttore chiama la funzione put(weight, range, col_index) con i parametri appropriati"
        Set node = .Nodes.Add
        node.TextFrame2.TextRange.Text = "d. La funzione put:"
    End With
    
    With smartArt.SmartArt.Nodes(2)
        .TextFrame2.TextRange.Text = "Consumers"
        Set node = .Nodes.Add
        node.TextFrame2.TextRange.Text = "a. Creazione di 3 consumatori: plane, truck e boat"
        Set node = .Nodes.Add
        node.TextFrame2.TextRange.Text = "b. Ogni consumatore attende il semaforo corrispondente (sem_plane, sem_truck o sem_boat)"
        Set node = .Nodes.Add
        node.TextFrame2.TextRange.Text = "c. Una volta ottenuto il semaforo, il consumatore chiama la funzione get(const char *name) con il nome appropriato"
        Set node = .Nodes.Add
        node.TextFrame2.TextRange.Text = "d. La funzione get:"
    End With
End Sub
