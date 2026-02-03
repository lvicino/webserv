#ifndef CGIMANAGER_HPP
#define CGIMANAGER_HPP

#include <vector>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "Cgi.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class CgiManager
{
    public:
        CgiManager();

        CgiManager(const CgiManager &other);

        CgiManager& operator=(const CgiManager &other);

        ~CgiManager();

        int checkCgiToRun(int& epfd);
		void checkCgiToDie();
        void addCgi(HTTPRequest& req,  int client_fd);
        int isCgiFd(uint32_t event, int fd);
        void deleteCgi(int fd);
        void getSize();
        Cgi *getCurrentCgi(int fd);

        int sendData(int fd);
        int receiveData(int fd);

    private:
        int fill_headers(CgiResponse &response);
        int fill_body(CgiResponse &response);
        int updateResponse(Cgi * current);
        std::vector<Cgi *> _cgis;
};


#endif
