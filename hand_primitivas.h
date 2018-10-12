void draw_finger(Finger& finger){
        Bone::Type boneType = static_cast<Bone::Type>(3);
        Vector boneEndPast=finger.bone(boneType).nextJoint();
        for (int b = 2; b >-1; --b) {
                boneType = static_cast<Bone::Type>(b);
                Bone bone = finger.bone(boneType);
                //Vector boneStart = bone.prevJoint();
                Vector boneEnd = bone.nextJoint();
                //std::cout << boneStart.x << " " << boneStart.y << " " << boneStart.z << std::endl; 
                //std::cout << boneEnd.x << " " << boneEnd.y << " " << boneEnd.z << std::endl; 
                glColor3f(1.0, 1.0, 0.0);
		draw_line(getPos(objl::Vector3(boneEndPast.x, boneEndPast.y,boneEndPast.z)), getPos(objl::Vector3(boneEnd.x,boneEnd.y,boneEnd.z)));
                glColor3f(0.0, 1.0, 0.0);
		draw_pixel(getPos(objl::Vector3(boneEnd.x,boneEnd.y,boneEnd.z)));
                boneEndPast=boneEnd;
        }   
        //std::cout << std::endl;
}

void draw_hand(Hand& hand){
	//std::cout << hand.arm().elbowPosition() << std::endl;
	Vector wristPosition = hand.arm().wristPosition();
	Vector palmPosition = hand.palmPosition();
	Vector palmNormal = /*hand.direction();*/hand.palmNormal();
	Vector sphereCenter = hand.sphereCenter();
	objl::Vector3 Wrist = getPos(objl::Vector3(wristPosition.x, wristPosition.y, wristPosition.z));
	objl::Vector3 Palm = (objl::Vector3(palmPosition.x, palmPosition.y, palmPosition.z));
	objl::Vector3 PalmNormal = (objl::Vector3(palmNormal.x, palmNormal.y, palmNormal.z));
	objl::Vector3 SphereCenter = getPos(objl::Vector3(sphereCenter.x,sphereCenter.y,sphereCenter.z));
	//const Finger finter=*(hand.fingers().begin());
	draw_sphere(Wrist, 10.0);
	draw_sphere(getPos(Palm), 15.0);
	PalmNormal=PalmNormal*50;
	glColor3f(0.0,0.0,0);
	draw_line(getPos(Palm),getPos(Palm+PalmNormal));
	//glColor4f(0.0,0.0,0,0.1);
	//draw_sphere(SphereCenter,(SphereCenter-getPos(Palm)).norm());
	double ratio= (SphereCenter-getPos(Palm)).norm();
	/*if(ratio<40){
		printf("puño\n");
	} else {
		//std::cout << ratio << std::endl;
	}
	if (ratio>100){
		printf("palma\n");
	}*/
        FingerList fingers = hand.fingers();
        for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
                Finger finger = *fl;
                draw_finger(finger);
        }
}

void draw_hands(){
	glLineWidth(2.5); 
	glPointSize(2);
	if(hands.count()==0){
		glColor3f(1.0,1.0,0);
		draw_sphere(getPos(objl::Vector3(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z)),15.0);
		glBegin(GL_LINES);
			draw_line(getPos(objl::Vector3(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z)),getPos(objl::Vector3(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z)));
			draw_line(getPos(objl::Vector3(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z)),getPos(objl::Vector3(-15.3823-rotate_y,127.651+rotate_x,115.778+rotate_z)));
			draw_line(getPos(objl::Vector3(-15.3823-rotate_y,127.651+rotate_x,115.778+rotate_z)),getPos(objl::Vector3(-1.37188-rotate_y,144.293+rotate_x,95.827+rotate_z)));
			//
			draw_line(getPos(objl::Vector3(-48.4583-rotate_y,121.36+rotate_x,135.954+rotate_z)),getPos(objl::Vector3(-32.3174-rotate_y,137.194+rotate_x,76.411+rotate_z)));
			draw_line(getPos(objl::Vector3(-32.3174-rotate_y,137.194+rotate_x,76.411+rotate_z)),getPos(objl::Vector3(-19.589-rotate_y,140.937+rotate_x,41.664+rotate_z)));
			draw_line(getPos(objl::Vector3(-19.589-rotate_y,140.937+rotate_x,41.664+rotate_z)),getPos(objl::Vector3(-12.9246-rotate_y,138.229+rotate_x,22.0143+rotate_z)));
			//
			draw_line(getPos(objl::Vector3(-58.5077-rotate_y,123.748+rotate_x,133.319+rotate_z)),getPos(objl::Vector3(-51.3423-rotate_y,139.198+rotate_x,75.3696+rotate_z)));
			draw_line(getPos(objl::Vector3(-51.3423-rotate_y,139.198+rotate_x,75.3696+rotate_z)),getPos(objl::Vector3(-53.4176-rotate_y,161.515+rotate_x,40.1717+rotate_z)));
			draw_line(getPos(objl::Vector3(-53.4176-rotate_y,161.515+rotate_x,40.1717+rotate_z)),getPos(objl::Vector3(-54.0972-rotate_y,171.526+rotate_x,17.691+rotate_z)));
			//
			draw_line(getPos(objl::Vector3(-68.9242-rotate_y,123.402+rotate_x,131.199+rotate_z)),getPos(objl::Vector3(-70.6707-rotate_y,137.229+rotate_x,78.7907+rotate_z)));
			draw_line(getPos(objl::Vector3(-70.6707-rotate_y,137.229+rotate_x,78.7907+rotate_z)),getPos(objl::Vector3(-78.777-rotate_y,158.395+rotate_x,47.4467+rotate_z)));
			draw_line(getPos(objl::Vector3(-78.777-rotate_y,158.395+rotate_x,47.4467+rotate_z)),getPos(objl::Vector3(-83.4269-rotate_y,169.743+rotate_x,26.837+rotate_z)));
			//
			draw_line(getPos(objl::Vector3(-79.1953-rotate_y,116.794+rotate_x,129.57+rotate_z)),getPos(objl::Vector3(-87.8555-rotate_y,131.213+rotate_x,82.272+rotate_z)));
			draw_line(getPos(objl::Vector3(-87.8555-rotate_y,131.213+rotate_x,82.272+rotate_z)),getPos(objl::Vector3(-103.359-rotate_y,147.81+rotate_x,61.7476+rotate_z)));
			draw_line(getPos(objl::Vector3(-103.359-rotate_y,147.81+rotate_x,61.7476+rotate_z)),getPos(objl::Vector3(-111.675-rotate_y,156.002+rotate_x,49.4818+rotate_z)));
		glEnd();
		objl::Vector3 p1(-30,0,-30), p2(30,0,30);
		objl::Vector3 p3(-30,0,30), p4(30,0,-30);
		glColor3f(0.0,1.0,1.0);
		draw_line(getPos(p1),getPos(p2));
		draw_line(getPos(p3),getPos(p4));
	} else {
		//glScalef(3,3,1.0);//glTranslatef(0,-250,0);
		for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
			Hand hand = *hl;
			//std::cout << std::string(hand.isLeft() ? "Left":"Right") << std::endl;
			draw_hand(hand);
		}
		//glTranslatef(0,250,0);//glScalef(1.0/3,1.0/3,1.0);
	}
}

void getArticulationPoints(std::vector<objl::Vector3>& AP,int handId=2){
        AP.clear();
        for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
                Hand hand = *hl;
		if(handId==2 || (hand.isRight()==handId)){
			FingerList fingers = hand.fingers();
			for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
				Finger finger = *fl;
				for (int b = 3; b >-1; --b) { 
					Bone::Type boneType = static_cast<Bone::Type>(b);
					Bone bone = finger.bone(boneType);
					Vector boneEnd = bone.nextJoint();
					AP.push_back(getPos(objl::Vector3(boneEnd.x,boneEnd.y,boneEnd.z)));
				}   
			}   
		}
        }
	if(!AP.size()){
		AP.push_back(getPos(objl::Vector3(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-15.3823-rotate_y,127.651+rotate_x,115.778+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-48.4583-rotate_y,121.36+rotate_x,135.954+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-32.3174-rotate_y,137.194+rotate_x,76.411+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-19.589-rotate_y,140.937+rotate_x,41.664+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-12.9246-rotate_y,138.229+rotate_x,22.0143+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-58.5077-rotate_y,123.748+rotate_x,133.319+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-51.3423-rotate_y,139.198+rotate_x,75.3696+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-53.4176-rotate_y,161.515+rotate_x,40.1717+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-54.0972-rotate_y,171.526+rotate_x,17.691+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-68.9242-rotate_y,123.402+rotate_x,131.199+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-70.6707-rotate_y,137.229+rotate_x,78.7907+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-78.777-rotate_y,158.395+rotate_x,47.4467+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-83.4269-rotate_y,169.743+rotate_x,26.837+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-79.1953-rotate_y,116.794+rotate_x,129.57+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-87.8555-rotate_y,131.213+rotate_x,82.272+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-103.359-rotate_y,147.81+rotate_x,61.7476+rotate_z)));
		AP.push_back(getPos(objl::Vector3(-111.675-rotate_y,156.002+rotate_x,49.4818+rotate_z)));
	}
}

bool isPuno(Hand hand){
	Vector sphereCenter = hand.sphereCenter();
	Vector palmPosition = hand.palmPosition();
	objl::Vector3 Palm = (objl::Vector3(palmPosition.x, palmPosition.y, palmPosition.z));
	objl::Vector3 SphereCenter = objl::Vector3(sphereCenter.x,sphereCenter.y,sphereCenter.z);
	double ratio=(SphereCenter-Palm).norm();
	if(ratio<45)
		return true;
	return false;
}

bool isPalm(Hand hand){
	Vector sphereCenter = hand.sphereCenter();
	Vector palmPosition = hand.palmPosition();
	objl::Vector3 Palm = (objl::Vector3(palmPosition.x, palmPosition.y, palmPosition.z));
	objl::Vector3 SphereCenter = objl::Vector3(sphereCenter.x,sphereCenter.y,sphereCenter.z);
	double ratio=(SphereCenter-Palm).norm();
	if(ratio>90)
		return true;
	return false;
}

bool handIsUp(Hand hand){
	Vector palmPosition = hand.palmPosition();
	Vector palmNormal = hand.palmNormal();
	objl::Vector3 Palm = (objl::Vector3(palmPosition.x, palmPosition.y, palmPosition.z));
	objl::Vector3 PalmNormal= objl::Vector3(palmNormal.x,palmNormal.y,palmNormal.z);
	return (PalmNormal.Y-Palm.Y)>0.7;
}

bool handIsDown(Hand hand){
	Vector palmPosition = hand.palmPosition();
	Vector palmNormal = hand.palmNormal();
	objl::Vector3 Palm = (objl::Vector3(palmPosition.x, palmPosition.y, palmPosition.z));
	objl::Vector3 PalmNormal= objl::Vector3(palmNormal.x,palmNormal.y,palmNormal.z);
	return (-PalmNormal.Y+Palm.Y)>0.7;
}

bool handIsLeft(Hand hand){
	Vector palmPosition = hand.palmPosition();
	Vector palmNormal = hand.palmNormal();
	objl::Vector3 Palm = (objl::Vector3(palmPosition.x, palmPosition.y, palmPosition.z));
	objl::Vector3 PalmNormal= objl::Vector3(palmNormal.x,palmNormal.y,palmNormal.z);
	std::cout << PalmNormal.X << std::endl;
	return (PalmNormal.X)>0.7;
}

bool handIsRight(Hand hand){
	Vector palmPosition = hand.palmPosition();
	Vector palmNormal = hand.palmNormal();
	objl::Vector3 Palm = (objl::Vector3(palmPosition.x, palmPosition.y, palmPosition.z));
	objl::Vector3 PalmNormal= objl::Vector3(palmNormal.x,palmNormal.y,palmNormal.z);
	return (-PalmNormal.X)>0.7;
}

void getLeapArticulationPoints(std::vector<objl::Vector3>& AP, int handId=2){
        AP.clear();
        for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
                Hand hand = *hl;
		if(handId==2 || (hand.isRight()==handId)){
			FingerList fingers = hand.fingers();
			for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
				Finger finger = *fl;
				for (int b = 3; b >-1; --b) { 
					Bone::Type boneType = static_cast<Bone::Type>(b);
					Bone bone = finger.bone(boneType);
					Vector boneEnd = bone.nextJoint();
					AP.push_back((objl::Vector3(boneEnd.x,boneEnd.y,boneEnd.z)));
				}   
			}
		}
        }
	if(!AP.size()){
		AP.push_back((objl::Vector3(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z)));
		AP.push_back((objl::Vector3(-15.3823-rotate_y,127.651+rotate_x,115.778+rotate_z)));
		AP.push_back((objl::Vector3(-48.4583-rotate_y,121.36+rotate_x,135.954+rotate_z)));
		AP.push_back((objl::Vector3(-32.3174-rotate_y,137.194+rotate_x,76.411+rotate_z)));
		AP.push_back((objl::Vector3(-19.589-rotate_y,140.937+rotate_x,41.664+rotate_z)));
		AP.push_back((objl::Vector3(-12.9246-rotate_y,138.229+rotate_x,22.0143+rotate_z)));
		AP.push_back((objl::Vector3(-58.5077-rotate_y,123.748+rotate_x,133.319+rotate_z)));
		AP.push_back((objl::Vector3(-51.3423-rotate_y,139.198+rotate_x,75.3696+rotate_z)));
		AP.push_back((objl::Vector3(-53.4176-rotate_y,161.515+rotate_x,40.1717+rotate_z)));
		AP.push_back((objl::Vector3(-54.0972-rotate_y,171.526+rotate_x,17.691+rotate_z)));
		AP.push_back((objl::Vector3(-68.9242-rotate_y,123.402+rotate_x,131.199+rotate_z)));
		AP.push_back((objl::Vector3(-70.6707-rotate_y,137.229+rotate_x,78.7907+rotate_z)));
		AP.push_back((objl::Vector3(-78.777-rotate_y,158.395+rotate_x,47.4467+rotate_z)));
		AP.push_back((objl::Vector3(-83.4269-rotate_y,169.743+rotate_x,26.837+rotate_z)));
		AP.push_back((objl::Vector3(-79.1953-rotate_y,116.794+rotate_x,129.57+rotate_z)));
		AP.push_back((objl::Vector3(-87.8555-rotate_y,131.213+rotate_x,82.272+rotate_z)));
		AP.push_back((objl::Vector3(-103.359-rotate_y,147.81+rotate_x,61.7476+rotate_z)));
		AP.push_back((objl::Vector3(-111.675-rotate_y,156.002+rotate_x,49.4818+rotate_z)));
	}
}

void Cube::draw(int cara=0){
	objl::Vector3 vertex;
	if(cara==0){
        glBegin(GL_POLYGON);
		vertex=getPos(trans_x,trans_y,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
		vertex=getPos(trans_x+esc,trans_y,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
		vertex=getPos(trans_x+esc,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
		vertex=getPos(trans_x,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
        glEnd();
	}
	if(cara==1){
        glBegin(GL_POLYGON);
                vertex=getPos(trans_x,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=getPos(trans_x+esc,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=getPos(trans_x+esc,trans_y+esc,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=getPos(trans_x,trans_y+esc,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
        glEnd();
	}
	if(cara==2){
        glBegin(GL_POLYGON);
                vertex=getPos(trans_x,trans_y,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=getPos(trans_x,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=getPos(trans_x,trans_y+esc,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=getPos(trans_x,trans_y+esc,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
        glEnd();
	}
	if(cara==3){
        glBegin(GL_POLYGON);
                vertex=getPos(trans_x+esc,trans_y,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=getPos(trans_x+esc,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=getPos(trans_x+esc,trans_y+esc,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=getPos(trans_x+esc,trans_y+esc,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
        glEnd();
	}
}

void Cube::draw2(int cara=0){
	objl::Vector3 vertex;
	if(cara==0){
        glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		vertex=objl::Vector3(trans_x,trans_y,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
		vertex=objl::Vector3(trans_x+esc,trans_y,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
		vertex=objl::Vector3(trans_x+esc,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
		vertex=objl::Vector3(trans_x,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
        glEnd();
	}
	if(cara==1){
        glBegin(GL_QUADS);
                vertex=objl::Vector3(trans_x,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=objl::Vector3(trans_x+esc,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=objl::Vector3(trans_x+esc,trans_y+esc,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=objl::Vector3(trans_x,trans_y+esc,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
        glEnd();
	}
	if(cara==2){
        glBegin(GL_QUADS);
                vertex=objl::Vector3(trans_x,trans_y,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=objl::Vector3(trans_x,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=objl::Vector3(trans_x,trans_y+esc,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=objl::Vector3(trans_x,trans_y+esc,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
        glEnd();
	}
	if(cara==3){
        glBegin(GL_QUADS);
                vertex=objl::Vector3(trans_x+esc,trans_y,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=objl::Vector3(trans_x+esc,trans_y,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=objl::Vector3(trans_x+esc,trans_y+esc,trans_z-esc);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
                vertex=objl::Vector3(trans_x+esc,trans_y+esc,trans_z);
		glVertex3f(vertex.X,vertex.Y,vertex.Z);
        glEnd();
	}
}

void ListCube::draw(){
        for (int i=0;i<cubes.size();i++)
                cubes[i].draw();
}
