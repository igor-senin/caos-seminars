#include <us_xfr.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


#define BACKLOG 5

int main(int argc, char* argv[]) {
  struct sockaddr_un addr;
  int sfd;
  int cfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  // socket - создает сокет
  // bind - привязывает сокет к публичному адресу
  // listen - регистрирует сокет в ОС как пассивный (слушающий)
  // accept - принимает очередное клиентское соединение

  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1) {
    error_exit("socket");
  }

  if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
    error_exit("remove");
  }

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

  if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
    error_exit("bind");
  }

  // 

  if (listen(sfd, BACKLOG) == -1) {
    error_exit("listen");
  }

  // write(sfd, ...) -> ERROR

  for (;;) {
    cfd = accept(sfd, NULL, NULL);

    if (cfd == -1) {
      error_exit("accept");
    }

    while ((num_read = read(cfd, buf, BUF_SIZE)) > 0) {
      if (write(cfd, buf, num_read) != num_read) {
        error_exit("write");
      }
    }

    if (num_read == -1) {
      error_exit("read");
    }

    if (close(cfd) == -1) {
      soft_error("close");
    }

  }
}
