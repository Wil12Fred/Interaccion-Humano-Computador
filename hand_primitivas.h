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
                glBegin(GL_LINES);
                        glVertex3f(boneEndPast.x, boneEndPast.y, boneEndPast.z);
                        glVertex3f(boneEnd.x, boneEnd.y,boneEnd.z);
                glEnd();
                glColor3f(0.0, 1.0, 0.0);
                glBegin(GL_POINTS);
                        glVertex3f(boneEnd.x,
                                        boneEnd.y,
                                        boneEnd.z);
                glEnd();
                boneEndPast=boneEnd;
        }   
        //std::cout << std::endl;
}

void draw_hand(Hand& hand){
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
		glBegin(GL_LINES);
		glColor3f(1.0,1.0,0);
			glVertex3f(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z);glVertex3f(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z);
			glVertex3f(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z);glVertex3f(-15.3823-rotate_y,127.651+rotate_x,115.778+rotate_z);
			glVertex3f(-15.3823-rotate_y,127.651+rotate_x,115.778+rotate_z);glVertex3f(-1.37188-rotate_y,144.293+rotate_x,95.827+rotate_z);

			glVertex3f(-48.4583-rotate_y,121.36+rotate_x,135.954+rotate_z);glVertex3f(-32.3174-rotate_y,137.194+rotate_x,76.411+rotate_z);
			glVertex3f(-32.3174-rotate_y,137.194+rotate_x,76.411+rotate_z);glVertex3f(-19.589-rotate_y,140.937+rotate_x,41.664+rotate_z);
			glVertex3f(-19.589-rotate_y,140.937+rotate_x,41.664+rotate_z);glVertex3f(-12.9246-rotate_y,138.229+rotate_x,22.0143+rotate_z);

			glVertex3f(-58.5077-rotate_y,123.748+rotate_x,133.319+rotate_z);glVertex3f(-51.3423-rotate_y,139.198+rotate_x,75.3696+rotate_z);
			glVertex3f(-51.3423-rotate_y,139.198+rotate_x,75.3696+rotate_z);glVertex3f(-53.4176-rotate_y,161.515+rotate_x,40.1717+rotate_z);
			glVertex3f(-53.4176-rotate_y,161.515+rotate_x,40.1717+rotate_z);glVertex3f(-54.0972-rotate_y,171.526+rotate_x,17.691+rotate_z);

			glVertex3f(-68.9242-rotate_y,123.402+rotate_x,131.199+rotate_z);glVertex3f(-70.6707-rotate_y,137.229+rotate_x,78.7907+rotate_z);
			glVertex3f(-70.6707-rotate_y,137.229+rotate_x,78.7907+rotate_z);glVertex3f(-78.777-rotate_y,158.395+rotate_x,47.4467+rotate_z);
			glVertex3f(-78.777-rotate_y,158.395+rotate_x,47.4467+rotate_z);glVertex3f(-83.4269-rotate_y,169.743+rotate_x,26.837+rotate_z);

			glVertex3f(-79.1953-rotate_y,116.794+rotate_x,129.57+rotate_z);glVertex3f(-87.8555-rotate_y,131.213+rotate_x,82.272+rotate_z);
			glVertex3f(-87.8555-rotate_y,131.213+rotate_x,82.272+rotate_z);glVertex3f(-103.359-rotate_y,147.81+rotate_x,61.7476+rotate_z);
			glVertex3f(-103.359-rotate_y,147.81+rotate_x,61.7476+rotate_z);glVertex3f(-111.675-rotate_y,156.002+rotate_x,49.4818+rotate_z);

		glEnd();
		glBegin(GL_LINES);
		glColor3f(0.0,1.0,1.0);
			glVertex3f(-30,0,-30);glVertex3f(30,0,30);
			glVertex3f(-30,0,30);glVertex3f(30,0,-30);
		glEnd();
	} else {
		//glScalef(3,3,1.0);
		//glTranslatef(0,-250,0);
		for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
			Hand hand = *hl;
			//std::cout << std::string(hand.isLeft() ? "Left":"Right") << std::endl;
			draw_hand(hand);
		}
		//glTranslatef(0,250,0);
		//glScalef(1.0/3,1.0/3,1.0);
	}
}

void getArticulationPoints(std::vector<objl::Vector3>& AP){
        AP.clear();
        for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
                Hand hand = *hl;
                FingerList fingers = hand.fingers();
                for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
                        Finger finger = *fl;
                        for (int b = 3; b >-1; --b) { 
                                Bone::Type boneType = static_cast<Bone::Type>(b);
                                Bone bone = finger.bone(boneType);
                                Vector boneEnd = bone.nextJoint();
                                AP.push_back(objl::Vector3(boneEnd.x,boneEnd.y,boneEnd.z));
                        }   
                }   
        }
	if(!AP.size()){
		AP.push_back(objl::Vector3(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z));AP.push_back(objl::Vector3(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z));
		AP.push_back(objl::Vector3(-41.913-rotate_y,102.057+rotate_x,138.329+rotate_z));AP.push_back(objl::Vector3(-15.3823-rotate_y,127.651+rotate_x,115.778+rotate_z));
		AP.push_back(objl::Vector3(-15.3823-rotate_y,127.651+rotate_x,115.778+rotate_z));AP.push_back(objl::Vector3(-1.37188-rotate_y,144.293+rotate_x,95.827+rotate_z));
		AP.push_back(objl::Vector3(-48.4583-rotate_y,121.36+rotate_x,135.954+rotate_z));AP.push_back(objl::Vector3(-32.3174-rotate_y,137.194+rotate_x,76.411+rotate_z));
		AP.push_back(objl::Vector3(-32.3174-rotate_y,137.194+rotate_x,76.411+rotate_z));AP.push_back(objl::Vector3(-19.589-rotate_y,140.937+rotate_x,41.664+rotate_z));
		AP.push_back(objl::Vector3(-19.589-rotate_y,140.937+rotate_x,41.664+rotate_z));AP.push_back(objl::Vector3(-12.9246-rotate_y,138.229+rotate_x,22.0143+rotate_z));
		AP.push_back(objl::Vector3(-58.5077-rotate_y,123.748+rotate_x,133.319+rotate_z));AP.push_back(objl::Vector3(-51.3423-rotate_y,139.198+rotate_x,75.3696+rotate_z));
		AP.push_back(objl::Vector3(-51.3423-rotate_y,139.198+rotate_x,75.3696+rotate_z));AP.push_back(objl::Vector3(-53.4176-rotate_y,161.515+rotate_x,40.1717+rotate_z));
		AP.push_back(objl::Vector3(-53.4176-rotate_y,161.515+rotate_x,40.1717+rotate_z));AP.push_back(objl::Vector3(-54.0972-rotate_y,171.526+rotate_x,17.691+rotate_z));
		AP.push_back(objl::Vector3(-68.9242-rotate_y,123.402+rotate_x,131.199+rotate_z));AP.push_back(objl::Vector3(-70.6707-rotate_y,137.229+rotate_x,78.7907+rotate_z));
		AP.push_back(objl::Vector3(-70.6707-rotate_y,137.229+rotate_x,78.7907+rotate_z));AP.push_back(objl::Vector3(-78.777-rotate_y,158.395+rotate_x,47.4467+rotate_z));
		AP.push_back(objl::Vector3(-78.777-rotate_y,158.395+rotate_x,47.4467+rotate_z));AP.push_back(objl::Vector3(-83.4269-rotate_y,169.743+rotate_x,26.837+rotate_z));
		AP.push_back(objl::Vector3(-79.1953-rotate_y,116.794+rotate_x,129.57+rotate_z));AP.push_back(objl::Vector3(-87.8555-rotate_y,131.213+rotate_x,82.272+rotate_z));
		AP.push_back(objl::Vector3(-87.8555-rotate_y,131.213+rotate_x,82.272+rotate_z));AP.push_back(objl::Vector3(-103.359-rotate_y,147.81+rotate_x,61.7476+rotate_z));
		AP.push_back(objl::Vector3(-103.359-rotate_y,147.81+rotate_x,61.7476+rotate_z));AP.push_back(objl::Vector3(-111.675-rotate_y,156.002+rotate_x,49.4818+rotate_z));
	}
}
