# Descripci'on:
Este ejecutable crea un ambiente 3D junto con un objeto 3D, y el usuario podra cambiar din'amicamente los tipos de iluminaci'on junto con su origen y vector.

# Requisitos generales
>-Requisito 1: Tener ambiente 3D

>-Requisito 2: Tener objeto 3D

>-Requisito 3: Tener iluminaci'on de ambiente

>-Requisito 4: Tener iluminaci'on difusa

>-Requisito 5: Tener iluminaci'on especular

>-Requisito 6: Visualizar el origen y vector de iluminaci'on

>-Requisito 7: Manejar din'amicamente la posici'on de origen de iluminaci'on

>-Requisito 8: Manejar din'amicamente el vector de iluminaci'on.

# Requisitos especificos
>-Requisito 1: Realizarlo con codigo c++

>-Requisito 2: Realizarlo con el framework OpenGL

>-Requisito 3: Cambiar entre tipo de iluminaci'on mediante el teclado

>-Requisito 4: Cambiar entre el objeto 'origen de iluminaci'on' y 'vector de iluminaci'on' mediante teclado

>-Requisito 5: Manejar el origen y vector de iluminaci'on mediante el mouse

# Compilaci'on:
  g++ -o iluminandoTortuga main.cpp Camara.cpp light.cpp primitivas.cpp Vector_tools.cpp -L/System/Library/Frameworks -framework GLUT -framework OpenGL
