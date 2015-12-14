import os,os.path
import sys
import shutil

def get_ndk_root():
    try:
        NDK_ROOT = os.environ['NDK_ROOT']
    except Exception:
        print "NDK_ROOT not defined."
        sys.exit(1)
    return NDK_ROOT

def get_num_of_cpu():
    try:
        platform = sys.platform
        if platform == 'win32':
            if 'NUMBER_OF_PROCESSORS' in os.environ:
                return int(os.environ['NUMBER_OF_PROCESSORS'])
            else:
                return 20
        else:
            from numpy.dishutils import cpuinfo
            return cpuinfo.cpu._getNCPUS()
    except Exception:
        print "Can't know cpuinfo, use default 1 cpu"
        return 1

def main():
    '''remove file  and copy the new file
    '''
    project = os.path.dirname(os.path.realpath(__file__))

    ndk_root = get_ndk_root()
    ndk_path = os.path.join(ndk_root, "ndk-build")

    num_or_cpu = get_num_of_cpu()

    ndk_module_path = os.path.join(project, "../..")
    command = '%s NDK_DEBUG=1 NDK_MODULE_PATH=%s'%(ndk_path, ndk_module_path)

    os.system(command)

if __name__ == '__main__':
    main()
