# Mouse Posyx
---
Control your mouse and keyboard throught your browser's smartphone. [[video]](https://www.youtube.com/watch?v=foAUzgEAnbc)

This application uses Websockets to create a connection between the host
and the browser and it is compatible for Windows and Linux.

### Build instructions:
---
<details>
    <summary>
Building for Linux (Click to expand)
    </summary>
1. Clone this repository

```
 $ git clone https://github.com/eiguike/mouse-posyx.git
```

2. Install the prerequisites packages:
```
$ sudo apt install cmake zlib1g-dev libx11-dev libxtst-dev
```

3. Initialize submodule folder and update it

```
 $ git submodule init; git submodule update
```

4. Build libwebsockets and install

```
 $ cd libwebsockets;mkdir build;cd build;cmake ..;make; sudo make install;
```
5. In mouse-posyx's folders, execute the following commands to build:

```
 $ mkdir build; cd build
 $ cmake ..
 $ make
```

6. Copy libwebsockets's implementations to the proper folder
```
$ sudo cp /usr/local/lib/libwebsockets* /usr/lib
```

7. Execute the application
```
$ ./mouse-posyx
```

8. Access the application throught IP:8000 in your favorite browser.
9. Enjoy!

</details>

<details>
    <summary>
Building for Windows (Click to expand)
    </summary>
1. Install OpenSSL, cmake and Visual Studio

2. Clone this repository

3. Build and install libwebsockets

4. In root folder, create a 'build' folder
```
 md build; cd build
```

5a. Prepare the development enviroment to compile
```
 cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RELEASE
```
5b. Prepare the development enviroment to compile, with OpenSSL activated:
```
 cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DUSE_SSL=TRUE
```

6. Compile
```
 nmake
```

7. Execute the application

8. Access the application throught IP:8000 in your favorite browser.

9. Enjoy!
</details>
<details>
    <summary>
Building for MacOSX (Click to expand)
    </summary>
1. Clone this repository

```
 $ git clone https://github.com/eiguike/mouse-posyx.git
```

2. Initialize submodule folder and update it

```
 $ git submodule init; git submodule update
```

3. Build libwebsockets and install

```
 $ cd libwebsockets;mkdir build;cd build;cmake .. -DLWS_WITH_SSL=OFF -DLWS_WITHOUT_BUILTIN_SHA1=OFF;make; sudo make install;
```
4. In mouse-posyx's folders, execute the following commands to build:

```
 $ mkdir build; cd build
 $ cmake ..
 $ make
```

5. Execute the application
```
$ ./mouse-posyx
```

7. Access the application throught IP:8000 in your favorite browser.
8. Enjoy!

</details>

---

### Binaries release:
---
Binaries are only available for Windows, and you can download [here](https://github.com/eiguike/mouse-posyx/files/1978295/mouse-posyx_setup.zip)

## Author:
@[eiguike](https://github.com/eiguike) - [rick.eihara@gmail.com](mailto:rick.eihara@gmail.com)
