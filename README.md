# Mouse Posyx
---
This application is possible to control your mouse and keyboard throught any modern browser in smartphone, the communication is made throught LAN, its backend is developed with libwebsockets (C) and the frontend is developed with Pure Javascript and HTML.

### Building in GNU/Linux:
1. Clone this repository
> $ git clone https://github.com/eiguike/mouse-posyx.git
2. Build and install libwebsockets, you can read all the instructions here.
3. In root folder:
> $ mkdir build; cd build
> $ cmake ..
> $ make
4. Execute the application
> $ ./mouse-posyx
5. Access the application throught IP:8000 in your favorite browser.
6. Enjoy!

---

### Building in Windows:
1. Install OpenSSL, cmake and Visual Studio
2. Clone this repository
> git clone https://github.com/eiguike/mouse-posyx.git
2. Build and install libwebsockets, you can read all the instructions here.
3. In root folder:
> md build; cd build
> cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RELEASE
> nmake
4. Move libwebsockets.dll to build folder.
5. Execute the application
> mouse-posyx.exe
6. Access the application throught IP:8000 in your favorite browser.
7. Enjoy!


---

## Author:
@[eiguike](https://github.com/eiguike) - [rick.eihara@gmail.com](mailto:rick.eihara@gmail.com)

