struct Cube{
	double trans_x;
	double trans_y;
	double trans_z;
	double esc;
	bool intersected;
	Cube(double x, double y, double z,double e){
		trans_x=x;
		trans_y=y;
		trans_z=z;
		esc=e;
		intersected=false;
	}
	bool check_intersection(objl::Vector3& vertex){
		double x=vertex.X,y=vertex.Y,z=vertex.Z;
		if(!(x>trans_x && x<trans_x+esc)){
			return false;
		}
		if(!(y>trans_y && y<trans_y+esc)){
			return false;
		}
		if(!(z<trans_z && z>trans_z-esc)){
			return false;
		}
		return true;
	}
	bool check_intersection(std::vector<objl::Vector3>& vertexs){
		for (int i=0;i<vertexs.size();i++)
			intersected=(intersected || check_intersection(vertexs[i]));
		return intersected;
	}
	void draw(int cara);
	void draw2(int cara);
};

struct ListCube{
	double trans_x;
	double trans_y;
	double trans_z;
	double esc;
	double dif_x;
	double dif_z;
	bool intersection;
	int cant;
	std::vector<Cube> cubes;
	ListCube(){}
	ListCube(double x, double y, double z, double e, double dx, double dz,int n){
		set(x,y,z,e,dx,dz,n);
	}
	void set(double x, double y, double z, double e, double dx, double dz,int n){
		intersection=false;
		trans_x=x;
		trans_y=y;
		trans_z=z;
		esc=e;
		dif_x=dx;
		dif_z=dz;
		cant=n;
		clear();
		for (int i=0;i<cant;i++){
			cubes.push_back(Cube(trans_x+i*dif_x,trans_y,trans_z-dif_z,esc));
		}
		cubes[0].trans_z+=dif_z;
		cubes[cubes.size()-1].trans_z+=dif_z;
	}
	int size(){
		return cubes.size();
	}
	void clear(){
		cubes.clear();
	}
	bool check_intersection(std::vector<objl::Vector3>& vertexs){
		for (int i=0;i<cubes.size();i++){
			intersection= intersection || cubes[i].check_intersection(vertexs);
		}
		return intersection;
	}
	void draw();
};
