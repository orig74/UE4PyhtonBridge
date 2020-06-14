# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
import os
import ctypes
import glob,json
if 'UE4EDITOR_SO' in os.environ:
    libfile=os.environ['UE4EDITOR_SO']
    print('loading libfile',libfile)
else:
    #no need python is called rfrom the main procces and should have all the simbols
    libfile=''
#libfile=''
print('start loading lib file')
libc=ctypes.CDLL(libfile,mode=ctypes.RTLD_GLOBAL)
print('done loading lib file')
