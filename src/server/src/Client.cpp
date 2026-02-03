#include "Client.hpp"

Client::Client()
{
    return ;
}

Client::Client(int fd)
{
    to_send = "";
    _state = 0;
    this->_fd = fd;
}

Client::Client(const Client& other)
{
    *this = other;
    return;
}


void Client::setToSend(std::string buff)
{
    to_send = buff;
}

std::string Client::getToSend()
{
    return to_send;
}

int Client::getFd()
{
    return this->_fd;
}

std::string& Client::getBuffer()
{
    return this->_buffer;
}

void Client::setConf(Conf & conf)
{
    this->_confs.push_back(conf);
}
void Client::setBuffer(std::string data)
{
    this->_buffer = this->_stash + data;
}

void Client::setStash()
{
    this->_stash = this->_buffer;
}

int Client::hasRequests()
{
    return this->_requests.size();
}

int Client::hasResponse()
{
    return this->_responses.size();
}

int Client::lastRequestComplete()
{
    return this->_requests.back().complete;
}

void Client::startNewRequest()
{
    this->_requests.push_back(HTTPRequest());
    this->_state = 1;
}

void Client::addRequest(HTTPRequest & request)
{
    this->_requests.push_back(request);
}

HTTPRequest & Client::getRequest()
{
    return this->_requests.back();
}

HTTPResponse& Client::getResponseCgi()
{
    return  this->_responses.front();
}

int Client::getState()
{
    return _state;
}

HTTPResponse Client::getResponse()
{
    HTTPResponse resp;
    if (this->_responses.size())
    {
        resp = this->_responses.front();
        this->_responses.erase(this->_responses.begin());

        if (!this->_requests.empty())
            this->_requests.erase(this->_requests.begin());
    }
    _state = 0;
    return resp;
}

std::vector<HTTPRequest>& Client::getAllRequest()
{
    return this->_requests;
}

std::vector<Conf>&          Client::getConf()
{
    return this->_confs;
}

Client& Client::operator=(const Client& src)
{
    this->_fd = src._fd;
    this->_buffer = src._buffer;
    this->_requests = src._requests;
    this->_responses = src._responses;
    this->_confs = src._confs;
    this->_state = src._state;
    this->to_send = src.to_send;
    return *this;
}

int Client::requestsAllComplete()
{
    unsigned int i = 0;
    for (std::vector<HTTPRequest>::iterator it = this->_requests.begin(); it != this->_requests.end(); ++it)
    {
        if (it->complete == true)
            i++;
    }
    return i == this->_requests.size();
}

int Client::atleastOneComplete()
{
    int i = 0;
    for (std::vector<HTTPRequest>::iterator it = this->_requests.begin(); it != this->_requests.end(); ++it)
    {
        if (it->complete == true)
            i++;
    }
    return i > 0;
}

void Client::setResponse(HTTPResponse & response)
{
    this->_responses.push_back(response);
}

Client::~Client()
{
    return ;
}
