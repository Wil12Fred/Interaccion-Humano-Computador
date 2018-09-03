c++ -o main main.cc -I../include -L/System/Library/Frameworks -framework GLUT -framework OpenGL ../lib/libLeap.dylib
install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib main
