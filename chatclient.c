#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

/*Function generates the handle for the client and verifies that the length is not too long*/
char* generateHandle(){

	printf("%s","Please enter username: ");
	
	//Allocate space for userName
	char*  myUserName = (char*)malloc(50 * sizeof(char));	

	//Keep asking for user handle until the input is valid
	int looper = 1;
	while(looper ==1){
	
		fgets(myUserName, 20, stdin);
		//trim string because it will be concated with user > 
		myUserName[strlen(myUserName)-1]='\0';
		if(strlen(myUserName) < 11){
			looper = 0;
		}
		else{
		printf("%s", "Please enter a unsername less than 11 characters\n");
		}
	}
	//Add > to end of string
	char indent [] =">";
       	strcat(myUserName,indent);
	return myUserName;
} 

/*Handles receiving messages and prints to console */
void receiveMessage(int numbytes, int sockfd, char* buff, char* myUserName){
	numbytes = recv(sockfd,buff,1000,0);
	
	if(numbytes == 0){
		printf("server closed connection\n");
		free(myUserName);
		myUserName = NULL;
		exit(0);
	}
	buff[numbytes] = '\0';
	printf("%s\n",buff); 
}

/*Handles user input and sends to socket  */
void getInput(char* myUserName, int sockfd){
	
	char myInput [500];
	char tempValue[50];
        char quit[] = "\\quit\0";

	printf(myUserName);
		//Check to make sure length is below 500
		int checkLength = 0;
		while(checkLength == 0){
	
			fgets(myInput,1000, stdin);
			if(strlen(myInput) > 500){
				printf("Error, input too long\n");
				printf(myUserName);
			}else{
				checkLength = 1;
			}
		}
		//append terminator to input
		myInput[strlen(myInput)-1] = '\0';
		//trim \n from string
		strncpy(myInput, myInput, strlen(myInput)-1);
		
		//if user enters \quit end connection and program
		if( strcmp(quit, myInput) == 0 ){
			printf("connection closed\n");
			close(sockfd);
			free(myUserName);
			myUserName=NULL;
			exit(0);
		}
		//copy input to temp variable to have persistent copy of user handle
		strcpy(tempValue,myUserName);
	
		//cat together the username and handle 
		strcat(tempValue,myInput);
		tempValue[strlen(tempValue)+1]='\0';
		send(sockfd, tempValue, strlen(tempValue), 0);	
}


int main(int argc, char* argv[])
{
	int sockfd, numbytes, rv;
	char buf[500];
	struct addrinfo hints, *servinfo, *p;
	
	//insure user enteres host address and port number 
	if (argc != 3) {
        	fprintf(stderr,"Not Enough Arguments\n");
        	exit(1);
    	}

	//Set up hints struct to store address and configuration settings for port per recommended Beej's guide
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if ((rv	= getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0 )
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        	exit(1);
	}

	//Establish socket and connect to server
	p = servinfo;
	sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	connect(sockfd, p->ai_addr, p->ai_addrlen); 

	//generate user Handle	
	char* myUserName  = generateHandle();	
	
	//infinite loop until server or user disconnects
	while(1)
	{
		getInput(myUserName, sockfd);
		receiveMessage(numbytes, sockfd, buf,myUserName);
	}
	free(myUserName);
	myUserName = NULL;
	return 0;
}

