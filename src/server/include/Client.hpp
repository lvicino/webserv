#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "Conf.hpp"

class Client
{
    private:
        int                      _fd;
        std::string              _buffer;
        int _state;
        std::string _stash;
        std::vector<Conf> _confs;
        std::string to_send;

    public:
        Client();
        std::vector<HTTPRequest> _requests;
        std::vector<HTTPResponse> _responses;
        Client(int fd);
        Client(const Client& other);

        Client& operator=(const Client& src);

        // getter
        int getFd();
        std::string&                getBuffer();
        HTTPRequest&                getRequest();
        std::vector<HTTPRequest>&   getAllRequest();
        std::vector<Conf>&          getConf();
        HTTPResponse                getResponse();
        HTTPResponse &               getResponseCgi();
        int getState();
        std::string getToSend();

        // setter
        void setConf(Conf & conf);
        void setBuffer(std::string data);
        void setResponse(HTTPResponse & response);
        void setStash();
        void setToSend(std::string buff);

        // requests
        int hasRequests();
        int requestsAllComplete();
        int atleastOneComplete();
        int lastRequestComplete();
        void startNewRequest();
        void addRequest(HTTPRequest &newRequest);

        int hasResponse();
        ~Client();

};

#endif
