#include <iostream>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <cassert>

class User {
  public:
    // Since in clasroom context, a user can have multiple teachers, we allow multiple mentors
    // for each user
    unsigned int id;            // Unique id
    double version;
    unsigned int infection_num;         // Random number assigned for each infection to prevent cycles
    std::vector<User*> master_list;   // A list of the user's mentors
    std::vector<User*> apprentice_list;  // A list of the user's pupils

    User ();                         // Constructors
    User (double version_num);
    void totalInfection (double version_num);           // Infect all mentors and pupils connected this this user
    void limitedInfection (double version_num, int num_users);  // Infect a set number of users
    void limitedInfectionBounded (double version_num, int num_users); // Infect a set number of users; a master and its pupils have the same version during processing

  private:
    void upwardInfection (double version_num);          // Infect all masters
    void downwardInfection(double version_num);         // Infect all pupils

};

User::User () {
    User (1.0);
}

User::User (double version_num) {
    version = version_num;
}

void User::totalInfection (double version_num) {
    limitedInfection(version_num, -1);
}

// Negative num_users means infect everybody
// This version infects everybody until to the num_users bound. It will try to infect pupils
// first and then connected mentors in a BFS fashion
void User::limitedInfection (double version_num, int num_users) {
    std::queue<User*> bfs_queue;

    // Random number used for this infection session
    unsigned int infection_num = rand();

    // Initialize number of users
    bool num_users_infinite = false; // If true, this session infects all connected users
    int num_users_left = 0;
    if (num_users < 0) {
        num_users_infinite = true;
    } else {
        num_users_left = num_users;
    }

    // Add self first
    bfs_queue.push(this);

    while (!bfs_queue.empty()) {
        User* user_ptr = bfs_queue.front();
        bfs_queue.pop();

        // Process only the ones we haven't
        if (user_ptr->infection_num != infection_num) {
            // Return if exceeded number of infected users
            if (num_users_left == 0 && (!num_users_infinite)) {
                return;
            }

            user_ptr->version = version_num;
            user_ptr->infection_num = infection_num;

            num_users_left--;

            // Add all immediate connections to the queue; apprentice first
            for (auto& apprentice_ptr : user_ptr->apprentice_list) {
                bfs_queue.push(apprentice_ptr);
            }
            for (auto& master_ptr : user_ptr->master_list) {
                bfs_queue.push(master_ptr);
            }
        }
    }
}

// This version infects the number of users <= num_users. It provides the guarantee that
// all mentors and pupils in range are infected together or not at call
void User::limitedInfectionBounded (double version_num, int num_users) {
    // This version does not take negative users
    assert(num_users >= 0);

    std::queue<User*> bfs_queue, master_queue;

    // Random number used for this infection session
    unsigned int infection_num = rand();

    // Initialize number of users
    unsigned int num_users_left = num_users;

    // Add self first
    master_queue.push(this);

    while (!master_queue.empty()) {
        User* master_ptr = master_queue.front();
        master_queue.pop();

        // Check if this node has been visisted already
        if (master_ptr->infection_num == infection_num) {
            continue;
        }

        // Look ahead ot make sure that we have room for it.
        if (!(master_ptr->apprentice_list.size() + 1 <= num_users_left)) {
            continue;
        }

        // Add the master and all its pupils to the process list
        bfs_queue.push(master_ptr);
        for (auto& apprentice : master_ptr->apprentice_list) {
            bfs_queue.push(apprentice);
        }

        // Process everybody in the current process queue
        while (!bfs_queue.empty()) {
            User* user_ptr = bfs_queue.front();
            bfs_queue.pop();

            // Process only the ones we haven't
            if (user_ptr->infection_num != infection_num) {
                user_ptr->version = version_num;
                user_ptr->infection_num = infection_num;

                num_users_left--;

                // Add all immediate connections to the master queue
                for (auto& apprentice_ptr : user_ptr->apprentice_list) {
                    master_queue.push(apprentice_ptr);
                }
                for (auto& master_ptr : user_ptr->master_list) {
                    master_queue.push(master_ptr);
                }

            }
        }
    }
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
        for (auto& master_ptr : user_ptr->master_list) {
            bfs_queue.push(master_ptr);
        }
    }
}

void User::downwardInfection (double version_num) {
    // Breath first search upward using queue
    std::queue<User*> bfs_queue;
    for (auto& apprentice_ptr : apprentice_list) {
        bfs_queue.push(apprentice_ptr);
    }

    while (!bfs_queue.empty()) {
        User* user_ptr = bfs_queue.front();
        bfs_queue.pop();
        user_ptr->version = version_num;
        // Add all mentors to the list
        for (auto& apprentice_ptr : user_ptr->apprentice_list) {
            bfs_queue.push(apprentice_ptr);
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

    srand (time(NULL));

    // Initialize
    std::vector<User> user_list;
    for (int i = 0; i < 100; ++i) {
        User user (0.0);
        user.id = i;
        user_list.push_back(user);
    }

    // Check
    printf("Init:\n");
    for (int i = 0; i < 100; i += 10) {
        for (int j = 0; j < 10; ++j) {
            printf("%.1f   ", user_list[i + j].version);
        }
        printf("\n");
        printf("\n");
    }

    // Make connections
    for (int i = 0; i < 10; ++i) {
        make_connection(&(user_list[10]), &(user_list[i]));
    }

    for (int i = 20; i < 30; ++i) {
        make_connection(&(user_list[i]), &(user_list[10]));
    }

    for (int i = 10; i < 20; ++i) {
        make_connection(&(user_list[20]), &(user_list[i]));
    }

    // Do infection
    // user_list[10].limitedInfection(1.0, 15);
    user_list[10].limitedInfectionBounded(1.0, 19);

    // Check
    printf("Final:\n");
    for (int i = 0; i < 100; i += 10) {
        for (int j = 0; j < 10; ++j) {
            printf("%.1f   ", user_list[i + j].version);
        }
        printf("\n");
        printf("\n");
    }
    printf("==============================\n");
}
