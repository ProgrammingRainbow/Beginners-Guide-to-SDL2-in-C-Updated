#include "game.h"

int main(void) {
    int exit_status = EXIT_FAILURE;

    struct Game *game = game_new();
    if (game) {
        game_run(game);

        exit_status = EXIT_SUCCESS;
    }

    game_free(game);

    return exit_status;
}
