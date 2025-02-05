#include "GameController.h"

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // TODO: Your code here 
    // don't forget to increase col of the projectiles (I didn't do it in the update so do it in here)

    ifstream file(commands_file);

    if (!file.is_open()) {
        cerr << "File cannot be opened!\n";
        exit(-1);
    }

    game->update_space_grid();

    string line;
    while(getline(file, line)) {
        // Move every projectile to the right and check if any of them collides with asteroids
        if (line == "NOP") { // Increase the tick and slide objects to the left
        } else if (line == "PRINT_GRID") {
            game->print_info();
            game->print_space_grid();
        } else if (line == "SHOOT") {
            game->shoot();
        } else if (line == "MOVE_UP") {
            game->player->move_up();
        } else if (line == "MOVE_DOWN") {
            game->player->move_down(game->space_grid.size());
        } else if (line == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid[0].size());
        } else if (line == "MOVE_LEFT") {
            game->player->move_left();
        } else {
            cout << "Unknown command: " << line << "\n";
            continue;
        }

        game->update_space_grid();  
        game->game_time += 1;
        game->current_score += 1;

        if (game->game_over == true) {
            game->game_time -= 1;
            game->current_score -= 1;
            game->update_space_grid();
            LeaderboardEntry* playerEntry = new LeaderboardEntry(game->current_score, time(nullptr), game->player->player_name);
            if (game->leaderboard.head_leaderboard_entry == nullptr) {
                game->leaderboard.head_leaderboard_entry = playerEntry;
            } else {
                LeaderboardEntry* current = game->leaderboard.head_leaderboard_entry;
                while (current != nullptr) {
                    if (game->current_score > current->score) {
                        game->leaderboard.insert(playerEntry);
                        break;
                    }
                    current = current->next;
                }
            }
            cout << "GAME OVER!\n";
            game->print_info();
            cout << "Player: " << game->player->player_name << "\n";
            game->print_space_grid();
            game->leaderboard.print_leaderboard();
            game->leaderboard.write_to_file(game->leaderboard_file_name);
            return;
        }
    }
    LeaderboardEntry* playerEntry = new LeaderboardEntry(game->current_score, time(nullptr), game->player->player_name);
    if (game->leaderboard.head_leaderboard_entry == nullptr) {
        game->leaderboard.head_leaderboard_entry = playerEntry;
    } else {
        LeaderboardEntry* current = game->leaderboard.head_leaderboard_entry;
        while (current != nullptr) {
            if (game->current_score > current->score) {
                game->leaderboard.insert(playerEntry);
                break;
            }
            current = current->next;
        }
    }
    game->game_time -= 1;
    game->current_score -= 1;
    // iki defa yazdırıyor print space grid onu kontrol et player ismini ekstra yazdırman lazım
    cout << "GAME FINISHED! No more commands!\n";
    game->print_info();
    cout << "Player: " << game->leaderboard.head_leaderboard_entry->player_name << "\n";
    game->print_space_grid();
    game->leaderboard.print_leaderboard();
    game->leaderboard.write_to_file(game->leaderboard_file_name);
    return;
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
}
