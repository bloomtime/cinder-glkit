#!bin/bash
# Usage: sh package.sh <Template_Path>

cp -R . $1
rm -rf $1/.git
python findreplace.py $1 "Prefix_Prefix" "«PREFIX»"
