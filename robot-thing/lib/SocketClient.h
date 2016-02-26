#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP

#include <lib/MsgChannel.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <string>
#include <map>

namespace frc973 {

class SocketClient {
public:
    static void *RunSocketClient(void*);

	class SocketListener {
	public:
		virtual void OnValueChange(std::string name, std::string newValue) = 0;
	protected:
		virtual ~SocketListener() {};
	};

	static void AddListener(std::string name, SocketListener *listener);
private:
    static bool Start();
    static void HandleAll(void*);
    static void HandleNetInput();
    static void HandleRobotInput();
    static void UpdateRobot(char *key, char *val);

    static int sockfd;
    static fd_set readfds;

    static Channel<std::string> sendChan;
    static Channel<std::string> recvChan;

    static void RecieveInput(int sockfd);
    static int sendall(const int s, const char *buf, int *len);

	static std::map<std::string, SocketListener*> listeners;
};

}
#endif
