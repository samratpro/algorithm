#include <iostream>
using namespace std;

int main() {
    int n, m; // n = number of processes, m = number of resources

    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resources: ";
    cin >> m;

    int alloc[n][m], max[n][m], need[n][m], avail[m];

    // Input Allocation Matrix
    cout << "\nEnter Allocation Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> alloc[i][j];
        }
    }

    // Input Max Matrix
    cout << "\nEnter Max Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> max[i][j];
        }
    }

    // Input Available Resources
    cout << "\nEnter Available Resources:\n";
    for (int i = 0; i < m; i++) {
        cin >> avail[i];
    }

    // Calculate Need Matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    bool finished[n] = {0};
    int safeSeq[n];
    int work[m];

    for (int i = 0; i < m; i++)
        work[i] = avail[i];

    int count = 0;

    while (count < n) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (!finished[i]) {
                int j;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > work[j])
                        break;
                }

                if (j == m) {
                    for (int k = 0; k < m; k++)
                        work[k] += alloc[i][k];

                    safeSeq[count++] = i;
                    finished[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            cout << "\nSystem is NOT in a safe state.\n";
            return 0;
        }
    }

    cout << "\nSystem is in a SAFE state.\nSafe Sequence: ";
    for (int i = 0; i < n; i++)
        cout << "P" << safeSeq[i] << " ";

    return 0;
}
