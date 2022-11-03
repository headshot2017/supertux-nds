# script that converts supertux milestone 1 levels
# from flexlay (lisp) to json format

import sys
import os
import json

STATE_START = 0
STATE_OPEN_BRACKET = 1
STATE_GET_BRACKET_NAME = 2
STATE_GET_BRACKET_VALUES = 3
STATE_READING_STRING = 4

levels = sys.argv[1:]
if not levels:
    print("\nflexlay2json")
    print("Converts a SuperTux Milestone 1 level (Flexlay editor) to JSON format\n")
    print("Usage:")
    print("flexlay2json.py <filenames/all>")
    exit()

# get levels
if levels[0].lower() == "all":
    levels = []
    for f in os.listdir("."):
        if f.endswith(".stl"):
            levels.append(f)

def getLisp(lispstr, top, start=0, state=STATE_OPEN_BRACKET, level=0):
    lispinput = ""
    thisbracket = ""
    i = start
    offset = 0

    while lispstr[i] != ")" or level == 0:
        if i == len(lispstr)-1: return
        char = lispstr[i]

        if state == STATE_START:
            if char == " ":
                state = STATE_OPEN_BRACKET
                i += 1
                offset += 1
                continue

        elif state == STATE_OPEN_BRACKET:
            if char == "(":
                state = STATE_GET_BRACKET_NAME
            elif char == " ":
                i += 1
                offset += 1
                continue
            else:
                raise Exception("Got '%s' at pos %d, expected '('" % (char, i+1))

        elif state == STATE_GET_BRACKET_NAME:
            if (char == " " or char == "(") and lispinput:
                top[lispinput] = []
                thisbracket = lispinput
                lispinput = ""
                state = STATE_GET_BRACKET_VALUES
                i += 1
                offset += 1
                continue
            elif char != " " and char != "(":
                lispinput += char

        elif state == STATE_GET_BRACKET_VALUES:
            if char == " " and lispinput: # more values
                top[thisbracket].append(lispinput if not lispinput.isdigit() else int(lispinput))
                lispinput = ""
            elif char == "\"": # get string
                state = STATE_READING_STRING
            elif char == "(": # new list
                top[thisbracket].append({})
                newoffset = getLisp(lispstr, top[thisbracket][-1], i, STATE_GET_BRACKET_NAME, level+1)
                i += newoffset
                offset += newoffset
            elif char == "#": # bool
                top[thisbracket].append(lispstr[i+1] == "t")
                i += 1
                offset += 1
            elif char != " " and char != ")":
                lispinput += char

        elif state == STATE_READING_STRING:
            if char != "\"":
                lispinput += char
            else:
                state = STATE_GET_BRACKET_VALUES

        i += 1
        offset += 1

    # we reached ")"
    if lispinput:
        if state == STATE_GET_BRACKET_NAME:
            top[lispinput] = {}
            thisbracket = lispinput
        else:
            top[thisbracket].append(lispinput if not lispinput.isdigit() else int(lispinput))
            lispinput = ""
    if len(top[thisbracket]) == 1: top[thisbracket] = top[thisbracket][0]
    thisbracket = ""
    state = STATE_OPEN_BRACKET
    if level > 0:
        pass
    return offset

def fixDictArrays(lispdict):
    names = lispdict.keys()
    for name in names:
        #print name
        itsAllDicts = True # itsalldicks
        replacement = {}
        for item in lispdict[name]:
            if isinstance(item, dict):
                replacement[item.keys()[0]] = item.values()[0]
                if item.keys()[0] == "objects":
                    objects = []
                    for obj in replacement["objects"]:
                        replacementObj = {"name": obj.keys()[0]}
                        fixDictArrays(obj)
                        for objkey in obj[obj.keys()[0]]:
                            replacementObj[objkey] = obj[obj.keys()[0]][objkey]
                        objects.append(replacementObj)
                    replacement[item.keys()[0]] = objects
                elif item.keys()[0] == "reset-points":
                    resetpoints = []
                    if isinstance(item.values()[0], dict):
                        for point in replacement["reset-points"].values():
                            replacementpoint = {}
                            replacementpoint["x"] = point[0]["x"]
                            replacementpoint["y"] = point[1]["y"]
                            resetpoints.append(replacementpoint)
                    elif isinstance(item.values()[0], list):
                        for points in replacement["reset-points"]:
                            for point in points.values():
                                replacementpoint = {}
                                replacementpoint["x"] = point[0]["x"]
                                replacementpoint["y"] = point[1]["y"]
                                resetpoints.append(replacementpoint)
                    replacement["reset-points"] = resetpoints
            else:
                itsAllDicts = False
                break
        lispdict[name] = replacement

# start converting flexlay to json
for lvl in levels:
    print lvl
    lispstr = open(lvl).read().split("\n")
    for txt in lispstr:
        if txt.lstrip().startswith(";"): del lispstr[lispstr.index(txt)] # delete comments
    lispstr = " ".join(lispstr)
    final = {}

    getLisp(lispstr, final)
    fixDictArrays(final)

    open(lvl[:-4]+".json", "w").write(json.dumps(final))