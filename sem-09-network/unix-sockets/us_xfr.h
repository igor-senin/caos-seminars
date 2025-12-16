#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>


#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE     100

void error_exit(const char* reason) {
  perror(reason);
  exit(EXIT_FAILURE);
}

void soft_error(const char* reason) {
  perror(reason);
}
