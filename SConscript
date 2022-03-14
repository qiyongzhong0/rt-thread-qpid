from building import *

cwd = GetCurrentDir()
path = [cwd+'/inc']
src  = Glob('src/*.c')
 
group = DefineGroup('qpid', src, depend = ['PKG_USING_QPID'], CPPPATH = path)

Return('group')