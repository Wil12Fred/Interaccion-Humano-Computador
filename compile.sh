#ESTA COMPILACIÓN ES PARA EL SISTEMA OPERATIVO OS X
c++ -o main -std=c++11 main.cc -Wno-deprecated -L/System/Library/Frameworks -framework GLUT -framework OpenGL /usr/local/lib/libLeap.dylib /usr/local/lib/libpng.dylib
if [ $? -ne 0 ]; then 
	echo "compilation error"
else
	install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib main
	./main
fi
