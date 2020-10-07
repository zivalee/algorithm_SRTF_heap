#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;

// heap
// 找小於+heapify (nlogn)
// 8, 23, 17, 56, 30, 36, 103, 74, 63, 85, 80, 122, 110, 112, 131, 139, 135, 144, 166, 156,

// first: Process Length (pj)
// second: Arrival Time (rj)
int findWaitingTime(vector<pair<int, int>> proc, int n, int total, vector<int> wt) {

    int r0 = 0, diff = 0, lastTime = 0;
    vector<int> heap;
    // put Process Length of first element into heap
    heap.push_back(proc[0].first);
    // todo min heap
    // make min heap
    make_heap(heap.begin(), heap.end(), greater<>{});

//    heap.push_back(10);
//    heap.push_back(6);
//    make_heap (heap.begin(),heap.end());
//    cout << heap.back() << endl;
//    heap.pop_back();
//    for(int j=0;j<heap.size();j++) {
//        cout << heap[j] << ", ";
//    }
//    heap.front()-=2;
//    cout << heap.front();

    for (int i = 1; i < n; i++) {
        if(proc[i].second>proc[i-1].second){
            diff = proc[i].second-proc[i-1].second;
            // root min > diff
            if(heap.front()>diff){
                heap.front()-=diff;
            }
            else{
                // end time
                int temp = heap.front();
                // delete root
                pop_heap(heap.begin(), heap.end(), greater<>{});
                heap.pop_back();
                // new root
                heap.front() = heap.front()-(diff-temp);
                // ?
                wt.push_back(temp+proc[i-1].second);
                total += temp+proc[i-1].second;
            };
        }
        heap.push_back(proc[i].first);
        push_heap(heap.begin(), heap.end(), greater<>{});
//        cout << endl;
//        for(int i=0;i<heap.size();i++){
//            cout << heap[i]<< ", ";
//        }
//        cout << endl;
    }
    for(int i=0;i<wt.size();i++){
        cout << wt[i]<< ", ";
    }
    return total;
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

    // execute SRPT
    cout << "There are k jobs...." << endl;
    for (int k = 20; k <= 20; k += 20) {
        vector<pair<int, int>> process;
        cout << "k = " << k << ", " << endl;

        // prepare data for SRPT
        for (int j = 0; j < k; j++) {
            // SRPT loop
            process.push_back({data[j].first, data[j].second});
            cout << data[j].second << ", ";
        }
        cout << endl;
        for (int j = 0; j < k; j++) {
            cout << data[j].first << ",  ";
        }
        cout << endl;

        int total = 0;
//        clock_t start = clock();
        // SRPT
        vector<int> wt;
        findWaitingTime(process, process.size(), total, wt);

//        clock_t end = clock();
//        double elapsed_secs = double(end - start);
//        cout << "elapsed run time: " << elapsed_secs << " ms, ";
//
//        // print process finished time
//        for (int b = 0; b < wt.size(); b++) {
//            total += wt[b];
//        }
//        cout << "Objective Value: " << total << endl;
    }


    return 0;
}
