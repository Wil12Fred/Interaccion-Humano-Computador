  /* Server code in C */
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <map>
#include <chrono>
#include <errno.h>
//#include <ncurses.h>
#include "utilidad.h"
//#include "juego.h"

/*VARIABLES Y FUNCIONES*/

struct sockaddr_in stSockAddr;
int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP),n;
//std::map<std::string,int> clients;
//std::map<std::string, int>::iterator it;
//game Game;

void acceptClient(int ConnectFD);
bool write2(int ConnectFD,std::string prnt, std::string act,std::string remitente="?"); 
void read2(int ConnectFD);

/***********************/

/*bool find_nick(std::string st){ //find  a  nickname is equal to st

	for (it = clients.begin(); it != clients.end(); it++)
		if(it->first == st){
			return true;
		}
	return false;
}

void find_str(int id,std::string & st){//return nickanme found their number socket  

	for (it = clients.begin(); it != clients.end(); it++)
		if(it->second == id){
			st=it->first;
		}
}

std::string protocoloGame(std::string affect){
	return fillZeros(affect.size(),4)+
		"G"+
		affect+
		fillZeros(Game.players[affect].startx,4)+
		fillZeros(Game.players[affect].starty,4)+
		fillZeros(Game.players[affect].lives,2);
}*/

bool existColaborator=0;
int colaborator=0;
int killer=0;
int cantTopos=0;
int cantBlock=0;
std::vector<bool> Topos;
std::vector<bool> BTopos;
std::vector<std::thread> blockTopos;

void block(){
	cantBlock=0;
	while(cantBlock<cantTopos){
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		int bl = rand()%3;
		if(bl<1){
			int eleg=(rand()%(cantTopos-cantBlock))+1;
			int aux=0;
			for (int i=0;i<BTopos.size();i++){
				if(!BTopos[i]){
					aux++;
				}
				if(aux==eleg){
					BTopos[i]=true;
					cantBlock++;
					std::string mns="00080001"+std::to_string(i);
					if(colaborator){
						write(colaborator, mns.c_str(), 9);
					}
					if(killer){
						write(killer, mns.c_str(), 9);
					}
					break;
				}
			}
		} else {
			//bool kill=rand()%2;
			//if(kill){
				/*for (int i=0;i<BTopos.size();i++){
					if(!BTopos[i] && !Topos[i]){
						Topos[i]=!Topos[i];
						std::string stopo=std::to_string(i);
						if(stopo.size()==1){
							stopo="0"+stopo;
						}
						stopo="0005"+stopo;
						if(killer){
								write(killer, stopo.c_str(), 6);
						}
						break;
					}
				}*/
			//}
		}
	}
	int punt=0;
	for (int i=0;i<Topos.size();i++){
		if(Topos[i]){
			punt++;
		}
	}
	if(punt>(cantTopos-punt)){
		if(colaborator){
			write(colaborator, "0006",4);
			existColaborator=false;
			colaborator=0;
		}
		if(killer){
			write(killer, "0007",4);
			killer=0;
		}
	} else {
		if(killer){
			write(killer, "0006",4);
			killer=0;
		}
		if(colaborator){
			write(colaborator, "0007",4);
			existColaborator=false;
			colaborator=0;
		}
	}
	//cantTopos=0;
	//existColaborator=0;
	//colaborator=0;
	//killer=0;
}

void read2(int ConnectFD){
	char buffer[250];
	int n;
	bool login=false;
	//
	for (;;){
		bzero(buffer, 5);
		do{
			n = read(ConnectFD, buffer, 4);
			if(n==0){
				/*if(login){
					std::string username = "";
					find_str(ConnectFD,username);
					if(Game.players.find(username)!=Game.players.end()){
						Game.players[username].lives=0;
						std::string msg=protocoloGame(username);
						for (auto it=clients.begin();it!=clients.end();it++){
							if(it->second!=ConnectFD){
								int nwrite=write(it->second,msg.c_str(),msg.size());
							}
						}
						Game.players.erase(Game.players.find(username));
					}
					clients.erase(clients.find(username));
				}*/
				if(ConnectFD==colaborator){
					colaborator=0;
					existColaborator=0;
				}
				close(ConnectFD);
				return;
			}
			std::string sendFile(buffer);
			if(sendFile=="0001"){
				cantTopos=9;
				BTopos.resize(9);
				for (int i=0;i<9;i++){
					BTopos[i]=false;
				}
				if(!existColaborator){
					write(ConnectFD, "00010",5);
					Topos.clear();
					Topos.resize(cantTopos);
					colaborator=ConnectFD;
					existColaborator=1;
				} else if(ConnectFD!=colaborator){
					killer=ConnectFD;
					write(ConnectFD, "00011",5);
					blockTopos.push_back(std::thread(block));
				} else {
					write(ConnectFD, "00010",5);
					//Topos.clear();
					//Topos.resize(cantTopos);
					//colaborator=ConnectFD;
					//existColaborator=1;
				}
				/*int x=rand()%2;
				std::string tipo="000";
				tipo.push_back(x+'0');
				write(ConnectFD, tipo.c_str(),4);*/
			} else if (sendFile=="0002"){
				std::string cant="0002"+fillZeros(std::to_string(cantTopos).size(),4)+std::to_string(cantTopos);
				write(ConnectFD, cant.c_str(), cant.size());
			} else if (sendFile=="0003"){
				std::string mns="0003"+fillZeros(cantTopos,4);
				if(ConnectFD==colaborator){
					for (int i=0;i<cantTopos;i++){
						if(Topos[i]){
							mns.push_back('1');
						} else {
							mns.push_back('0');
						}
					}
				} else {
					for (int i=0;i<cantTopos;i++){
						if(Topos[i]){
							mns.push_back('0');
						} else {
							mns.push_back('1');
						}
					}
				}
				write(ConnectFD, mns.c_str(), mns.size());
			} else if (sendFile=="0004"){
				buffer[5]=0;
				n = read(ConnectFD, buffer, 4);
				int size_mns=stoi(std::string(buffer));
				char mns[size_mns+1];
				mns[size_mns]=0;
				n = read(ConnectFD, mns, size_mns);
				int topo=atoi(mns);
				Topos[topo]=!Topos[topo];
				std::string stopo=std::to_string(topo);
				if(stopo.size()==1){
					stopo="0"+stopo;
				}
				if(ConnectFD==colaborator){
					if(killer && BTopos[topo]==false){
						write(killer,std::string("0005"+stopo).c_str(),6);
					} else {
						write(ConnectFD,std::string("0005"+stopo).c_str(),6);
						Topos[topo]=!Topos[topo];
					}
				} else {
					if(existColaborator && BTopos[topo]==false){
						write(colaborator,std::string("0005"+stopo).c_str(),6);
					} else {
						write(ConnectFD,std::string("0005"+stopo).c_str(),6);
						Topos[topo]=!Topos[topo];
					}
				}
			} else if(sendFile=="0005"){//SALIR
				if(ConnectFD==colaborator){
					existColaborator=false;
					colaborator=0;
					if(killer){
						write(killer,"0007",4);
					}
				} else if (ConnectFD==killer){
					killer=0;
					if(existColaborator){
						write(colaborator,"0007",4);
					}
				}
				cantTopos=0;
			}
			/*int size_txt=atoi(buffer);
			bzero(buffer, 4);
			//
			n = read(ConnectFD, buffer, 1);
			std::string action(buffer);
			bzero(buffer, 1);
			//
			if (action == "P"){ //Protocolo for Printing
				std::string prnt="";
				for (it = clients.begin(); it != clients.end(); it++){
					prnt+="(username "+it->first
					+" : " + std::to_string(it->second) + ") ";
				}
				write2(ConnectFD,prnt,action); 
				if(!login){
					prnt="no estas logueado";
					write2(ConnectFD,prnt,action); 
				}
				//
			} else if (action == "L"){//protocolo for Login
				char newNickName[size_txt];	
				newNickName[size_txt]=0;
				n = read(ConnectFD, newNickName, size_txt);
				if(find_nick(std::string(newNickName)) == true){ // find  a new nickname is equal to other already exists
					std::string err="nickname already exists, enter other";
					write2(ConnectFD,err,action);
					continue;
				}
				if(!login){
					login=true;
					//
					clients[newNickName] = ConnectFD; //adding a newclient
					std::cout << "Login: " << newNickName << std::endl;
					for (Game.it = Game.players.begin(); Game.it != Game.players.end(); Game.it++){
						std::string msg=protocoloGame(Game.it->first);
						int nwrite= write2(ConnectFD, msg.c_str(),"G");
					}
				} else {
					std::string username="";
					find_str(ConnectFD,username);
					std::string err="Error: You are logued with "+username;
					write2(ConnectFD,err,action);
					continue;
				}
				//
			} else if (action == "C"){ //protocolo for chating
				std::string username = "";
				find_str(ConnectFD,username); //username has nickname who send to mssg 
				//
				n = read(ConnectFD, buffer, 2); //reading a size of the other client
				int size_othername=atoi(buffer);
				bzero(buffer, 2);
				//
				char oname[size_othername+1];
				oname[size_othername]=0;
				n = read(ConnectFD, oname, size_othername); //reading a nickname the other client
				std::string othername(oname);
				int size_msg= size_txt;// size has the size the real mssg
				char bufferMsg[size_msg+1];
				bufferMsg[size_msg]=0;
				n = read(ConnectFD, bufferMsg, size_msg);
				std::string msg(bufferMsg);
				if(!find_nick(othername)){ //check if othername exists
					std::string err = "nickname not found, enter other\n";
					write2(ConnectFD, err, action);
					continue;
				}	
				//
				int otherConnectFD = clients.find(othername)->second; //finding socket number the other client for send to mssg 
				if (otherConnectFD < 0){
					perror("error in nickname");
				}
				
				msg = username+": "+msg; //msg final
				if(login){
					write2(otherConnectFD, msg, action,othername);
				} else {
					msg="Not login\n";
					write2(ConnectFD, msg, action);
				}
			} else if (action == "G"){ //protocolo for chating
				std::string username = "";
				find_str(ConnectFD,username);
				int size_msg= size_txt;// size has the size the real mssg
				n = read(ConnectFD, buffer, size_msg);
				int msg=atoi(buffer);
				bzero(buffer,size_msg);
				std::string affect=username;
				bool propagate=true;
				if(msg=='N'){
					n = read(ConnectFD, buffer, 4);
					int lines=atoi(buffer);
					bzero(buffer,4);
					n = read(ConnectFD, buffer, 4);
					int cols=atoi(buffer);
					bzero(buffer,4);
					if(login){
						Game.newPlayer(username,lines,cols);
					} else {
						write2(ConnectFD,"Not login (press ESC)","C");
						propagate=false;
					}
				} else if(!Game.move_user2(username,msg,affect)){
						propagate=false;
				}
				if(propagate){
					if(login){
						std::string nmsg=protocoloGame(affect),nmsg2;
						if(msg=='o')
							nmsg2= fillZeros(username.size(),4)+
								"O"+
								username+
								fillZeros(Game.players[username].starty-Game.maxy,4);
						for (it = clients.begin(); it != clients.end(); it++){
							int nwrite= write2(it->second, nmsg.c_str(), "G");
							if(msg=='o'){
								nwrite= write2(it->second, nmsg2.c_str(), "G" ,it->first);
							}
						}
					}
					if(Game.players[affect].lives==0){
						Game.players.erase(Game.players.find(affect));
						std::string alertMessage="You Lose "+affect;
						write2(clients[affect],alertMessage,"C");
						if(username!=affect){
							alertMessage="You Win "+username;
							write2(clients[username],alertMessage,"C");
						}
					}
				}
			} else if (action == "E"){//protocol for End
				std::vector<std::string> V;
				for (auto it=clients.begin();it!=clients.end();it++){
					if(it->second==ConnectFD){
						V.push_back(it->first);
					}
				}
				for (int i=0;i<V.size();i++){
					clients.erase(V[i]);
				}
				close(ConnectFD);
				return;
			} else if (action == "F"){
				sendFile+="D";
				std::string username = "";
				find_str(ConnectFD,username); //username has nickname who send to mssg 
				bzero(buffer,5);

				n = read(ConnectFD, buffer, 2); //reading a size of the other client
				int size_othername=atoi(buffer);
				bzero(buffer, 2);
				char oname[size_othername+1];
				oname[size_othername]=0;
				n = read(ConnectFD, oname, size_othername); //reading a nickname the other client
				std::string othername(oname);
				int size_msg= size_txt;// size has the size the real mssg
				char bufferMsg[size_msg+1];
				bufferMsg[size_msg]=0;
				n = read(ConnectFD, bufferMsg, size_msg);
				std::string msg(bufferMsg);
				n = read(ConnectFD, buffer, 4);
				std::string str_size_file(buffer);
				int size_file=atoi(buffer);
				std::cout << size_file << std::endl;
				bzero(buffer,4);
				char msg_file[size_file+1];
				msg_file[size_file]=0;
				n=read(ConnectFD,msg_file,size_file);
				std::cout << "File: " << msg_file << std::endl;
				if(!find_nick(othername)){ //check if othername exists
					std::string err = "nickname not found, enter other";
					write2(ConnectFD, err, "C");
					continue;
				}
				sendFile+=fillZeros(username.size(),2)+username+msg+str_size_file;
				int otherConnectFD = clients.find(othername)->second; //finding socket number the other client for send to mssg 
				if(login){
					write2(otherConnectFD, sendFile+std::string(msg_file), "D");
				} else {
					std::cout << action << std::endl;
					sendFile="Error: You arent login\n";
					write2(ConnectFD, sendFile, "C");
				}

			} else {// this is can be better, you can do it =)
				std::cout << "error in action, msg bad\n";
			}*/
		} while (n == 0);
	}
}

bool write2(int ConnectFD, std::string mssg, std::string act, std::string remitente){

	if (act == "P" or act == "C" or act == "L") { // L is when a nickname is repeat 
		mssg = fillZeros(mssg.size(),4)+"R" +mssg;
		std::cout << mssg << " -> " << remitente << "\n";
		int nwrite= write(ConnectFD, mssg.c_str(), mssg.size());
		return true;
	} else if (act=="D" || act=="G"){
		std::cout << mssg << " -> " << remitente << "\n";
		int nwrite= write(ConnectFD, mssg.c_str(), mssg.size());
		return true;
	}
	return false;
}

void acceptClient(int ConnectFD) {
	if (0 > ConnectFD) {
		perror("error accept failed");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
	//
	write2(ConnectFD,"Welcome","C");
	std::thread(read2, ConnectFD).detach();
	std::this_thread::sleep_for(std::chrono::seconds(100));
}

bool startConnection(int port){
	stSockAddr.sin_port = htons(port);
	if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in))){
		/*perror("error bind failed");close(SocketFD);exit(EXIT_FAILURE);*/
		return false;
	}
	if(-1 == listen(SocketFD, 10)){
		/*perror("error listen failed");close(SocketFD);exit(EXIT_FAILURE);*/
		return false;
	}
	std::cout << "Port " << port << ": " << "connection success" << std::endl;
	return true;
}

int main(void){
	srand(time(NULL));
	//
	if(-1 == SocketFD){
		perror("can not create socket");
		exit(EXIT_FAILURE);
	}
	//
	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
	//
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_addr.s_addr = INADDR_ANY;
	for (int i=1200;startConnection(i)==false;i++){
		std::cout << "Port " << i << ": " << "bind or listen error" << std::endl;
	}
	//
	while(true){
		int ConnectFD = accept(SocketFD, NULL, NULL);
		std::cout << "ConnectFD: " << ConnectFD << std::endl;
		std::thread(acceptClient,ConnectFD).detach();
	}
	//
	shutdown(SocketFD, SHUT_RDWR);
	close(SocketFD);
	return 0;
}
