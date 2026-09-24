# Web Server

This project is a base study of how HTTP servers work. The initial scope of this project is to learn the basics of HTTP/1.1, routing, networking, etc. As this project evolves, a further detailed description will be added to build apon the existing documentation.

## Goals
- **TCP Server Foundation**: Establishes socket connections and listens for incoming traffic
- **HTTP/1.1 Request Parsing**: Parses request lines, headers, and message bodies
- **Basic Routing**: Maps incoming requests to handler functions
- **HTTP Response Generation**: Constructs valid HTTP responses with status codes and headers

## Requirements

- Linux
- Make/GCC

Current requirements restrict this project to be run as a Linux based server. 

## Networking

The first step has been accomplished to build a basic TCP server based on the popular internet tutorial [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/split/)

## HTTP

As a starting point the [RFC 9110](https://www.rfc-editor.org/rfc/rfc9110.html) specification will be used as a referrence to what is expected for the implementation of HTTP/1.1. Most of the information will be derived from this document.

### What is HTTP

HTTP is a protocol designed to interact between a client and a server. This interaction is represented as messages that could either be a request (question) or a response (answer). A client makes a request which gets sent over the protocol using networking. The server recieves that message and uses its allocated resources to perform actions and creates a representation of those actions as a response.

Each request is individually defined by a method which determines how the request gets parsed and routed. These requests also determine how the server is affected.

 with resources to provide a representation of the stateless actions requested by the client. 
The next step to this project is to implement HTTP/1.1 specification in order to handle basic request/response from a website. As an initial guide, the [RFC 9110](https://www.rfc-editor.org/rfc/rfc9110.html) will be followed to learn more about HTTP.