Requisitos:
- Framework OpenGL, GLUT
- Leap Motion SDK v2.3.1 (Mac OS: /usr/local/lib/libLeap.dylib) (Linux: LEAPSDK/lib/x64/libLeap.so)
- Librería libPNG (/usr/local/lib/libpng.dylib)

Compilación servidor (Mac OS):
- c++ -std=c++11 server.cpp -o server

Compilación cliente (Mac OS):
- c++ -o client -std=c++11 client.cc -I./ -I./Leap -I./Model -Wno-deprecated -framework GLUT -framework OpenGL /usr/local/lib/libLeap.dylib -lpng

Redirección de librería (Mac OS), se detalla mejor en el archivo compile.sh:
- install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib main

Compilación servidor (Linux):
- c++ -std=c++11 server.cpp -o server -pthread

Compilación cliente (Linux):
- c++ -o client -std=c++11 client.cc -I./ -I./Leap -I./Model -Wno-deprecated -lGL -lGLU -lglut [LEAP]/LEAPSDK/lib/x64/libLeap.so -Wl,-rpath,[LEAP]/LeapSDK/lib/x64 -lpng -pthread

Recomendación uso Leap Motion
- Posición frente a la pantalla, entre 15-30 cm.
- Iluminación por luz natural

En el siguiente video detallamos las características de interacción del juego:
[![Alt text for your video](https://github.com/Wil12Fred/InteraccionHumanoComputador/blob/master/previus.png)](https://youtu.be/I3TlLthCp1Q)
