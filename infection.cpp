#include <iostream>
#include <queue>

class User {
  public:
    // Since in clasroom context, a user can have multiple teachers, we allow multiple mentors
    // for each user
    std::vector<User*> master_list;   // A list of the user's mentors
    std::vector<User*> padawan_list;  // A list of the user's pupils
    double version;
    User ();                         // Constructors
    User (double version_num);
    void totalInfection(double version_num);           // Infect all mentors and pupils connected this this user
  private:
    void upwardInfection(double version_num);          // Infect all masters
    // void downwardInfection(double version_num);        // Infect all pupils

};

User::User () {
    User (1.0);
}

User::User (double version_num) {
    version = version_num;
}

void User::totalInfection(double version_num) {
    // Process self first
    version = version_num;
    upwardInfection(version_num);
    // downwardInfection(version_num);
}

void User::upwardInfection(double version_num) {
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

int main (int argc, char *argv[]) {
    argc = argc;
    argv = argv;
}
