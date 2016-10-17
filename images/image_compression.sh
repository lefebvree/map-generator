#!/bin/bash

echo Converting ppm images to png...
mogrify -format png images/*.ppm

echo Deleting old ppm files...
rm images/*.ppm

echo Done
