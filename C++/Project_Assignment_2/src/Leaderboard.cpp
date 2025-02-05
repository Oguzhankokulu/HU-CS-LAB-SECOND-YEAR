#include "Leaderboard.h"

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here

    ifstream ifile(filename);
    

    if (!ifile.is_open()) {
        ofstream ofile(filename);
        ofile.close();
    } else {
        string line, playerName;
        unsigned long score = 0;
        time_t time;
        getline(ifile, line);
        if (!line.empty()) {
            istringstream iss(line);
            iss >> score >> time >> playerName;
            LeaderboardEntry* head = new LeaderboardEntry(score, time, playerName);
            head_leaderboard_entry = head;
            LeaderboardEntry* current = head;
            while(getline(ifile, line)) {
                istringstream iss(line);
                iss >> score >> time >> playerName;
                current->next = new LeaderboardEntry(score, time, playerName);
                current = current->next;
            }
        }
    }

    ifile.close();
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    // TODO: Your code here
    ofstream ofile(filename);

    LeaderboardEntry* current = head_leaderboard_entry;
    while(current != nullptr) {
        ofile << current->score << " " << current->last_played << " " << current->player_name << "\n";
        current = current->next;
    }

    ofile.close();
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    // TODO: Your code here

    cout << "Leaderboard\n" << "-----------";
    LeaderboardEntry* current = head_leaderboard_entry;
    int count = 1;
    while(current != nullptr) {
        char buffer[20];
        // Format the timestamp as "HH:MM:SS/DD.MM.YYYY"
        strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", localtime(&current->last_played));
        string formattedTime(buffer);
        cout << "\n" << count << ". " << current->player_name << " " << current->score << " " << formattedTime;
        current = current->next;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // TODO: Your code here
    if (head_leaderboard_entry == nullptr) {
        head_leaderboard_entry = new_entry;
    }

    LeaderboardEntry* current = head_leaderboard_entry;
    LeaderboardEntry* temp = head_leaderboard_entry;
    int counter = 1;
    bool isAdded = false;
    while(current != nullptr) { // Parse all of the objects of the linked list
        if (current->score < new_entry->score && !isAdded) { // If the new entry score is bigger than add it to list.
            isAdded = true;
            if (temp == current) { // If it's the head of the list
                new_entry->next = head_leaderboard_entry;
                head_leaderboard_entry = new_entry;
                counter += 1;
            } else {
                temp->next = new_entry;
                new_entry->next = current;
                counter += 1;
            }
        }

        if (temp == current) { // the first iteration
            current = current->next;
            counter += 1;
        } else {
            current = current->next;
            temp = temp->next;
            counter +=1;
        }
        
        if (counter > 10) {
            temp->next = nullptr;
            return;
        }
    }

}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    // TODO: Your code here
    LeaderboardEntry* current = head_leaderboard_entry->next;
    LeaderboardEntry* temp = head_leaderboard_entry;
    while (current != nullptr) {
        delete temp;
        temp = current;
        current = current->next;
    }
}
