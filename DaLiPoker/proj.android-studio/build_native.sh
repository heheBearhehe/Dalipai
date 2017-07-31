#!/bin/bash

cd ../proj.android

b=$(date +%s)
python build_native.py 
e=$(date +%s)

cp libs/armeabi/libcocos2dcpp.so ../proj.android-studio/daLiPoker/src/main/jniLibs/armeabi/

echo 'build cocos2dx.so cost='$(($e-$b))