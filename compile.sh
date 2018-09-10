#ESTA COMPILACIÓN ES PARA EL SISTEMA OPERATIVO OS X
c++ -o main main.cc -I../include -L/System/Library/Frameworks -framework GLUT -framework OpenGL ../lib/libLeap.dylib /usr/local/lib/libpng.dylib
if [ $? -ne 0 ]; then 
	echo "compilation error"
else
	install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib main
	./main
fi
