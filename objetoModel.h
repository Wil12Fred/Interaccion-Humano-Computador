#include "modelMesh.h"

struct Objeto{
	Model* model;
	objl::Vector3 Movimiento;
	std::map<int, objl::Vector3> Colores;
	bool useColor;
	bool intersected;
	bool invisible;
	float maxY;
	Objeto(){
		Movimiento=objl::Vector3(0,0,0);
		useColor=false;
		intersected=false;
		invisible=false;
	}
	Objeto(Model* m,double X,double Y,double Z){
		model=m;
		Movimiento=objl::Vector3(X,Y,Z);
		useColor=false;
		intersected=false;
		invisible=false;
	}
	void setMovimiento ( double X, double Y , double Z){
		Movimiento=objl::Vector3(X,Y,Z);
	}
	bool estaDentro(objl::Vector3 point,float *topY=0){
		float* dist=new float;
		point=point-Movimiento;
		if( model->estaDentro(point,dist)){
			if(topY){
				*topY=model->max.Y-*dist;
			}
			return true;
		}
		return false;
	}
	bool intersecta(std::vector<objl::Vector3> PA){
		intersected=false;
		for (int i=0;i<PA.size();i++){
			float *topY=new float;
			if(estaDentro(PA[i],topY)){
				if(!intersected){
					intersected=true;
					maxY=*topY;
				} else  {
					maxY=std::min(maxY,*topY);
				}
			}
		}
		return intersected;
	}
	void draw(){
		if(!invisible){
			if(!model->isLoaded){
				bool loadout = model->isLoaded = model->formatObj.LoadFile(model->fileObj);
				if(!loadout){
					printf("Error\n");
					return;
				}
			}
			if (useColor){
				model->useColor=true;
				for (auto it=Colores.begin();it!=Colores.end();it++){
					model->formatObj.LoadedMeshes[it->first].defaultColor=it->second;
				}
			}
			glTranslatef(Movimiento.X,Movimiento.Y,Movimiento.Z);
			if(intersected)
				glTranslatef(0,maxY-model->max.Y,0);
			model->draw();
			if(intersected)
				glTranslatef(0,model->max.Y-maxY,0);
			glTranslatef(-Movimiento.X,-Movimiento.Y,-Movimiento.Z);
		}
	}
};
