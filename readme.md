Requisitos:
- Framework OpenGL, GLUT
- Libería SDK Leap Motion (../lib/libLeap.dylib)
- Sistema Operativo macOS.

Compilación:
- c++ -o main main.cc -I../include -L/System/Library/Frameworks -framework GLUT -framework OpenGL ../lib/libLeap.dylib /usr/local/lib/libpng.dylib

Redirección de librería:
- install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib main
