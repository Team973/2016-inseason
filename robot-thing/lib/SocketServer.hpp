#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include <string>
#include <map>

namespace frc973 {

class SocketServer {
public:
    static void Start(void);
    static void Run();
    static void* RunSocketServer(void*);

	class SocketListener {
	public:
		virtual void OnValueChange(std::string name, std::string newValue) = 0;
	protected:
		virtual ~SocketListener() {};
	};

	static void AddListener(std::string name, SocketListener *listener);
private:
    static void sigchld_handler(int s);
    static void *get_in_addr(struct sockaddr *sa);
    static void UpdateRobot(char *key, char *val);
    static bool RecieveInput(int sock);

    static void *StartChild(void*);

    static int sockfd, new_fd;
    static fd_set readfds;

	static std::map<std::string, SocketListener*> listeners;
};

}

#endif
