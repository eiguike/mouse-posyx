call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
rd /s /q "C:\mouse-posyx"
timeout 10
windows_build.py
