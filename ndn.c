/******************************************************************************
 *
 * File Name: ndn.c
 * Autor:  G19 (RCI 20/21) - João Luzio (IST193096) & José Reis (IST193105)
 * Last Review: 22 Mar 2021
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include "ndn.h"

#define DEFAULT_HOST "tejo.tecnico.ulisboa.pt"
#define DEFAULT_IP "193.136.138.142"
#define DEFAULT_PORT "59000"

int main(int argc, char **argv){

	/* Common Variables */
	int i, errcode, endFLAG = 0;
	char nodeIP[20], nodeTCP[20], regIP[20], regUDP[20];
	fd_set current_sockets, ready_sockets;
	enum {unreg, reg, getout} state;
	int maxfd, counter;

	/* User Interface Variables */
	char user_str[64], cmd[64], net[64], nodeID[64], bootIP[64], bootTCP[64];
	int cmd_code, joined = 0;


	/* Node Topology Variables */
	node_info extern_node, backup_node;

	/* UDP Server Variables */
	struct addrinfo hints, *res;
	struct in_addr addr;
	ssize_t n;
	char buffer[128+1];

	/* TCP Server Variables */	
	int fd_TCP;

	/* Argument Process */	
	printf("\n");
	if(argc < 3){
		printf("Invalid number of arguments. Very few arguments inserted.\nTypical usage: 'ndn IP TCP regIP regUDP'.\n");
		exit(1);
	}
	else if(argc > 5){
		printf("Invalid number of arguments. Too many arguments inserted.\nTypical usage: 'ndn IP TCP regIP regUDP'.\n ");
		exit(1);
	}
	else if(argc == 3){
		printf("Valid number of arguments. Some arguments might have been ommited and some values will be set by deafault.\n");
		strcpy(regIP, DEFAULT_IP);
		strcpy(regUDP, DEFAULT_PORT);
	}
	else if(argc == 4){
		printf("Valid number of arguments. Some arguments might have been ommited and some values will be set by deafault.\n");
		strcpy(regIP, argv[3]);
		strcpy(regUDP, DEFAULT_PORT);
	}
	else if(argc == 5){
		printf("Valid number of arguments.\n");
		strcpy(regIP, argv[3]);
		strcpy(regUDP, argv[4]);
	}
	
	strcpy(nodeIP, argv[1]);
	strcpy(nodeTCP, argv[2]);
	if((check_ip(nodeIP) == 0) || (check_ip(regIP) == 0)){
		printf("Error in IP format verification.\n");
		exit(1);
	}
	if((atoi(regUDP) < 0) || (atoi(regUDP) > 65535)){
		printf("Error specifying UDP port.\n");
		exit(1);
	}
	printf("Arguments are valid.\n\n\n\n\n");

	/* TCP Server Connection Setup */

	/* Save UDP socket fd */
	FD_ZERO(&current_sockets);
	// FD_SET(fd_TCP, &current_sockets);
	
	/* User Interface */
	state = unreg;
	printf("Node Interface:\n");
	while(1){
		ready_sockets = current_sockets; // because select is destructive

		printf(">>> ");
		fflush(stdout);
		
		if(fgets(user_str, 64, stdin)!= NULL){
			errcode = sscanf(user_str, "%s", cmd);
			if(errcode != 1) continue;
			cmd_code = get_cmd(cmd); // Get command code
		}

   		switch(cmd_code){
   			case 1:
   				if(joined != 0){ // Already joined?
   					printf("\tNode already joined a net!\n");
   					break;
   				}

   				errcode = sscanf(user_str, "%s %s %s %s %s", cmd, net, nodeID, bootIP, bootTCP);
   				if((errcode != 3) && (errcode != 5)){
   					printf("\tInvalid command syntax. The ideal executable command is: 'join net id bootIP bootTCP'.\n");
   					break;
   				}
   				else{
   					joined = regNODE(1, net, nodeIP, nodeTCP, regIP, regUDP);
   					if(joined == 0) break;
   				}

   				if(errcode == 3){ // Indirect Join

   				}
   				else if(errcode == 5){ // Direct Join

   				}

   				break;

   			case 2:
   				if(joined != 1){ // Not joined yet?
   					printf("\tNode does not have joined any net!\n");
   					break;
   				}

   				joined = regNODE(0, net, nodeIP, nodeTCP, regIP, regUDP);
   				if(joined == 1) break;

   				memset(net, '0', sizeof(net));
   				break;

   			case 3:
   				printf("\tShutting down all connections and closing the node...\n");
   				if(joined == 1){
   					joined = regNODE(0, net, nodeIP, nodeTCP, regIP, regUDP);
   				}
   				endFLAG = 1;
   				if((endFLAG = 1) && (joined == 0)) printf("\tSucess! Node shut down.\n");
   				break;

   			default:
   				printf("\tInvalid or unknown command: %s\n", cmd);
   				break;
   		}
   		if(endFLAG == 1) break;
	}
	exit(0);
}