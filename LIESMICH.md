# lolQt

**Aus animierten GIFs und MP3-Dateien musikuntermalte Videos generieren**

## Voraussetzungen

  * Das [Microsoft Visual C++ 2010 SP1 Redistributable Package (x86)](http://www.microsoft.com/de-de/download/details.aspx?id=8328) muss installiert sein.

## Installation

 Laden Sie die aktuelle Version des [lolQt-Installationsprogramms](https://drive.google.com/folderview?id=0B3S-OBO0P8GMMlJZNzFLVDY2bFE&usp=sharing) herunter, starten Sie es folgen den Anweisungen.

## Bedienung in aller Kürze:

  * Gehen Sie ins Menü Extras/Einstellungen:
    - Prüfen Sie, ob der Pfad zum MEncoder korrekt ist.
    - Wählen Sie die Ausgabedatei (sollte die Endung .avi haben).
    - Passen Sie den Pfad zum Verzeichnis für temporäre Dateien an, falls notwendig.
    - Passen Sie die Bitrate fürs Audio-Encoding an, falls gewünscht.
  * Ziehen Sie ein animiertes GIFs auf die Bedienoberfläche. Die Animation wird sofort in einer Endlosschleife abgespielt.
  * Ziehen Sie eine Musikdatei (MP3, M4A) auf die Bedienoberfläche. Die Musik wird sofort abgespielt.
  * Tippen Sie im Takt zur Musik auf "Tipp auf mich im Takt!" oder wählen Sie die gewünschten Takte pro Minute in dem Eingabefeld links davon.
  * Wählen Sie den Versatz in Frames im Eingabefeld rechts davon, falls erforderlich. Damit beginnt die Animation im Video um die eingestellte Anzahl Frames verzögert. Damit sorgen Sie dafür, dass der Takt tatsächlich synchron zur Bewegung ist. Gegebenenfalls müssen Sie ein bisschen mit dem Wert experimentieren, bis es perfekt aussieht.
  * Klicken Sie auf "Video speichern", um das Video zu erzeugen. Es entsteht eine Datei im AVI-Format, in der die Bildsequenz aus dem GIF so oft wiederholt wird, dass sie exakt mit der Musik endet. Das generierte Video hat dieselben Ausmaße wie das GIF.

## Eigentümlichkeiten

Wenn Sie die Takte pro Minute ändern, ändert sich zwar auch die Abspielrate der Bildsequenz in der Bedienoberfläche, aber sie ist nur _mit Glück_ synchron zur Musik, weil die Routinen zum Lesen des GIFs und Abspielen als [```QMovie```](http://qt-project.org/doc/qt-5/QMovie.html) die Intervalle zwischen den Frames nicht korrekt berücksichtigen. Im gespeicherten Video ist die Synchronisation jedoch okay.

## To-do

  * Den Anwendern die Größe des Videos wählen lassen.
  * Automatische BPM-Erkennung.

## Copyright

Copyright (c) 2014 Oliver Lau, Heise Zeitschriften Verlag.

## Nutzungshinweise

Die Software "lolQt" wurde zu Lehr- und Demonstrationszwecken
geschaffen und ist nicht für den produktiven Einsatz vorgesehen.
Der Autor und der Heise Zeitschriften Verlag haften nicht für
Schäden, die aus der Nutzung der Software entstehen, und übernehmen
keine Gewähr für ihre Vollständigkeit, Fehlerfreiheit und Eignung
für einen bestimmten Zweck.

Falls Sie beabsichtigen, den Code in eigenen Produkten zu verwenden,
so ist das im Rahmen der GPL v3 kostenfrei gestattet (siehe die
Datei LICENSE für weitere Informationen).
