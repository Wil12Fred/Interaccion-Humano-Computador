void draw_pixel(objl::Vertex v){
	glBegin(GL_POINTS);
	glVertex3f(v.Position.X, v.Position.Y, v.Position.Z);
	glEnd();
}

void draw_line(objl::Vertex v1, objl::Vertex v2, double width=2.5) {
	glLineWidth(width); 
	glBegin(GL_LINES);
		glVertex3f(v1.Position.X, v1.Position.Y, v1.Position.Z);
		glVertex3f(v2.Position.X, v2.Position.Y, v2.Position.Z);
	glEnd();
}

void draw_triangle(objl::Vertex v1, objl::Vertex v2, objl::Vertex v3, bool texture=false){
	glBegin(GL_TRIANGLES);
		if(texture)
			glTexCoord2f(v1.TextureCoordinate.X, v1.TextureCoordinate.Y);
		glVertex3f(v1.Position.X, v1.Position.Y, v1.Position.Z);
		if(texture)
			glTexCoord2f(v2.TextureCoordinate.X, v2.TextureCoordinate.Y);
		glVertex3f(v2.Position.X, v2.Position.Y, v2.Position.Z);
		if(texture)
			glTexCoord2f(v3.TextureCoordinate.X, v3.TextureCoordinate.Y);
		glVertex3f(v3.Position.X, v3.Position.Y, v3.Position.Z);
	glEnd();
}

void draw_loadedMesh(objl::Loader& Loader,int i){
        objl::Mesh* curMesh = &Loader.LoadedMeshes[i];
	int *width=new int;
	int *height=new int;
	bool existTexture=false;
	if(curMesh->MeshMaterial.map_Kd!=""){
		existTexture=true;
		GLuint texture=png_texture_load((curMesh->MeshMaterial.map_Kd).c_str(),width,height);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
        for (int j=0;j<curMesh->Indices.size();j+=3){
		draw_triangle(curMesh->Vertices[curMesh->Indices[j]],
				curMesh->Vertices[curMesh->Indices[j+1]],
				curMesh->Vertices[curMesh->Indices[j+2]],existTexture);
        }
	if(existTexture){
			glDisable(GL_TEXTURE_2D);
	}
}

void Cube::draw(){
        glBegin(GL_POLYGON);
                glVertex3f(trans_x,trans_y,trans_z);
                glVertex3f(trans_x+esc,trans_y,trans_z);
                glVertex3f(trans_x+esc,trans_y,trans_z-esc);
                glVertex3f(trans_x,trans_y,trans_z-esc);
        glEnd();
        glBegin(GL_POLYGON);
                glVertex3f(trans_x,trans_y,trans_z-esc);
                glVertex3f(trans_x+esc,trans_y,trans_z-esc);
                glVertex3f(trans_x+esc,trans_y+esc,trans_z-esc);
                glVertex3f(trans_x,trans_y+esc,trans_z-esc);
        glEnd();
}

void ListCube::draw(){
        for (int i=0;i<cubes.size();i++)
                cubes[i].draw();
}

ListCube list_cube;
