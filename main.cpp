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

    int r0 = 0, diff = 0, afterCountT = 0;
    vector<int> heap;
    // put Process Length of first element into heap
    heap.push_back(proc[0].first);
    // make min heap
    make_heap(heap.begin(), heap.end(), greater<>{});

    for (int i = 1; i < n; i++) {
        if (proc[i].second > proc[i - 1].second) {
            diff = proc[i].second - proc[i - 1].second;
//            cout << diff << ", ";

            // root min > diff
            if (heap.front() > diff) {
                heap.front() -= diff;
            } else {
                // end time
                int previousT = proc[i - 1].second;

                while (diff > 0) {
                    int temp = heap.front();

                    if (diff >= temp) {
                        // delete root
                        pop_heap(heap.begin(), heap.end(), greater<>{});
                        heap.pop_back();
                        // diff
                        diff -= temp;
                        // ?
                        wt.push_back(temp + previousT);
                        previousT = temp + previousT;
                    } else {
                        heap.front() -= diff;
                        diff = 0;
                    }
                }

            }

        }

        // insert to heap
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

    int num =heap.size();
    for (int j = 0; j < num; j++) {
        // count time
        wt.push_back(afterCountT + heap.front());
        afterCountT += heap.front();
        // delete root
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

        clock_t start = clock();
        // SRPT
        vector<int> wt;
        wt = findWaitingTime(process, process.size(), wt);

        clock_t end = clock();
        double elapsed_secs = double(end - start);
        cout << "elapsed run time: " << elapsed_secs << " ms, ";

        int total = 0;
        // print process finished time
        for (int b = 0; b < wt.size(); b++) {
            total += wt[b];
        }
        cout << "Objective Value: " << total << endl;
    }


    return 0;
}
