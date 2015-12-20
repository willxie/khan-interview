#include <iostream>
#include <queue>
#include <algorithm>

class User {
  public:
    // Since in clasroom context, a user can have multiple teachers, we allow multiple mentors
    // for each user
    double version;
    std::vector<User*> master_list;   // A list of the user's mentors
    std::vector<User*> apprentice_list;  // A list of the user's pupils

    User ();                         // Constructors
    User (double version_num);
    void totalInfection (double version_num);           // Infect all mentors and pupils connected this this user
  private:
    void upwardInfection (double version_num);          // Infect all masters
    // void downwardInfection(double version_num);        // Infect all pupils

};

User::User () {
    User (1.0);
}

User::User (double version_num) {
    version = version_num;
    // printf("Hello %f", version);
}

void User::totalInfection (double version_num) {
    // Process self first
    version = version_num;
    upwardInfection(version_num);
    // downwardInfection(version_num);
}

void User::upwardInfection (double version_num) {
    // Breath first search upward using queue
    std::queue<User*> bfs_queue;
    for (auto& master_ptr : master_list) {
        bfs_queue.push(master_ptr);
    }

    while (!bfs_queue.empty()) {
        User* user_ptr = bfs_queue.front();
        bfs_queue.pop();
        user_ptr->version = version_num;
        // Add all mentors to the list
        for (auto& master_ptr : master_list) {
            bfs_queue.push(master_ptr);
        }

    }

}

void make_connection (User* master_ptr, User* apprentice_ptr) {
    // Add apprentice to master only if the connection doesn't already exist
    if (std::find(master_ptr->apprentice_list.begin(),
                  master_ptr->apprentice_list.end(),
                  apprentice_ptr) == master_ptr->apprentice_list.end()) {
        master_ptr->apprentice_list.push_back(apprentice_ptr);
    }

    // Similarly for the reverse
    if (std::find(apprentice_ptr->master_list.begin(),
                  apprentice_ptr->master_list.end(),
                  master_ptr) == apprentice_ptr->master_list.end()) {
        apprentice_ptr->master_list.push_back(master_ptr);
    }

}

int main (int argc, char *argv[]) {
    // Dummy for suppressing warnings compiler flags
    argc = argc;
    argv = argv;

    // Initialize
    std::vector<User> user_list;
    for (int i = 0; i < 100; ++i) {
        User user (1.0);
        user_list.push_back(user);
    }

    // Make connections
    for (int i = 10; i < 20; ++i) {
        make_connection(&(user_list[i]), &(user_list[0]));
    }

    // Check
    for (int i = 0; i < 100; i += 10) {
        for (int j = 0; j < 10; ++j) {
            printf("%.1f   ", user_list[i + j].version);
        }
        printf("\n");
        printf("\n");
    }

}
