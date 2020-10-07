#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// heap
// 找小於+heapify (nlogn)

// first: Process Length (pj)
// second: Arrival Time (rj)
vector<int> findWaitingTime(vector<pair<int, int>> proc, int n, vector<int> wt) {

    int diff = 0, afterCountT = 0;
    vector<int> heap;
    // put Process Length of first element into heap
    heap.push_back(proc[0].first);
    // make min heap
    make_heap(heap.begin(), heap.end(), greater<>{});

    // adding process length of elements into heap
    // examine every element's arrival time and adjust the heap before putting the process length into heap
    for (int i = 1; i < n; i++) {
        // arrival time is in increasing order
        // if arrival time of job i is equal to i-1, put the process length of it into heap directly
        // if not, adjust heap then put the length into heap
        if (proc[i].second > proc[i - 1].second) {
            // time passed between two job
            diff = proc[i].second - proc[i - 1].second;

            // if the time passed can't even finished one job
            // root of heap (min job length) > time passed
            if (heap.front() > diff) {
                heap.front() -= diff;
            } else {
                // if the time passed can finish more than one job
                // arrival time of the previous job
                int previousT = proc[i - 1].second;

                while (diff > 0) {
                    int temp = heap.front();

                    if (diff >= temp) {
                        // if can finish more jobs
                        // delete root
                        pop_heap(heap.begin(), heap.end(), greater<>{});
                        heap.pop_back();
                        // update time passed (diff)
                        diff -= temp;
                        // count job finished time
                        wt.push_back(temp + previousT);
                        // update previous job finished time
                        previousT = temp + previousT;
                    } else {
                        // if no time left
                        heap.front() -= diff;
                        diff = 0;
                    }
                }
            }
        }

        // insert job length to heap
        heap.push_back(proc[i].first);
        push_heap(heap.begin(), heap.end(), greater<>{});

        // print heap
//        for (int i = 0; i < heap.size(); i++) {
//            cout << heap[i] << ", ";
//        }

        // save last job arrive time for later counting
        if (i == n - 1) {
            afterCountT = proc[i].second;
        }

    }

//    cout << "afterCountT "<< afterCountT << endl;

    // after all process length are put into heap
    // count the finished time of the remaining jobs in heap
    int num =heap.size();
    for (int j = 0; j < num; j++) {
        // count time
        wt.push_back(afterCountT + heap.front());
        afterCountT += heap.front();
        // delete the counted root min
        pop_heap(heap.begin(), heap.end(), greater<>{});
        heap.pop_back();
    }

    return wt;
}

int main() {
    // read csv
    vector<pair<int, int>> data;
    ifstream myFile("/home/ziva/CLionProjects/C++/SRPT/test_instance.csv");

    // Make sure the file is open
    if (!myFile.is_open()) throw runtime_error("Could not open file");

    // helper vars
    string line, colname;
    int tempCol = 0;
    // read first line (the column names)
    if (myFile.good()) {
        getline(myFile, line);

        // create a stringstream from line
        stringstream ss(line);

        // extract each column name
        while (getline(ss, colname, ',')) {
            // initialize and add <colname, int> pairs to data
            stringstream colnameS(colname);
            colnameS >> tempCol;
            data.push_back({tempCol, 0});
        }
    }

    while (getline(myFile, line)) {
        stringstream ss(line);
        string val;

        int colIdx = 0;
        int temp = 0;

        while (getline(ss, val, ',')) {
            // convert string to int
            stringstream valS(val);
            valS >> temp;

            data[colIdx].second = temp;
            colIdx++;
        }
    }
    myFile.close();

    // delete first column r_j, p_j
    data.erase(data.begin());

    // todo sort by Arrival Time (second param)?
    vector<pair<int, int>> rawData = data;

    // execute heap SRPT
    cout << "There are k jobs...." << endl;
    for (int k = 20; k <= 100; k += 20) {
        vector<pair<int, int>> process;
        cout << "k = " << k << ", ";

        // prepare data for SRPT
        for (int j = 0; j < k; j++) {
            // SRPT loop
            process.push_back({data[j].first, data[j].second});
//            cout << data[j].second << ", ";
        }
//        cout << endl;
//        for (int j = 0; j < k; j++) {
//            cout << data[j].first << ",  ";
//        }

        // SRPT
        clock_t start = clock();
        vector<int> wt;
        wt = findWaitingTime(process, process.size(), wt);
        clock_t end = clock();

        // runtime
        double elapsed_secs = double(end - start);
        cout << "elapsed run time: " << elapsed_secs << " ms, ";

        // count process finished time
        int total = 0;
        for (int b = 0; b < wt.size(); b++) {
            total += wt[b];
        }
        cout << "Objective Value: " << total << endl;
    }

    return 0;
}
