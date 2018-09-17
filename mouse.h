static int old_x, old_y;
void Mouse_Luces(int x, int y){
        float rot_x, rot_y;
        rot_y = (float)(old_y - y);
        rot_x = (float)(x - old_x);
        Rotar_Luces_Latitud( LOCAL_MyLights[current_light],rot_y*DEGREE_TO_RAD );                          
        Rotar_Luces_Longitud( LOCAL_MyLights[current_light], rot_x*DEGREE_TO_RAD);                         
        LOCAL_MyLights[current_light]->pointAtInfinity[0] = LOCAL_MyLights[current_light]->position[0];    
        LOCAL_MyLights[current_light]->pointAtInfinity[1] =     LOCAL_MyLights[current_light]->position[1];
        LOCAL_MyLights[current_light]->pointAtInfinity[2] =     LOCAL_MyLights[current_light]->position[2];
        old_y = y;
        old_x = x;
        glutPostRedisplay();
}

void Mouse_Spot(int x, int y) {
    float rot_x, rot_y;
    rot_y = (float) (old_y - y);
    rot_x = (float) (x - old_x);
    Rotar_Spot_Latitud(LOCAL_MyLights[current_light], rot_y * DEGREE_TO_RAD);
    Rotar_Spot_Longitud(LOCAL_MyLights[current_light], rot_x * DEGREE_TO_RAD);
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

void Mouse_Spot_Abrir_Cerrar(int x, int y) {
    float step;
    step = (float) (y - old_y);
    old_y = y;
    if (LOCAL_MyLights[current_light]->spotCutOff + step < 90 && LOCAL_MyLights[current_light]->spotCutOff + step > 0)
        LOCAL_MyLights[current_light]->spotCutOff += step;
    LOCAL_MyLights[current_light]->needsUpdate = TRUE;
    glutPostRedisplay();
}

void Mouse_Luces_Acercar_Alejar(int x, int y){
        float step;
        step = (float) (y - old_y) / 20.0f;
        old_y = y;
        Acercar_Alejar_Luces( LOCAL_MyLights[current_light], step );
        glutPostRedisplay();
}

void MouseMotion(int x, int y) {
    old_y = y;
    old_x = x;
}

void mouse(int button, int state, int x, int y){                                              
        old_x = x;
        old_y = y;
        switch(button){
                case GLUT_LEFT_BUTTON:
                        if(current_light > 0){
                                if(current_light == 2 && spot_move == 1){                     
                                        if (state == GLUT_DOWN)                               
                                                glutMotionFunc(Mouse_Spot_Abrir_Cerrar);      
                                        if (state == GLUT_UP){                                
                                                glutPassiveMotionFunc(Mouse_Spot);            
                                                glutMotionFunc(NULL);                         
                                        }
                                }else{
                                        if (state == GLUT_DOWN)                               
                                                glutMotionFunc(Mouse_Luces_Acercar_Alejar);   
                                        if (state == GLUT_UP){                                
                                                glutPassiveMotionFunc(Mouse_Luces);           
                                                glutMotionFunc(NULL);                         
                                        }
                                }
                        }else{
                                /*switch(MiCamara->camMovimiento){                              
                                        case CAM_EXAMINAR:                                    
                                                if (state == GLUT_DOWN) glutMotionFunc(Zoom); 
                                                if (state == GLUT_UP){                        
                                                        glutPassiveMotionFunc(Examinar);      
                                                        glutMotionFunc(NULL);                 
                                                }                                             
                                                break;                                        
                                        case CAM_PASEAR:                                      
                                                if (state == GLUT_DOWN) glutMotionFunc(Andar);
                                                if (state == GLUT_UP) glutMotionFunc(NULL);   
                                                break;                                        
                                }*/
                        }
                        break;
                case GLUT_RIGHT_BUTTON:
                        if (state == GLUT_DOWN){                                              
                                printf("Right Buttom\n");                                     
                        }
                        break;
                default:
                        break;
        }
        glutPostRedisplay();
}
