#define eps 1e-9
#define pi acos(-1)

void draw_pixel(objl::Vector3 v){
	glBegin(GL_POINTS);
	glVertex3f(v.X, v.Y, v.Z);
	glEnd();
}

void draw_line(objl::Vector3 v1, objl::Vector3 v2, double width=2.5) {
	glLineWidth(width); 
	glBegin(GL_LINES);
		glVertex3f(v1.X, v1.Y, v1.Z);
		glVertex3f(v2.X, v2.Y, v2.Z);
	glEnd();
}

void draw_triangle(objl::Vector3 v1, objl::Vector3 v2, objl::Vector3 v3){
	glBegin(GL_TRIANGLES);
		glVertex3f(v1.X, v1.Y, v1.Z);
		glVertex3f(v2.X, v2.Y, v2.Z);
		glVertex3f(v3.X, v3.Y, v3.Z);
	glEnd();
}

void draw_sphere(objl::Vector3 Center, double size){
	glTranslatef(Center.X,Center.Y,Center.Z);
	glPushMatrix();
		glutSolidSphere(size,28,28);
	glPopMatrix();
	glTranslatef(-Center.X,-Center.Y,-Center.Z);
}

bool loadTexture(std::string texture_file){
	if(texture_file==""){
		return false;
	}
	int *width=new int;
	int *height=new int;
	GLuint texture=png_texture_load(texture_file.c_str(),width,height);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture);
	return true;
}

void draw_pixel(objl::Vertex v){
	draw_pixel(v.Position);
}

void draw_line(objl::Vertex v1, objl::Vertex v2, double width=2.5) {
	draw_line(v1.Position, v2.Position, width);
}

void draw_triangle(objl::Vertex v1, objl::Vertex v2, objl::Vertex v3, bool texture=false){
	if(!texture){
		glBegin(GL_TRIANGLES);
			glNormal3f(v1.Normal.X,v1.Normal.Y,v1.Normal.Z);
			glVertex3f(v1.Position.X, v1.Position.Y, v1.Position.Z);
			glNormal3f(v2.Normal.X,v2.Normal.Y,v2.Normal.Z);
			glVertex3f(v2.Position.X, v2.Position.Y, v2.Position.Z);
			glNormal3f(v3.Normal.X,v3.Normal.Y,v3.Normal.Z);
			glVertex3f(v3.Position.X, v3.Position.Y, v3.Position.Z);
		glEnd();
		return;
	}
	glBegin(GL_TRIANGLES);
		glNormal3f(v1.Normal.X,v1.Normal.Y,v1.Normal.Z);
		glTexCoord2f(v1.TextureCoordinate.X, v1.TextureCoordinate.Y);
		glVertex3f(v1.Position.X, v1.Position.Y, v1.Position.Z);
		glNormal3f(v2.Normal.X,v2.Normal.Y,v2.Normal.Z);
		glTexCoord2f(v2.TextureCoordinate.X, v2.TextureCoordinate.Y);
		glVertex3f(v2.Position.X, v2.Position.Y, v2.Position.Z);
		glNormal3f(v3.Normal.X,v3.Normal.Y,v3.Normal.Z);
		glTexCoord2f(v3.TextureCoordinate.X, v3.TextureCoordinate.Y);
		glVertex3f(v3.Position.X, v3.Position.Y, v3.Position.Z);
	glEnd();
}

void draw_mesh(objl::Mesh* curMesh, bool withTexture=true){
	bool existTexture=false;
	if(withTexture){
		if(loadTexture(curMesh->MeshMaterial.map_Kd)){
			glColor4f(1.0,1.0,1.0,1.0);
			existTexture=true;
			//glDisable(GL_BLEND);//glDepthMask(GL_TRUE);
		}
	}
        for (int j=0;j<curMesh->Indices.size();j+=3){
		draw_triangle(curMesh->Vertices[curMesh->Indices[j]],
				curMesh->Vertices[curMesh->Indices[j+1]],
				curMesh->Vertices[curMesh->Indices[j+2]],
				existTexture);
        }
	if(existTexture){
			glDisable(GL_TEXTURE_2D);
			//glDepthMask(GL_FALSE);//glEnable(GL_BLEND);//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void draw_loadedMesh(objl::Loader& Loader,int i){
        objl::Mesh* curMesh = &Loader.LoadedMeshes[i];
	bool existTexture=false;
	if(loadTexture(curMesh->MeshMaterial.map_Kd)){
		glColor4f(1.0,1.0,1.0,1.0);
		existTexture=true;
		//glDisable(GL_BLEND);//glDepthMask(GL_TRUE);
	}
        for (int j=0;j<curMesh->Indices.size();j+=3){
		draw_triangle(curMesh->Vertices[curMesh->Indices[j]],
				curMesh->Vertices[curMesh->Indices[j+1]],
				curMesh->Vertices[curMesh->Indices[j+2]],
				existTexture);
        }
	if(existTexture){
			glDisable(GL_TEXTURE_2D);
			//glDepthMask(GL_FALSE);//glEnable(GL_BLEND);//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}
