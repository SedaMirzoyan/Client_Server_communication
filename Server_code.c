#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <string.h> 
#include <math.h>
#define STANDARD_GRAVITY 9.81
#define g STANDARD_GRAVITY


int calculateTiming(int V0, int angle)
{
	int t = (2 * V0 * sin(angle))/g;
	return t;
}

int calculateVerticalDistance(int V0, int angle)
{
	int t = calculateTiming(V0, angle);
	int y = V0 * t * sin(angle) - (g * pow(t, 2))/2;

	return y;
}

int calculateHorizontalDistance(int V0, int angle)
{
	int t = calculateTiming(V0, angle);
	int x = V0 * t * cos(angle);

	return x;
}

int calculateMaximumHeight(int V0, int angle)
{
	int H = (pow(V0, 2) * pow(sin(angle), 2))/2 * g;

	return H;
}

int calculateRange(int V0, int angle)
{
	int l = (pow(V0, 2) * sin(2 *angle))/ g;

	return l;
}


int main() 
{ 

	int server_socket;
	int new_socket; 
	struct sockaddr_in server_addr; 
	int server_addr_len = sizeof(server_addr); 
	char str_data[1024];
 
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("Socket creation failed\n"); 
		exit(EXIT_FAILURE); 
	} 
	else
	{
		printf("Socket created\n");
	}

	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080); 

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
	{ 
		perror("Bind failed\n");
	 	close(server_socket); 
		exit(EXIT_FAILURE); 
	} 
	else
	{
		printf("Bind succedded\n");
	}

	if (listen(server_socket, 3) < 0) 
	{
	 	perror("Listen failed"); 
		close(server_socket); 
		exit(EXIT_FAILURE); 
	} 
	else
	{
		printf("Listening\n");
	}

 	if ((new_socket = accept(server_socket, (struct sockaddr *)&server_addr, (socklen_t *)&server_addr_len)) < 0) 
 	{ 
		perror("Accept failed\n"); 
 		close(server_socket);
 		exit(EXIT_FAILURE); 
	} 
	else
	{
		printf("Connected\n");
	}
	
   	read(new_socket, str_data, 1024); 
 	printf("Request received: %s\n", str_data); 
  
  	int initial_velocity;
  	int launch_angle;
  	sscanf(str_data, "GET /?initial_velocity=%d&launch_angle=%d", &initial_velocity, &launch_angle);
   
  	int x = calculateTiming(initial_velocity, launch_angle);
  	int y = calculateVerticalDistance(initial_velocity, launch_angle);
  	int H = calculateHorizontalDistance(initial_velocity, launch_angle);
  	int l = calculateRange(initial_velocity, launch_angle);
   
   
  	char response[1024]; 
  	snprintf(response, sizeof(response), "Horizontal distance:%d\nVertical distance: %d\nMaximum Height: %d\nRange: %d", x, y, H, l);
  	printf("Response: %s\n", response);
 
   	write(new_socket, response, strlen(response));
     
   	close(new_socket); 
   	close(server_socket);
     
    	return 0; 
 }

