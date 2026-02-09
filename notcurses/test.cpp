#include <cstring>
#include <notcurses/direct.h>
#include <notcurses/notcurses.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  notcurses_options ncopt;
  memset(&ncopt, 0, sizeof(ncopt));

  struct notcurses *nc = notcurses_init(&ncopt, stdout);
  struct ncplane *stdplane = notcurses_stdplane(nc);

  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 25; j+=5) {
      ncplane_putchar_yx(stdplane, i, j, '*');
      notcurses_render(nc);
      usleep(5000);
    }
  }
  sleep(2);
  notcurses_stop(nc);
  return 0;
}
