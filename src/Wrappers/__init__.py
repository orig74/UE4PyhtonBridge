# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
import os
import ctypes
import glob,json
if 'PATH_FILE' in os.environ:
    path_file=os.environ['PATH_FILE']
    path_data=json.load(open(path_file))
    libfile=glob.glob(path_data['project_path']+'/Binaries/Linux/libUE4Editor*.so')[0]
    print('loading libfile',libfile)
else:
    libfile=''
#libfile=''
print('start loading lib file')
libc=ctypes.CDLL(libfile,mode=ctypes.RTLD_GLOBAL)
print('done loading lib file')
#libc.calledfrompython.argtypes=[]
#libc.calledfrompython.restype=ctypes.c_int
