#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
using namespace std;

struct Process{
    int pid;
    int arrival;
    int burst;
    int priority;
    int waiting;
    int turnaround;
};

// ----- FCFS --------
void FCFS(vector<Process> p){
    sort(p.begin(), p.end(), [](Process a, Process b){
        return a.arrival < b.arrival;
    });

    int time = 0;
    int n = p.size();
    double totalWT = 0, totalTAT = 0;

    cout << "\nFCFS Scheduling\n";

    // ---- For Gantt Chart ----
    cout << "Gantt Chart:\n";
    for(auto &x : p){
        if(time < x.arrival)
            time = x.arrival;
        cout<< "P" << x.pid << " (" << time << "-";
        time += x.burst;
        cout << time << ") | ";
    }
    cout<< "\n\n";

    // ----- For Table -----
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";

    time = 0;
    for(auto &x : p){
        if (time < x.arrival)
            time = x.arrival;
        x.waiting = time - x.arrival;
        time += x.burst;
        x.turnaround = time - x.arrival;
        int completion = time;

        totalWT += x.waiting;
        totalTAT += x.turnaround;

        cout << x.pid << "\t"
             << x.arrival << "\t"
             << x.burst << "\t"
             << completion << "\t"
             << x.turnaround << "\t"
             << x.waiting << endl;
    }

    cout << "\nAverage Turnaround Time = " << totalTAT / n << endl;
    cout << "Average Waiting Time = " << totalWT / n << endl;
}

//--- SJF (Non-Preemptive) --
void SJF_NP(vector<Process> p){
    int n = p.size(), time = 0, done = 0;
    vector<bool> used(n, false);

    cout<< "\nSJF (Non-Preemptive)\nPID\tWT\tTAT\n";

    while(done < n){
        int idx = -1, mn = INT_MAX;
        for(int i = 0; i < n; i++){
            if(!used[i] && p[i].arrival <= time && p[i].burst < mn){
                mn = p[i].burst;
                idx = i;
            }
        }

        if(idx == -1){ time++; continue;}

        p[idx].waiting = time - p[idx].arrival;
        time += p[idx].burst;
        p[idx].turnaround = p[idx].waiting + p[idx].burst;
        used[idx] = true;
        done++;
        cout<< p[idx].pid << "\t" << p[idx].waiting << "\t" << p[idx].turnaround << endl;
    }
}

//------ Priority (Non-Preemptive) ---------- 
void Priority_NP(vector<Process> p){
    int n = p.size(), time = 0, done = 0;
    vector<bool> used(n, false);

    cout<< "\nPriority (Non-Preemptive)\nPID\tWT\tTAT\n";

    while(done < n){
        int idx = -1, best = INT_MAX;
        for(int i = 0; i < n; i++){
            if(!used[i] && p[i].arrival <= time && p[i].priority< best) {
                best = p[i].priority;
                idx = i;
            }
        }

        if(idx == -1){ time++; continue;}

        p[idx].waiting = time - p[idx].arrival;
        time += p[idx].burst;
        p[idx].turnaround = p[idx].waiting + p[idx].burst;
        used[idx] = true;
        done++;
        cout<< p[idx].pid << "\t" << p[idx].waiting << "\t" << p[idx].turnaround << endl;
    }
}

//----- Round Robin -------
void RoundRobin(vector<Process> p, int tq){
    int n = p.size();
    queue<int> q;
    vector<int> rem(n);
    vector<bool> inQueue(n, false);

    for(int i = 0; i < n; i++)
        rem[i] = p[i].burst;

    int time = 0;
    int completed = 0;
    q.push(0);
    inQueue[0] = true;
    
    cout<< "\nRound Robin (TQ = " << tq << ")\nPID\tWT\tTAT\n";

    while(completed < n){
        if(q.empty()){
            time++;
            for(int i = 0; i < n; i++){
                if(!inQueue[i] && rem[i] > 0 && p[i].arrival <= time){
                    q.push(i);
                    inQueue[i] = true;
                    break;
                }
            }
            continue;
        }

        int i = q.front();
        q.pop();

        int exec = min(tq, rem[i]);
        rem[i] -= exec;
        time += exec;

        for(int j = 0; j < n; j++){
            if(!inQueue[j] && rem[j] > 0 && p[j].arrival <= time){
                q.push(j);
                inQueue[j] = true;
            }
        }

        if(rem[i] > 0){
            q.push(i);
        } else{
            p[i].turnaround = time - p[i].arrival;
            p[i].waiting = p[i].turnaround - p[i].burst;
            completed++;

            cout << p[i].pid << "\t"
                 << p[i].waiting << "\t"
                 << p[i].turnaround << endl;
        }
    }
}

int main(){
    int mainOpt, subOpt;

    cout<< "1. Non-Preemptive\n2. Preemptive\nChoose: ";
    cin>> mainOpt;

    if(mainOpt == 1){
        cout<< "\n1. FCFS\n2. SJF\n3. Priority\nChoose: ";
        cin>> subOpt;
    } else{
        cout << "\n1. SJF (Preemptive)\n2. Priority (Preemptive)\n3. Round Robin\nChoose: ";
        cin>> subOpt;
    }

    int n;
    cout<< "\nEnter number of processes: ";
    cin>> n;

    vector<Process> processes;
    for(int i = 0; i < n; i++){
        Process p;
        p.pid = i + 1;
        cout << "Process " << p.pid << " (arrival, burst, priority): ";
        cin >> p.arrival >> p.burst >> p.priority;
        p.waiting = 0;
        p.turnaround = 0;
        processes.push_back(p);
    }

    cout<< "\n************* RESULT *************\n";

    if(mainOpt == 1){
        if(subOpt == 1) FCFS(processes);
        else if(subOpt == 2) SJF_NP(processes);
        else Priority_NP(processes);
    }else{
        if(subOpt == 3) RoundRobin(processes, 3);
        else cout << "Preemptive version not implemented.\n";
    }

    return 0;
}
