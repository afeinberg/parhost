# -*-python-*-

import os

env = Environment()
env.Append(CPPFLAGS='-g -Wall -Wextra')
env.Append(CPPPATH=[os.path.join(os.curdir, "threadpool")])
env.Program(target='parhost',
            source=['parhost.cc'],
            LIBS=['boost_thread-mt'])
