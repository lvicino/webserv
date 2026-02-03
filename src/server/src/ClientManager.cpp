#include "ClientManager.hpp"

ClientManager::ClientManager()
{
	return ;
}

void ClientManager::addClient(int fd, Socket * socket, std::vector<Conf>* confs)
{
	if (!_clients.count(fd))
	{
		Client * client = new Client(fd);
		int i = 0;
		for (std::vector<Conf>::iterator it = confs->begin(); it != confs->end(); ++it)
		{
			if (it->getHost() == socket->getHost() && it->getPort() == socket->getPort())
			{
				i++;
				client->setConf(*it);
			}
		}
		if (!_clients.count(fd))
			this->_clients[fd] = client;
	}
}


int ClientManager::prepareClientResponse(Client* client, CgiManager& cgimanager)
{
	int ret = 1;
	int fd = client->getFd();
	std::vector<Conf> &clientConf = client->getConf();
	for (std::vector<HTTPRequest>::iterator it = client->getAllRequest().begin();
	it != client->getAllRequest().end(); ++it)
	{
		if (it->complete == true)
		{
			HTTPRequestHandler reqHandler(*it);
			HTTPResponse response = reqHandler.handle(clientConf);
			if (it->cgi)
			{
				ret = 2;
				cgimanager.addCgi(*it, fd);
			}
			else
				client->setResponse(response);
		}
	}
	return ret;
}

int ClientManager::checkRequestTimeout()
{
	time_t current_time = time(0);

	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* client = it->second;
		if (!client)
			continue;

		if (client->getState())
		{
			HTTPRequest req = client->getRequest();
			if (req.start > -1 && !req.complete && current_time - req.start > TIMEOUT_REQUEST)
			{
				int fd = client->getFd();
				++it;
				sendCatchError(fd, "time");
				return fd;
			}
		}
	}
	return -1;
}

int ClientManager::getNbClient()
{
	return this->_clients.size();
}

int ClientManager::receiveData(int fd, CgiManager& cgiManager)
{
	char buffer[2048];
	ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);

	if (bytes == 0)
		return -1;
	else if (bytes == -1)
		return 0;

	Client *client = this->getClient(fd);
	std::string data(buffer, bytes);
	client->setBuffer(data);
	try {
		while (!client->getBuffer().empty())
		{
			if (!client->hasRequests() || client->lastRequestComplete())
				client->startNewRequest();
			HTTPRequest& req = client->getRequest();
			HTTPRequestParser::fillRequest(req, client->getBuffer());
			client->setStash();
			if (!req.complete)
				break;
		}
	}
	catch (std::exception& e)
	{
		std::string error(e.what());
		this->sendCatchError(fd, error);
		std::cout << e.what() << std::endl;
		return -1;
	}
	if (client->atleastOneComplete())
		return (this->prepareClientResponse(client, cgiManager));
	return 0;
}


void ClientManager::sendCatchError(int fd, std::string reason)
{
	int error;
	if (reason == "version")
		error = 505;
	else if (reason == "method")
		error = 501;
	else if (reason == "payload")
		error = 413;
	else if (reason == "time")
		error = 408;
	else
		error = 400;
	HTTPResponse resp(error, reason);
	resp.setConnection("closed");
	std::string toSend(resp.serialize());
	send(fd, toSend.c_str(), toSend.size(),0);
	std::map<int, Client*>::iterator it = this->_clients.find(fd);

	if (it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}
}

void ClientManager::deleteClient(int client_fd)
{
	std::map<int, Client*>::iterator it = this->_clients.find(client_fd);
	if (it != this->_clients.end())
	{
		delete it->second;
		this->_clients.erase(it);
	}
}

int ClientManager::sendData(int fd)
{
	int ret = 2;
	Client *client = this->getClient(fd);
	std::string toSend = client->getToSend();

	if (!toSend.size())
	{
		HTTPResponse resp = client->getResponse();
		toSend = resp.serialize();
		std::string cnx = resp.getConnection();
		if (cnx == "close")
			ret = -1;
		else if (client->hasResponse())
			ret = 1;

	}

	const char *test = toSend.c_str();
	ssize_t bytes = send(fd, test, toSend.size(), 0);

	std::string newSend(toSend.erase(0, bytes));

	if (!bytes)
		ret = 1;
	else if (bytes == -1)
		ret = -1;
	else if (newSend.size())
		ret = 1;

	client->setToSend(newSend);
	return ret;
}

Client * ClientManager::getClient(int fd)
{
	return this->_clients[fd];
}


ClientManager::~ClientManager()
{
    std::map<int, Client*>::iterator it = this->_clients.begin();
    while (it != this->_clients.end())
    {
        close(it->second->getFd());
        delete it->second;
        this->_clients.erase(it++);
    }
}