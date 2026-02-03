/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvicino <lvicino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:32:37 by lvicino           #+#    #+#             */
/*   Updated: 2025/11/11 16:56:29 by lvicino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "HTTPRequest.hpp"
# include "CgiResponse.hpp"
#include "HTTPResponse.hpp"

# include <iostream>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <time.h>
# include <string.h>
# include <stdio.h>
# include <dirent.h>
# include <fcntl.h>

# define TIMEOUT 5

class	Cgi
{
	private:

		time_t			_start;
		pid_t			_pid;
		int				_return_status;
		int				_cgi_in;
		int				_cgi_out;
		int				_client_fd;
		std::string		_path;
		std::string		_body;
		HTTPResponse	*_httpResponse;
		CgiResponse		*_cgiResponse;

		std::string	_redirect_status;
		std::string	_script_filename;
		std::string	_auth_type;
		std::string	_content_length;
		std::string	_content_type;
		std::string	_gateway_interface;
		std::string	_path_info;
		std::string	_path_translated;
		std::string	_query_string;
		std::string	_remote_addr;
		std::string	_remote_host;
		std::string	_remote_ident;
		std::string	_remote_user;
		std::string	_request_method;
		std::string	_script_name;
		std::string	_server_name;
		std::string	_server_port;
		std::string	_server_protocol;
		std::string	_server_software;
		std::string	_cookies;

		char	**getEnvp(void);
		char	**getArgv(void);

		void	setPipe(int *fd1, int *fd2);
		void	freeTab(char **tab, size_t n);

	public:
		Cgi(HTTPRequest req, int client_fd);
		// Cgi(const Cgi &other);

		Cgi	&operator=(const Cgi &other);

		pid_t			getPid(void);
		std::string		getBody(void);
		int				getFdIn(void);
		int				getFdOut(void);
		int				getFdClient(void);
		CgiResponse		*getCgiResponse(void);
		HTTPResponse	*getHttpResponse(void);

		void	setHttpResponse(HTTPResponse * response);

		bool	execCgi(void);
		int		healtCheck(void);

		~Cgi();
};

#endif
