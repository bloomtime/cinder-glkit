#!bin/bash

# Usage
#   Make sure you are cd'd into the project directory before running.
#   $> sh package.sh <Template_Path>

cp -R . $1
rm -rf $1/.git $1/package.sh $1/findreplace.py
python findreplace.py $1 "CinderGLKitTemplate" "«PREFIX»"
