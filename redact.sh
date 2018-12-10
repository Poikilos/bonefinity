#!/bin/sh
mv FrameworkDummy.h frameworkdummy.h
mv FrameworkDummy.cpp frameworkdummy.cpp
mv byter_32bgra.h RFile.h
mv byter_32bgra.cpp RFile.cpp
mv gbuffer32bgra.cpp RImage_bgra32.cpp
mv gbuffer32bgra.h RImage_bgra32.h
mv sprite.h RSprite_bgra32.h
mv sprite.cpp RSprite_bgra32.cpp
mv targa.h Targa.h
mv targa.cpp Targa.cpp
mv world.h RWorld.h
mv world.cpp RWorld.cpp
mv anim32bgra.h RAnim_bgra32.h
mv anim32bgra.cpp RAnim_bgra32.cpp
if [ ! -f "RGradient_bgra32.h" ]; then
  mv etc/gradient.h RGradient_bgra32.h
fi
if [ ! -f "RGradient_bgra32.cpp" ]; then
  mv etc/gradient.cpp RGradient_bgra32.cpp
fi
