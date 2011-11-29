# expects 3 args
# python findreplace.py <path> <find string> <replace string>

import os
import fileinput
import sys

allowed_extensions = [ ".h", ".cpp", ".m", ".mm", ".pbxproj", ".pch", ".plist" ]
forbidden_dirs = [ ".git", ".svn" ]

path, search_str, replace_str = sys.argv[1:4]

print "Replacing " + search_str + " with " + replace_str

def findReplaceFilename(path, filename):
    name = filename.replace(search_str, replace_str)
    if name != filename:
        os.rename(os.path.join(path, filename), os.path.join(path, name))
        return True
    return False

def findReplaceContents(path):
    f = open(path, "r")
    lines = f.readlines(99999999)
    f.close()
    f = open(path, "w")
    f.seek(0, os.SEEK_SET)
    for line in lines:
        replaced = line.replace(search_str, replace_str)
        if replaced != line:
            print "Replaced line in " + str(path)
            print replaced
        f.write(replaced)

for root, dirs, files in os.walk(path):
    for filename in files:
        if filename[filename.rfind("."):] not in allowed_extensions:
            continue
        findReplaceContents(os.path.join(root, filename))
        findReplaceFilename(root, filename)

    for dirname in dirs:
        if dirname in forbidden_dirs:
            dirs.remove(dirname)
            continue
        if findReplaceFilename(root, dirname):
            dirs.remove(dirname)
            dirs.append(dirname.replace(search_str, replace_str))
