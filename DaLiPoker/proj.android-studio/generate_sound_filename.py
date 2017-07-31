#!/usr/bin/env python

import os
import sys


CHARACTOR_LIST = ["guolaoshi", "dami", "daxiong", "dali", "yujie"]
SOUND_FILE_ROOT_DIR = './daLiPoker/src/main/assets/sound/'

CONFIG_FILE_PATH = '../Classes/game/EffectSoundFileConfig.h'

def generate_sound_file():

  out = open(CONFIG_FILE_PATH, 'w')
  out.write('#ifndef EFFECT_SOUND_FILE_CONFIG_H')
  out.write('\n')
  out.write('#define EFFECT_SOUND_FILE_CONFIG_H')
  out.write('\n')
  out.write('\n')
  out.write('#include <string>')
  out.write('\n')
  out.write('using namespace std;')
  out.write('\n')
  out.write('\n')

  out.write('static string EFFECT_SOUND[] = {')
  out.write('\n')

  for name in CHARACTOR_LIST:
    d = SOUND_FILE_ROOT_DIR + name
    for filename in os.listdir(d):
      out.write('    "%s",' % filename)
      out.write('\n')

  out.write('};')
  out.write('\n')
  out.write('\n')

  # for name in CHARACTOR_LIST:
  #   d = SOUND_FILE_ROOT_DIR + name
  #   out.write('static string %s_SOUND[] = {' % name)
  #   out.write('\n')
    
  #   for filename in os.listdir(d):
  #     if os.path.isfile(os.path.join(d, filename)):
  #       out.write('    "%s",' % filename)
  #       out.write('\n')

  #   out.write('};')
  #   out.write('\n')
  #   out.write('\n')


  # out.write('static string EFFECT_SOUND[5][] = {')
  # out.write('\n')

  # out.write('    guolaoshi_SOUND,')
  # out.write('\n')
  # out.write('    dami_SOUND,')
  # out.write('\n')
  # out.write('    daxiong_SOUND,')
  # out.write('\n')
  # out.write('    dali_SOUND,')
  # out.write('\n')
  # out.write('    yujie_SOUND,')
  # out.write('\n')
  # out.write('};')
  # out.write('\n')
  # out.write('\n')

  out.write('#endif /* EFFECT_SOUND_FILE_CONFIG_H */')
  out.write('\n')

if __name__ == '__main__':
    try:
        generate_sound_file()
    except Exception as e:
        print e



#         //
# //  R.h
# //  DaLiPoker
# //
# //  Created by lugan on 5/22/16.
# //
# //

# #ifndef EFFECT_SOUND_FILE_CONFIG_H
# #define EFFECT_SOUND_FILE_CONFIG_H

# #include <string>
# using namespace std;

# static string DAMI_SOUND[] = {
# };

# #endif /* EFFECT_SOUND_FILE_CONFIG_H */
