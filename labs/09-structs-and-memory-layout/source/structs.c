#include <stdio.h>

struct Player {
    int health;
    int ammo;
    char rank;
};

int main() {
    struct Player player = {100, 30, 'A'};

    printf("Health: %d\n", player.health);
    printf("Ammo: %d\n", player.ammo);
    printf("Rank: %c\n", player.rank);

    return 0;
}
