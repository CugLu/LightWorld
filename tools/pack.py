import os
import os.path
import fnmatch, re
import shutil

def iter_find_files(path, fnexp):
	print path
	reobj = re.compile("\w+\.[hc]p?p?$")
	for root, dirs, files, in os.walk(path):
	 	for filename in files:
	 		if reobj.match(filename):
	 			print root, filename
	 			src = os.path.join(root, filename)
	 			des = src.replace(path, path+"Tmp")
	 			directory = os.path.dirname(des)
	 			if not os.path.exists(directory):
	 				os.makedirs(directory)
	 			shutil.copy(src, des)

def copyFiles(path, newPath, extensions):
	for root, dirs, files, in os.walk(path):
	    for filename in files:
	    	# if filename.endswith(('.h', '.hpp', 'cpp', '.c', '.dll', '.jpg', '.b3d', '.lib')):
	    	if filename.endswith(extensions):
				print root, filename
	 			src = os.path.join(root, filename)
	 			des = src.replace(path, newPath)
	 			directory = os.path.dirname(des)
	 			if not os.path.exists(directory):
	 				os.makedirs(directory)
	 			shutil.copy(src, des)

def main():
	python_dir = os.path.dirname(os.path.realpath(__file__))
	print python_dir

	copyFiles(python_dir, python_dir+"tmp", ('.h', '.hpp', 'cpp', '.c', '.dll', '.jpg', '.b3d', '.lib'))
	

if __name__ == '__main__':
	main()