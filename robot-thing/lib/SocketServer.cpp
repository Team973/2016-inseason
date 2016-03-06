
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "SocketServer.hpp"
#include <map>
#include <string>
#include "pthread.h"

#define PORT "5800"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

#define MAX_MSG_LEN 128

namespace frc973 {

int SocketServer::sockfd;
int SocketServer::new_fd;
fd_set SocketServer::readfds;

std::map<std::string, SocketServer::SocketListener*> SocketServer::listeners;

void SocketServer::sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *SocketServer::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void SocketServer::Start(void)
{
    //int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    //struct sockaddr_storage their_addr; // connector's address information
    //socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    //char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
        	int iSetOption = 1;
        	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
        			(char*)&iSetOption, sizeof(iSetOption));
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");
}

void SocketServer::AddListener(std::string var, SocketServer::SocketListener *listener) {
    listeners[var] = listener;
}

/**
 * Changes the values of constants on the robot.  Should be called only by 
 * RecieveInput.
 */
void SocketServer::UpdateRobot(char *key, char *val) {
    std::string msg (key);
    if (listeners.find(msg) != listeners.end()) {
        listeners[msg]->OnValueChange(msg, val);
    }
    else {
        printf("No listener found for key '%s' so set event falling through\n", key);
    }
}

bool SocketServer::RecieveInput(int sock) {
    printf("Recieved user input\n");
    char buffer[MAX_MSG_LEN],
    bytesTotal;
    int bytesRecieved = 0;

    int n = recv(sock, &bytesTotal, 1, 0);
    printf("recieving %d bytes\n", bytesTotal);

    if (bytesTotal == 0) {
    	printf("Client disconected... murdering child\n");
        return false;
    }

    while (bytesRecieved < bytesTotal) {
        n = recv(sock, 
                buffer + bytesRecieved, 
                sizeof(buffer) - bytesRecieved, 
                0);
        //printf("chunk: %s\n", buffer + bytesRecieved);
        if (n == 0) {

        }
        else if (n > 0) {
            bytesRecieved += n;
        }
        else {
            return false;
        }
    }
    printf("msg recieved %s.  bytes recieved %d\n", buffer, bytesRecieved);
    buffer[bytesRecieved] = '\0';

    char* key = strtok(buffer,"=");
    char* val = strtok(NULL,"=");
    printf("key: %s value: %s\n",key,val);
    UpdateRobot(key, val);

    return true;
}

typedef struct {
	fd_set readfds;
	int new_fd;
} ChildInfo;

void *SocketServer::StartChild(void *in) {
	ChildInfo *info = (ChildInfo*) in;
	bool clientStillAlive = true;
	while (clientStillAlive) {
		FD_ZERO(&(info->readfds));
		FD_SET(info->new_fd, &(info->readfds));

		fd_set readySockets = readfds;

		//Select will change readySockets such that it's full of only sockets
		//with input in them
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 50000;
		if (select(new_fd + 1, &readySockets, NULL, NULL, &tv) == -1) {
			perror("select");
		}

		if (FD_ISSET(info->new_fd, &readySockets)) {
		}
		clientStillAlive = RecieveInput(new_fd);
	}
	return NULL;
}

void SocketServer::Run() {
    while(1) {  // main accept() loop
        struct sockaddr_storage their_addr; // connector's address information
        char s[INET6_ADDRSTRLEN];
        socklen_t sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr),
                s, sizeof s);
        printf("server: got connection from %s\n", s);

        ChildInfo *childInfo = (ChildInfo*) malloc(sizeof(ChildInfo));
        childInfo->new_fd = new_fd;
        childInfo->readfds = readfds;

        pthread_t childThread;
        pthread_create(&childThread, NULL, StartChild, childInfo);
    }
}

void* SocketServer::RunSocketServer(void*) {
    frc973::SocketServer::Start();
    frc973::SocketServer::Run();
    return NULL;
}

}
