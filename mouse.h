static int old_x, old_y;
bool existOld=0;

void Mouse_Luces(int x, int y){
	if(existOld){
		if(current_light>-1){
			float rot_x, rot_y;
			rot_y = (float)(y - old_y)*0.2;
			rot_x = (float)(x - old_x)*0.5;
			if(rot_y==0 && rot_x==0){
			} else {
				std::cout << rot_y << " " << rot_x << std::endl;
				Rotar_Luces_Latitud( LOCAL_MyLights[current_light],rot_y * DEGREE_TO_RAD );                          
				Rotar_Luces_Longitud( LOCAL_MyLights[current_light], rot_x * DEGREE_TO_RAD);                         
				LOCAL_MyLights[current_light]->pointAtInfinity[0] = LOCAL_MyLights[current_light]->position[0];    
				LOCAL_MyLights[current_light]->pointAtInfinity[1] = LOCAL_MyLights[current_light]->position[1];
				LOCAL_MyLights[current_light]->pointAtInfinity[2] = LOCAL_MyLights[current_light]->position[2];
				old_y = y;
				old_x = x;
				glutPostRedisplay();
			}
		}
	} else {
		old_x=x;
		old_y=y;
		existOld=1;
	}
}
