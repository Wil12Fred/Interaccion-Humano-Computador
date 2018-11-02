//#include "ParseObj/obj.h"

struct Model{
	std::set<int> Meshes;
	objl::Loader formatObj;
	objl::Vector3 toCenter;
	bool useColor,isLoaded;
	std::string fileObj;
	double esc;
	objl::Vector3 min,max;
	std::map<int,std::pair<objl::Vector3,objl::Vector3> > Limits;
	bool hasLimits;

	Model(){
		isLoaded=false;
		hasLimits=false;
		esc=1;
		toCenter=objl::Vector3(0,0,0);
	}

	Model(std::string nameObj, double escala=1, objl::Vector3 toCent=objl::Vector3(0,0,0)){
		hasLimits=false;
		fileObj=nameObj;
		if(load()){
			esc=escala;
			toCenter=toCent;
			if(escala!=1){
				for (int i=0;i<formatObj.LoadedMeshes.size();i++){
					objl::Mesh* curMesh=&formatObj.LoadedMeshes[i];
					for (int j=0;j<curMesh->Vertices.size();j++){
						objl::Vertex* curVertex=&(curMesh->Vertices[j]);
						curVertex->Position=(curVertex->Position)+toCenter;
						curVertex->Position=(curVertex->Position)*esc;
					}
				}
			}
		}
	}

	bool load(){
		isLoaded=formatObj.LoadFile(fileObj);
		return isLoaded;
	}

	void addMesh(int i){
		Meshes.insert(i);
		objl::Vector3 max_local,min_local;
		objl::Mesh* curMesh=&formatObj.LoadedMeshes[i];
		int nv=curMesh->Vertices.size();
		if(nv==0){
			return;
		}
		min_local=curMesh->Vertices[0].Position;
		max_local=curMesh->Vertices[0].Position;
		for (int j=1;j<nv;j++){
			objl::Vertex* curVertex=&(curMesh->Vertices[j]);
			min_local=min_local.min(curVertex->Position);
			max_local=max_local.max(curVertex->Position);
		}
		if(!hasLimits){
			hasLimits=true;
			min=min_local;
			max=max_local;
		} else {
			min=min.min(min_local);
			max=max.max(max_local);
		}
	}

	void solid(){
		if(hasLimits) {
			Limits.clear();
			double inc=(max.Y-min.Y)/5;
			for (auto it=Meshes.begin();it!=Meshes.end();it++){
				objl::Mesh* curMesh=&formatObj.LoadedMeshes[*it];
				for (int j=0;j<curMesh->Vertices.size();j++){
					objl::Vector3 curVertex=curMesh->Vertices[j].Position;
					int rel=(curVertex.Y-min.Y)/inc;
					if(Limits.find(rel)==Limits.end()){
						Limits[rel]=std::make_pair(curVertex,curVertex);
					} else {
						std::pair<objl::Vector3,objl::Vector3> curLimit=Limits[rel];
						curLimit.first=curLimit.first.min(curVertex);
						curLimit.second=curLimit.second.max(curVertex);
						Limits[rel]=curLimit;
					}
				}
			}
			for (int i=1;i<=Limits.rbegin()->first;i++){
				if(Limits.find(i)==Limits.end()){
					Limits[i]=Limits[i-1];
				}
			}
		}
	}

	bool estaDentro(objl::Vector3 point, float * dist=0){
		bool intersec=false;
		*dist=0;
		for (auto it=Limits.begin();it!=Limits.end();it++){
			objl::Vector3 curMin=it->second.first;
			objl::Vector3 curMax=it->second.second;
			if(!intersec){
				if(curMin.Y>=point.Y || curMax.Y<=point.Y){
					continue;
				}
				if(curMin.X>=point.X || curMax.X<=point.X){
					continue;
				}
				if(curMin.Z>=point.Z || curMax.Z<=point.Z){
					continue;
				}
				intersec=true;
				if(dist){
					*dist=curMax.Y-point.Y;
				}
			} else {
				if(curMin.X>=point.X || curMax.X<=point.X){
					continue;
				}
				if(curMin.Z>=point.Z || curMax.Z<=point.Z){
					continue;
				}
				*dist=*dist+curMax.Y-curMin.Y;
			}
		}
		return intersec;
	}

	void addColor(int i,double r, double g, double b){
		formatObj.LoadedMeshes[i].defaultColor=objl::Vector3(r,g,b);
	}

	void setToCenter(double X,double Y, double Z){
		toCenter=objl::Vector3(X,Y,Z);
		for (int i=0;i<formatObj.LoadedMeshes.size();i++){
			objl::Mesh* curMesh=&formatObj.LoadedMeshes[i];
			for (int j=0;j<curMesh->Vertices.size();j++){
				objl::Vertex* curVertex=&(curMesh->Vertices[j]);
				curVertex->Position=(curVertex->Position)+toCenter;
			}
		}
	}

	void moveToCenter(){
		if(hasLimits){
			toCenter=objl::Vector3(-(max.X+min.X)/2,
						-min.Y,//(max.Y+min.Y)/2,
						-(max.Z+min.Z)/2);
			for (int i=0;i<formatObj.LoadedMeshes.size();i++){
				objl::Mesh* curMesh=&formatObj.LoadedMeshes[i];
				for (int j=0;j<curMesh->Vertices.size();j++){
					objl::Vertex* curVertex=&(curMesh->Vertices[j]);
					curVertex->Position=(curVertex->Position)+toCenter;
				}
			}
			max=max+toCenter;
			min=min+toCenter;
		}
	}

	void draw(bool withTexture=true){
		if(!isLoaded){
			bool loadout = isLoaded = formatObj.LoadFile(fileObj);
			if(!loadout){
				printf("Error\n");
				return;
			}
		}
		for (auto it=Meshes.begin();it!=Meshes.end();it++){
			if(useColor){
				objl::Vector3 color=formatObj.LoadedMeshes[*it].defaultColor;
				glColor4f(color.X, color.Y, color.Z,0.9);
			}
			draw_mesh(&formatObj.LoadedMeshes[*it],withTexture);
		}
	}
};
