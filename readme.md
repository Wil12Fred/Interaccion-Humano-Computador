Requisitos:
- Framework OpenGL, GLUT
- Librería SDK Leap Motion (/usr/local/lib/libLeap.dylib)
- Librería libPNG (/usr/local/lib/libpng.dylib)
- Sistema Operativo macOS.

Compilación:
- c++ -o main main.cc -L/System/Library/Frameworks -framework GLUT -framework OpenGL /usr/local/lib/libLeap.dylib /usr/local/lib/libpng.dylib

Redirección de librería:
- install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib main
