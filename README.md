# lolQt

**Merge animated GIFs and background music into video**

## Prerequisites

  * [Microsoft Visual C++ 2010 SP1 Redistributable Package (x86)](http://www.microsoft.com/de-de/download/details.aspx?id=8328)

## Installation

 [Download](https://drive.google.com/folderview?id=0B3S-OBO0P8GMMlJZNzFLVDY2bFE&usp=sharing) the most recent version of the installer, run it, and follow the instructions.

## Usage in a nutshell

  * Go to Extras/Settings:
    - Check path to MEncoder binary.
    - Choose output file (AVI).
    - Change path to temporary directory if necessary.
    - Change audio bitrate if necessary.
  * Drop an animated GIF onto the GUI. And endless repetition of the frame sequence is displayed straightaway.
  * Drop a music file (MP3) onto the GUI. The music will play immediately.
  * Tap on "Beat me!" according to the rhythm to compute beats per minute, or choose bpm in the spin box.
  * Click "Save frames" to write the output file to disk. An AVI will be written with the sequence of the GIF's frames repeated as long as the music lasts. The sequence will be in sync with the music. The generated video has the same dimensions as the GIF.

## Quirks

Changing bpm will change the display speed of the GIF's frame sequence. Unfortunately the GUI display will be in sync with the music _only by luck_, because the routines used to read the GIF and play it as a [```QMovie```](http://qt-project.org/doc/qt-5/QMovie.html) do not properly respect the intervals between the frames. Nevertheless, the frame sequence and the music in the written video will be in sync dead sure.

## To-do

  * Let the user choose the video dimensions.
  * Automatic bpm detection.

## Copyright

Copyright (C) 2014 Oliver Lau, Heise Zeitschriften Verlag.

**lolQt** comes with ABSOLUTELY NO WARRANTY. It's free software, and you are welcome to redistribute it under certain conditions. See LICENSE for details.
