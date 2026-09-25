#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

#include "../inc/http-handler.h"

#define PORT "8000"
#define BACKLOG 10

#define MAXBUFLEN 512

void init(struct addrinfo*, int*);
void server(int);
void print_ip_addresses(struct addrinfo*);

void sigchld_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(void)
{
  struct addrinfo hints, *servinfo;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  struct sigaction sa;

  int sockfd;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;  // Either IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  printf("IP addresses for localhost:\n\n");

  init(servinfo, &sockfd);

  if (servinfo == NULL){
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }
  
  if(listen(sockfd, BACKLOG) == -1){
    perror("listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = ERESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  server(sockfd);
  return 0;
}

void init(struct addrinfo *servinfo, int *sockfd){
  struct addrinfo *p;
  int yes=1;

  for(p = servinfo; p != NULL; p = p->ai_next) {
    *sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (*sockfd == -1){
      perror("server: socket");
      continue;
    }
    
    int opt = setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (opt == -1){
      perror("setsockopt");
      exit(1);
    }

    int b = bind(*sockfd, p->ai_addr, p->ai_addrlen);

    if (b == -1){
      close(*sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  servinfo = p;

  //freeaddrinfo(p);
}

void server(int sockfd){
  int new_fd;

  struct sockaddr_storage their_addr;
  socklen_t addr_size;

  char ipstr[INET6_ADDRSTRLEN];

  while (1){
    addr_size = sizeof their_addr;

    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    if (new_fd == -1){
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), ipstr, sizeof ipstr);

    printf("Connection Accepted: %s\n", ipstr);

    char* msg = "Hello, world!\n";
    int len = strlen(msg);

    char recv_msg[MAXBUFLEN];
    int recv_len;

    if(!fork()){
      //close(sockfd);
      if (send(new_fd, msg, len, 0) == -1)
        perror("send");

      recv_len = recv(new_fd, recv_msg, MAXBUFLEN-1, 0);
      if (recv_len == -1)
        perror("recv");

      close(new_fd);

      recv_msg[recv_len] = '\0';

      Request request;

      parse_http_request(recv_msg, &request);

      //printf("Received msg(%d): %s\n", recv_len, recv_msg);



      //exit(1);
    }



    close(new_fd);
  }
}

void print_ip_addresses(struct addrinfo *servinfo){
  struct addrinfo *p;

  char ipstr[INET6_ADDRSTRLEN];

  for(p = servinfo; p != NULL; p = p->ai_next) {
    void *addr;
    char *ipver;
    struct sockaddr_in *ipv4;
    struct sockaddr_in6 *ipv6;

    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    if (p->ai_family == AF_INET) { // IPv4
      ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else { // IPv6
      ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    // convert the IP to a string and print it:
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
  }
}
