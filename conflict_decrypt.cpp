#include "formats/sah.hpp"

int main(int ac, char **av)
{
        if (ac != 3) {
                printf("usage: %s encr|decr <from> <dest>", av[0]);
        }
        if (!strcmp(av[1], "decr")) {
                printf("decrypt...\n");

                Shaiya::Sah sah(av[2]);
                sah.Write(av[3]);

                printf("done. Saved to: %s\n", av[3]);

        } else if (!strcmp(av[1], "encr")) {
                printf("not implemented yet\n");
        } else {
                printf("error. run %s for usage.\n", av[0]);
        }
        return 0;
}