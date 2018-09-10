Compilación para el sistema operativo OS X
Requisitos:
- Framework OpenGL, GLUT
- Libería SDK Leap Motion (../lib/libLeap.dylib)
c++ -o main main.cc -I../include -L/System/Library/Frameworks -framework GLUT -framework OpenGL ../lib/libLeap.dylib
install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib main
