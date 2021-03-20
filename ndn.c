/* Comment Header */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>

#define IP "139.136.138.142"
#define PORT "59000"

int check_ip(char *full_ip);
int get_cmd(char *cmd);

int main(int argc, char **argv){

	/* Common Variables */
	int i, errcode;
	char argvector[5][8] = {"Name", "IP", "TCP", "regIP", "regUDP"}; // Desired Arguments Description

	/* User Interface Variables */
	char user_str[20], cmd[64], net[64], node_id[64];
	int cmd_code;

	/* UDP Server Variables */
	struct addrinfo hints, *res;
	struct sockaddr addr;
	int fd;
	ssize_t n;
	socklen_t addrlen;
	char buffer[128+1];

	/* TCP Server Variables */	

	/* Argument Process */	
	printf("Number of arguments: %d\n", argc);
	for(i=0; i<argc; i++){
		printf("%s: %s\n", argvector[i], argv[i]);
	}
	/*
	if(argc < 3){
		printf("Invalid number of arguments. Very few arguments inserted.\nThe ideal executable command is: 'ndn IP TCP regIP regUDP'\n");
		exit(1);
	}
	else if(argc > 5){
		printf("Invalid number of arguments. Too many arguments inserted.\nThe ideal executable command is: 'ndn IP TCP regIP regUDP'\n ");
	}
	else if(argc == 3){
		printf("Valid number of arguments. Some arguments might have been ommited and some values will be set by deafault.\n");
	}
	else if(argc == 4){
		printf("Valid number of arguments. Some arguments might have been ommited and some values will be set by deafault.\n");
	}
	else if(argc == 5){
		printf("Valid number of arguments.\n");
		printf("Arguments are valid.\n");
	}
	*/

	/* UDP Node Server Connection Setup */

	fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
	if(fd == -1) exit(1);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_DGRAM; // UDP socket

	errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", "59000", &hints, &res);
	if(errcode != 0) exit(1);
	
	/* User Interface */

	while(1){
		printf(">>>");
		if(fgets(user_str, 20, stdin)!= NULL){
			errcode = sscanf(user_str, "%s", cmd);
			//printf("%s\n", cmd);
			if(errcode != 1) continue;
			cmd_code = get_cmd(cmd);
		}

   		switch(cmd_code){
   			case 1:

   				errcode = sscanf(user_str, "%s ", cmd);

   				n = sendto(fd, "REG 19 19.19.19.19 19", strlen("REG 19 19.19.19.19 19"), 0, res->ai_addr, res->ai_addrlen);
				if(n == -1) exit(1);
				
				addrlen = sizeof(addr);
				n = recvfrom(fd, buffer, strlen(buffer)-1, 0, &addr, &addrlen);
				if(n == -1) exit(1);
				buffer[n] = '\0';
				printf("%s\n", buffer);

   				break;

   			case 2:

   				break;

   			case 3:

   				break;

   			default:
   				printf("Invalid command.\n");
   				break;
   		}

		break;
	}

	/* UDP Node Server Connection Close */

	freeaddrinfo(res);
	close(fd);
	exit(0);
}

int check_ip(char *full_ip){

	return 0;
}

int get_cmd(char *cmd){
	if(strcmp(cmd, "join") == 0) return 1;
	if(strcmp(cmd, "leave") == 0) return 2;
	if(strcmp(cmd, "exit") == 0) return 3;
	else return 0;
}