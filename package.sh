#!/bin/bash

# Usage :
#   Make sure you are cd'd into the project directory before running.
#   $ ./package.sh <Template_Path>
#
# Example :
#   $ ./package.sh ../TinderBox.app/Contents/Resources/Templates/Cocoa\ Touch/CinderGLKit/

cp -R . "$1"
rm -rf "$1/.git" "$1/package.sh" "$1/findreplace.py"
python findreplace.py "$1" "CinderGLKitTemplate" "«PREFIX»"
