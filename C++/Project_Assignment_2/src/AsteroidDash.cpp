#include "AsteroidDash.h"


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    // TODO: Your code here

    ifstream file(input_file);

    if (!file.is_open()) {
        cerr << "File cannot be opened!\n";
        exit(-1);
    }

    vector<vector<bool>> temp = get_matrix(file);

    for (int i = 0; i < temp.size(); i++) {
        vector<int> row(temp[i].begin(), temp[i].end());
        space_grid.push_back(row);
    }
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    // TODO: Your code here

    ifstream file(player_file_name);

    if (!file.is_open()) {
        std::cerr << "File cannot be opened!\n";
        exit(-1);
    }

    int ammo = 10, lives = 3, row, column;
    file >> row >> column;
    vector<vector<bool>> shape = get_matrix(file);

    player = new Player(shape, row, column, player_name, ammo, lives);

    file.close();
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here

    ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "File cannot be opened!\n";
        exit(-1);
    }

    string line;
    int s, t;
    ObjectType type = ASTEROID;
    vector<vector<bool>> shape;
    CelestialObject* current;
    bool isLastEmpty = false;
    while (getline(file, line)) { // Create new CelestialObjects according to given input
        if(line.empty()){ // If the line is empty then there is enough info to create a new object
            if (isLastEmpty) {
                continue;
            }
            //create celestial object (using shape, s, t, e)
            if (celestial_objects_list_head == nullptr) {
                celestial_objects_list_head = new CelestialObject(shape, type, s, t);
                current = celestial_objects_list_head;
            } else {
                current->next_celestial_object = new CelestialObject(shape, type, s, t);
                current = current->next_celestial_object;
            }
            // To reset type and shape, it won't change if there is no e in the input
            type = ASTEROID;
            shape = vector<vector<bool>>();
            isLastEmpty = true;
            continue;
        } else { // If the line is not empty then get the necessery info for the next object
            isLastEmpty = false;
            if (line[0] == '[' || line[0] == '{' || line[0] == '1' || line[0] == '0') {
                // Get shape line excluding the brackets
                if (line[0] == '[' || line[0] == '{') {
                    line = line.substr(1);  // Remove the starting bracket
                }
                if (line.back() == ']' || line.back() == '}') {
                    line = line.substr(0, line.size() - 1);  // Remove the ending bracket if it exists
                }

                vector<bool> row;
                for (char c : line) {
                    if (c == '1') {
                        row.push_back(true);
                    } else if (c == '0') {
                        row.push_back(false);
                    }
                    // Ignore any other characters (like spaces or tabs)
                }
                shape.push_back(row);

            } else if (line[0] == 's') {
                istringstream(line.substr(2)) >> s;  // Extract 's' value
            } else if (line[0] == 't') {
                istringstream(line.substr(2)) >> t;  // Extract 't' value
            } else if (line[0] == 'e') {
                string str = line.substr(2);
                type = (str == "life") ? LIFE_UP : AMMO;  // Set 'e' value
            }
        }
    }
    current->next_celestial_object = new CelestialObject(shape, type, s, t);
    //create last celestial object
    
    // Calculate rotations
    current = celestial_objects_list_head;
    while(current != nullptr) {
        calculate_rotations(current);
        current = current->next_celestial_object;
    }
}

void AsteroidDash::print_info() const {
    cout << "Tick: " << game_time << "\n";
    cout << "Lives: " << player->lives << "\n";
    cout << "Ammo: " << player->current_ammo << "\n";
    cout << "Score: " << current_score << "\n";
    if (leaderboard.head_leaderboard_entry == nullptr) {
        cout << "High Score: " << current_score << "\n";
    } else {
        if (current_score >= leaderboard.head_leaderboard_entry->score) {
            cout << "High Score: " << current_score << "\n";
        } else {
            cout << "High Score: " << leaderboard.head_leaderboard_entry->score << "\n";
        }
    }
}

// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here
    

    for (int i = 0; i < space_grid.size(); i++) {
        for (int j = 0; j < space_grid[0].size(); j++) {
            if (space_grid[i][j] == 1) {
                cout << occupiedCellChar;
            } else if (space_grid[i][j] == 0) {
                cout << unoccupiedCellChar;
            }
        }
        cout << "\n";
    }
    cout << "\n";
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    // TODO: Your code here
    vector<vector<bool>> spacecraft_shape = player->spacecraft_shape;

    int pSize = projectiles.size();
    for (int i = 0; i < pSize; i++) {
        projectiles[i].col += 1;
        if (projectiles[i].col >= space_grid[0].size()) {
            projectiles.erase(projectiles.begin() + i);
            i -= 1;
            pSize -= 1;
        }
    }

    // Projectile collision with objects
    int projectilesSize = projectiles.size();
    for (int i = 0; i < projectilesSize; i++) {
        if (space_grid[projectiles[i].row][projectiles[i].col] == 1) {
            CelestialObject* previous = celestial_objects_list_head;
            CelestialObject* current = celestial_objects_list_head;
            while (current != nullptr) {
                bool isEmpty = false;
                if (current->time_of_appearance <= game_time && current->object_type == ASTEROID) {
                    if (current->starting_row < projectiles[i].row && projectiles[i].row < current->starting_row + current->shape.size()) {
                        if ((space_grid[0].size() - (game_time - (current->time_of_appearance - 1))) == projectiles[i].col) {
                            space_grid[projectiles[i].row][projectiles[i].col] = 0;
                            current->shape[projectiles[i].row - current->starting_row][0] = 0;
                            current->removedCellNumber += 1;
                            current_score += 10;
                            
                            isEmpty = true;
                            for (int i = 0; i < current->shape.size(); i++) {
                                for (int j = 0; j < current->shape[0].size(); j++) {
                                    if (current->shape[i][j] == 1) {
                                        isEmpty = false;
                                    }
                                }
                            }

                            if (!isEmpty) {
                                calculate_rotations(current);
                                if (projectiles[i].row - current->starting_row <= current->shape.size()/2) {
                                    current = current->right_rotation;
                                } else {
                                    current = current->left_rotation;
                                }
                            }
                            
                            // Delete the projectile
                            projectiles.erase(projectiles.begin() + i);
                            i -= 1;
                            projectilesSize -= 1;
                            break;
                        }
                    }
                }
                if (isEmpty) { // If the shape is empty, then delete the object. Add all points
                    current_score += current->removedCellNumber*100;
                    if (current == previous) {
                        celestial_objects_list_head = current->next_celestial_object;
                        current = current->next_celestial_object;
                        previous = previous->next_celestial_object;
                    } else {
                        previous->next_celestial_object = current->next_celestial_object;
                        current = current->next_celestial_object;
                    }
                } else {
                    if (current == previous) {
                        current = current->next_celestial_object;
                    } else {
                        current = current->next_celestial_object;
                        previous = previous->next_celestial_object;
                    }
                }
            }
        }
    }


    for (int i = 0; i < space_grid.size(); i++) {
        for (int j = 0; j < space_grid[0].size(); j++) {
            space_grid[i][j] = 0;
            for (Projectile projectile : projectiles) {
                if (projectile.row == i && projectile.col == j) {
                    space_grid[i][j] = 1;
                }
            }
        }
    }

    if(!game_over) {
        for (int i = 0; i < spacecraft_shape.size(); i++) {
            for (int j = 0; j < spacecraft_shape[0].size(); j++) {
                if (spacecraft_shape[i][j] == 1) {
                    space_grid[player->position_row + i][player->position_col + j] = spacecraft_shape[i][j];
                }
            }
        }
    }


    // Object collision with player
    CelestialObject* previous = celestial_objects_list_head;
    CelestialObject* current = celestial_objects_list_head;
    while (current != nullptr) {
        bool isRemoved = false;
        if (current->time_of_appearance <= game_time) {
            int height = current->shape.size();
            int width = current->shape[0].size();
            
            for (int row = 0; row < current->shape.size(); row++) {
                // Check if there is a player in the left, (If there is an object in the last tick where player stands right now (player moved right), there is a collision)
                int isNew = (game_time == current->time_of_appearance) ? 1 : 0;
                if (space_grid[current->starting_row + row][space_grid[0].size() - ((game_time-1) - (current->time_of_appearance - 1)) + isNew] == 1 &&
                    space_grid[current->starting_row + row][space_grid[0].size() - (game_time - (current->time_of_appearance - 1))] == 1) {
                    if (current->object_type == ASTEROID) {
                        player->lives -= 1;
                        if (player->lives == 0) {
                            game_over = true;
                        } else {
                            current_score -= 1;
                        }
                        if (current == previous) {
                            celestial_objects_list_head = celestial_objects_list_head->next_celestial_object;
                            current = celestial_objects_list_head;
                            previous = celestial_objects_list_head;
                        } else {
                            previous = current -> next_celestial_object;
                            current = current->next_celestial_object;
                        }
                        isRemoved = true;
                        break;
                    } else if (current->object_type == LIFE_UP) {
                        player->lives += 1;
                        if (current == previous) {
                            celestial_objects_list_head = celestial_objects_list_head->next_celestial_object;
                            current = celestial_objects_list_head;
                            previous = celestial_objects_list_head;
                        } else {
                            previous = current -> next_celestial_object;
                            current = current->next_celestial_object;
                        }
                        isRemoved = true;
                        break;
                    } else {
                        player->current_ammo = player->max_ammo;
                        if (current == previous) {
                            celestial_objects_list_head = celestial_objects_list_head->next_celestial_object;
                            current = celestial_objects_list_head;
                            previous = celestial_objects_list_head;
                        } else {
                            previous = current -> next_celestial_object;
                            current = current->next_celestial_object;
                        }
                        isRemoved = true;
                        break;
                    }
                    
                }
            }
            
            if (!isRemoved) {
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        space_grid[current->starting_row + i][space_grid[0].size() - (game_time - (current->time_of_appearance - 1)) + j] = current->shape[i][j];
                    }
                }
            }
        }
        
        if(!isRemoved) {
            current = current->next_celestial_object;
        }
    }
   

    
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // TODO: Your code here
    if (player->current_ammo > 0) {
        int exit_row = player->position_row + player->spacecraft_shape.size() / 2;
        int exit_col = player->position_col + player->spacecraft_shape[0].size() - 1;
        Projectile projectile = Projectile{exit_row, exit_col}; // row and column
        projectiles.push_back(projectile);
        player->current_ammo -= 1;
    }
    
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here

    delete player;
}

vector<vector<bool>> AsteroidDash::get_matrix(ifstream &file) {
    vector<vector<bool>> matrix;
    
    string line;
    bool value = false;
    while(getline(file, line)) {
        vector<bool> row;

        for (char c : line) {
            if (c == ' ' || c == '\t') {
                continue;  // Skip spaces or tabs
            }
            value = (c == '1');  // Convert '1' to true, '0' to false
            row.push_back(value);
        }

        //Check if it's empty
        if(!row.empty()){
            matrix.push_back(row);
        }
    }

    return matrix;
}

vector<vector<bool>> AsteroidDash::get_right_rotation(vector<vector<bool>> shape) {
    vector<vector<bool>> temp(shape[0].size(), vector<bool>(shape.size(), false));

        // Implementation of rotating right
        for (int i = 0; i < shape[0].size(); i++) { // for column of default and row of rotated
            for (int j = shape.size(); j > 0; j--) { // for row of default and column of rotated, starting from the end
                temp[i][shape.size() - j] = shape[j - 1][i];
            }
        }
    
    return temp;
}

vector<vector<bool>> AsteroidDash::get_left_rotation(vector<vector<bool>> shape) {
    vector<vector<bool>> temp(shape[0].size(), vector<bool>(shape.size(), false));
    
    // Implementation of rotating left
        for (int i = 0; i < shape.size(); i++) { // for row of default and column of rotated
            for (int j = shape[0].size(); j > 0; j--) { // for column of default and row of rotated, starting from the end
                temp[j - 1][i] = shape[i][shape[0].size() - j];
            }
        }
    
    return temp;
}

void AsteroidDash::calculate_rotations(CelestialObject* cObject) {

        vector<vector<bool>> rshape = get_right_rotation(cObject->shape);
        vector<vector<bool>> lshape = get_left_rotation(cObject->shape);
        int row = cObject->starting_row, time = cObject->time_of_appearance;
        if (rshape == cObject->shape) { // If right rotation is equal to the default shape then there is 1 variation
            cObject->right_rotation = cObject;
            cObject->left_rotation = cObject;
            return;
        } else if (rshape == lshape) { // If right and left rotations are equal then there are 2 variations
            CelestialObject* rObject = cObject;
            rObject = new CelestialObject(rshape, cObject->object_type, row, time);
            cObject->right_rotation = rObject;
            cObject->left_rotation = rObject;
            rObject->left_rotation = cObject;
            rObject->right_rotation = cObject;
            rObject->next_celestial_object = cObject->next_celestial_object;
            return;
        } else { // If right and left rotations are different then there are 4 variations.
            CelestialObject* rObject = cObject;
            CelestialObject* temp = cObject;
            for (int i = 0; i < 3; i++) {
                rshape = get_right_rotation(rObject->shape);
                temp = new CelestialObject(rshape, cObject->object_type, row, time);
                rObject->right_rotation = temp;
                temp->left_rotation = rObject;
                temp->next_celestial_object = rObject->next_celestial_object;
                rObject = temp;
            }

            rObject->right_rotation = cObject;
            cObject->left_rotation = rObject;
            return;
        }
}