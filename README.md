# webserv
42 webserv

A compatible HTTP/1.1 server written in C++ 98. This project involves writing a web server from scratch that can handle multiple client connections, manage static and dynamic content, and support standard HTTP methods.

## Overview

`webserv` is designed to be non-blocking and uses I/O multiplexing (e.g., `poll()` or `epoll()`) to handle concurrent connections efficiently. It parses standard HTTP requests, routes them to the appropriate resources based on a configuration file, and serves responses back to the client.

## Features

* **HTTP/1.1 Support:** Handles standard headers and status codes.
* **HTTP Methods:**
    * `GET`: Retrieve resources.
    * `POST`: Submit data (supports chunked transfer encoding and file uploads).
    * `DELETE`: Remove resources.
* **I/O Multiplexing:** Uses non-blocking I/O to manage multiple file descriptors simultaneously without threads (as per project constraints).
* **CGI Support:** Executes external scripts (Python, PHP) via the Common Gateway Interface.
* **Static Website Hosting:** Serves HTML, CSS, images, and other static assets.
* **Custom Configuration:** Reads a configuration file to set up servers, ports, routes, and error pages.
* **Error Handling:** Custom default error pages (404, 500, etc.).
