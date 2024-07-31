#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <string.h>
#define SERVER_IP "127.0.0.1"


void sendRequest(int sock_des, int initial_velocity, int launch_angle) 
{
	char str_request[1024];
	snprintf(str_request, sizeof(str_request), "%d %d", initial_velocity, launch_angle); 

	if (send(sock_des, str_request, strlen(str_request), 0) < 0) 
	{
		perror("Send failed\n");
		exit(EXIT_FAILURE);
	}
	
	char str_data[1024] = {0};
	int read_size;
	while ((read_size = read(sock_des, str_data, sizeof(str_data) - 1)) > 0)
	{
		printf("%s", str_data);
	}

	if (read_size < 0) 
	{
		perror("Read failed\n");
		exit(EXIT_FAILURE);
	}
}

int main() 
{
	int sock_des;
	struct sockaddr_in serv_addr;

	if ((sock_des = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("Socket was not created\n");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8080);

	if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
		perror("Invalid address");
		exit(EXIT_FAILURE);
	}

	if (connect(sock_des, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("Connection failed\n");
		close(sock_des);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Client connected\n");
	}

	int initial_velocity;
	int launch_angle;
	printf("Enter values for initial velocity and launch angle:\n");
	scanf("%d %d", &initial_velocity, &launch_angle);

	sendRequest(sock_des, initial_velocity, launch_angle);

	close(sock_des);

	return 0;
}
