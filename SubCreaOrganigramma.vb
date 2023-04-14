Sub CreaOrganigramma()

    Dim shp As Shape
    Dim x As Integer, y As Integer
    Dim w As Integer, h As Integer
    
    'Crea la forma per l'inizializzazione del contatore della colonna corrente
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, 50, 50, 120, 60)
    shp.TextFrame.TextRange.Text = "Initialiser first_empty_row à -1"
    x = shp.Left + shp.Width
    y = shp.Top + shp.Height / 2
    
    'Crea la forma per la selezione del tempo di attesa
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 50, y - 30, 120, 60)
    shp.TextFrame.TextRange.Text = "Sélectionner un temps d'attente aléatoire (8 ou 12)"
    
    'Crea la forma per l'attesa
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 50, y + 30, 120, 60)
    shp.TextFrame.TextRange.Text = "Attendre pendant le temps d'attente sélectionné"
    
    'Crea la forma per il blocco del mutex per il buffer
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 150, y - 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Verrouiller le mutex pour le buffer"
    
    'Crea la forma per il controllo del contatore della colonna corrente
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartDecision, x + 150, y, 120, 60)
    shp.TextFrame.TextRange.Text = "Vérifier si le compteur de la colonne courante a atteint sa limite"
    
    'Crea la forma per l'incremento del contatore della colonna corrente
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 150, y + 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Incrémenter le compteur pour la colonne courante"
    
    'Crea la forma per la ricerca della prima riga vuota nel buffer per la colonna corrente
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 250, y - 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Trouver la première ligne vide dans le buffer pour la colonne courante"
    
    'Crea la forma per il controllo della presenza di una riga vuota nel buffer per la colonna corrente
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartDecision, x + 250, y, 120, 60)
    shp.TextFrame.TextRange.Text = "Si une ligne vide est trouvée"
    'Crea la forma per la generazione di un numero casuale nella gamma specificata
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 250, y + 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Générer un nombre aléatoire dans la plage spécifiée"

    'Crea la forma per l'assegnazione del peso con il numero casuale aggiunto al buffer
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 350, y - 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Attribuer le poids avec le nombre aléatoire ajouté au buffer"

    'Crea la forma per il controllo del completamento della riga corrente
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartDecision, x + 350, y, 120, 60)
    shp.TextFrame.TextRange.Text = "Vérifier si la ligne courante est complètement remplie"

    'Crea la forma per il calcolo del peso totale per la riga corrente e l'assegnazione di un ID di convoglio
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 350, y + 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Calculer le poids total pour la ligne courante et attribuer un ID de convoi"

    'Crea la forma per il blocco del mutex per l'ID di convoglio
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 450, y - 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Verrouiller et déverrouiller le mutex pour convoi_id"

    'Crea la forma per la determinazione del tipo di trasporto per la riga corrente in base al peso totale
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 450, y, 120, 60)
    shp.TextFrame.TextRange.Text = "Déterminer le type de transport pour la ligne courante en fonction du poids total"

    'Crea la forma per la segnalazione della disponibilità dell'aereo
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 550, y - 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Si < 10001, signaler qu'un avion est disponible"

    'Crea la forma per la segnalazione della disponibilità del camion
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 550, y, 120, 60)
    shp.TextFrame.TextRange.Text = "Si entre 10001 et 30001, signaler qu'un camion est disponible"

    'Crea la forma per la segnalazione della disponibilità della nave
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 550, y + 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Si > 30001, signaler qu'un bateau est disponible"
    'Crea la forma per lo sblocco del mutex per il buffer
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 650, y - 60, 120, 60)
    shp.TextFrame.TextRange.Text = "Déverrouiller le mutex pour le buffer"

    'Crea la forma per l'inizializzazione di tmp_i a -1
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x, y + 150, 120, 60)
    shp.TextFrame.TextRange.Text = "Initialiser tmp_i à -1"

    'Crea la forma per il calcolo della soglia inferiore per il tipo di trasporto
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 150, y + 150, 120, 60)
    shp.TextFrame.TextRange.Text = "Définir les bornes inférieure et supérieure pour le type de transport"

    'Crea la forma per il blocco del mutex per il buffer
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 150, y + 210, 120, 60)
    shp.TextFrame.TextRange.Text = "Verrouiller le mutex pour le buffer"

    'Crea la forma per la ricerca del convoglio appropriato
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 250, y + 150, 120, 60)
    shp.TextFrame.TextRange.Text = "Trouver le convoi approprié"

    'Crea la forma per il controllo della presenza di un convoglio corrispondente
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartDecision, x + 250, y + 210, 120, 60)
    shp.TextFrame.TextRange.Text = "Vérifier si un convoi correspondant a été trouvé"

    'Crea la forma per la cancellazione del buffer per il convoglio trovato
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 250, y + 270, 120, 60)
    shp.TextFrame.TextRange.Text = "Effacer le buffer pour le convoi"

    'Crea la forma per il rilascio dei semafori
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 350, y + 210, 120, 60)
    shp.TextFrame.TextRange.Text = "Libérer les sémaphores"

    'Crea la forma per l'incremento del numero di convogli elaborati
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 450, y + 150, 120, 60)
    shp.TextFrame.TextRange.Text = "Augmenter le nombre de convois traités"

    'Crea la forma per ilblocco del mutex per il numero di convogli elaborati
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartProcess, x + 550, y + 90, 120, 60)
    shp.TextFrame.TextRange.Text = "Verrouiller et déverrouiller le mutex pour convois_processed"

    'Crea la forma per il completamento dell'organigramma
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeFlowchartTerminator, x + 650, y + 150, 120, 60)
    shp.TextFrame.TextRange.Text = "Fin"

    'Crea le connessioni tra le forme
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 170, y, 220, y)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 170, y + 60, 220, y + 60)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 270, y - 30, 320, y - 30)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 270, y + 30, 320, y + 30)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 390, y - 60, 440, y - 60)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 390, y, 440, y)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 390, y + 60, 440, y + 60)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 490, y - 60, 540, y - 60)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 490, y, 540, y)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 590, y - 60, 640, y - 60)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 590, y, 640, y)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 590, y + 60, 640, y + 60)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 690, y - 60, 740, y - 60)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 690, y, 740, y)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 690, y + 60, 740, y + 60)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 90, y + 120, 140, y + 120)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 270, y + 120, 320, y + 120)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 270, y + 180, 320, y + 180)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 370, y + 120, 420, y + 120)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 370, y + 180, 420, y + 180)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 470, y + 120, 520, y + 120)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 570, y + 120, 620, y + 120)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 570, y + 180, 620, y + 180)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 670, y + 120, 720, y + 120)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 670, y + 180, 720, y + 180)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 70, y + 240, 120, y + 240)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 270, y + 240, 320, y + 240)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartDecision, 270, y + 300, 320, y + 300)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 270, y + 360, 320, y + 360)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 370, y + 300, 420, y + 300)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 470, y + 240, 520, y + 240)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 570, y + 240, 620, y + 240)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 570, y + 300, 620, y + 300)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 670, y + 240, 720, y + 240)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartProcess, 670, y + 300, 720, y + 300)
    ActivePresentation.Slides(1).Shapes.AddConnector(msoConnectorFlowchartTerminator, 670, y + 360, 720, y + 360)
    'Evidenzia tutte le forme dell'organigramma
    ActivePresentation.Slides(1).Shapes.Range(Array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23)).Select

    'Imposta lo stile della forma e del testo dell'organigramma
    With ActivePresentation.Slides(1).Shapes.Range(Array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23)).ShapeStyle
    .FillColor.SchemeColor = ppFillBackground
    .Fill.Transparency = 0.9
    .Line.Visible = msoFalse
    .Effect.Type = msoAnimEffectNone
    .TextFrame.TextRange.Font.Name = "Arial"
    .TextFrame.TextRange.Font.Size = 14
    .TextFrame.TextRange.ParagraphFormat.Alignment = ppAlignCenter
    End With

    'Aggiunge il titolo all'organigramma
    Set shp = ActivePresentation.Slides(1).Shapes.AddShape(msoShapeRectangle, x - 60, y - 100, 840, 60)
    shp.Fill.ForeColor.RGB = RGB(180, 198, 231)
    shp.TextFrame.TextRange.Text = "Organigramme pour les fonctions put() et get():"
    shp.TextFrame.TextRange.Font.Name = "Arial"
    shp.TextFrame.TextRange.Font.Size = 22
    shp.TextFrame.TextRange.ParagraphFormat.Alignment = ppAlignCenter

End Sub