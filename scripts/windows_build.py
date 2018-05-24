#!/usr/bin/python3

import urllib.request
import os
import shutil
import sys

openssl_setup = "http://slproweb.com/download/Win32OpenSSL-1_0_2o.exe"
cmake_setup = "http://cmake.org/files/v3.11/cmake-3.11.1-win64-x64.msi"

def download_file(url, file_name):
  print("[DOWNLOAD] Downloading: ", openssl_setup)
  url_info = urllib.request.urlopen(url)
  buffer = url_info.read()
  print("[DOWNLOAD] Complete!")
  print("[DOWNLOAD] Writing file into ", file_name)    
  with open(file_name, "wb") as f:
    f.write(buffer)
  print("[DOWNLOAD] Complete!")    	
  return True

def clean():
  print("[CLEAN] Cleaning folder")
  try:
    print("[CLEAN] removing mouse-posyx folder")
    shutil.rmtree("C:\\mouse-posyx")
    print("[CLEAN] mouse-posyx folder removed")
  except:
    print("[CLEAN] mouse-posyx folder already cleaned")
 
def downloading_files():
  return download_file(openssl_setup, "OpenSSL.exe")
  
def install_files():
  print("[INSTALL] Installing OpenSSL.exe")
  os.system("OpenSSL.exe /SILENT")
  print("[INSTALL] Complete!")
  return True
  
def git_clone(branch):
  print("[GIT-CLONE] Cloning mouse-posyx with branch: " + branch)
  os.system("git clone https://github.com/eiguike/mouse-posyx.git -b " + branch)
  os.chdir("mouse-posyx")
  
  print("[GIT-CLONE] Updating submodules")  
  os.system("git submodule init & git submodule update")
  
def building():
  os.chdir("libwebsockets")
  os.system("mkdir build")
  os.chdir("build")
  os.system("cmake .. -G \"NMake Makefiles\"")
  os.system("nmake")
  os.chdir("c:\mouse-posyx")
  os.system("mkdir build")
  os.chdir("build")
  os.system("cmake .. -G \"NMake Makefiles\"")
  error = os.system("nmake")
  if (error != 0):
    print("Error happened!")
    return False
  return True
  
def create_setup():
  error = os.system("ISCC /F C:\mouse-posyx_setup.iss")  
  if error != 0:
    print("Error happened!")
    return False
  return True

if __name__ == "__main__":
  if (len(sys.argv) == 1):
    branch = "master"
  else:
    branch = sys.argv[1]
	
  clean()
  downloading_files()
  install_files()
  git_clone(branch)
  result = building()
  create_setup()
  #publish(result)

    
  