#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp> // Common file
#include <ext/pb_ds/tree_policy.hpp> // Including tree_order_statistics_node_update
#define IO ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)

using namespace std;
using namespace __gnu_pbds;
using lli = long long int;

template<typename key> using ordered_set = tree <key, null_type, less<key>, rb_tree_tag, tree_order_statistics_node_update>;

int pages;
string method;

//retrieve needed variables for printing the FIFO method output
void viewFIFO(auto ee, int pf) {
    cout << "Replacement Policy = FIFO" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Page   Content of Frames" << endl;
    cout << "----   -----------------" << endl;
    //priting validations, making sure if either print 0 if the current process number is less tahn 10
    //if occured a page fault, then we will print F
    //process all inputs and print as whats needed
    for(int i = 0; i < (int) ee.size(); i++) {
        pair< int, pair< bool, vector< int > > > e = ee[i];
        if(e.second.first) {
            if(e.first < 10) {
                cout << "0" << e.first << " " << "F" << "   ";
            } else {
                cout << e.first << " " << "F" << "   ";
            }
        } else {
            if(e.first < 10) {
                cout << "0" << e.first << "     ";
            } else {
                cout << e.first << "     ";
            }
        }
        //printing the current data stored in the buffer for the corresponding input that was inserted at the time
        vector<int> aux = e.second.second;
        for(int i = 1; i < aux.size(); i++) {
            if(aux[i] < 10)
                cout << "0" << aux[i] << " ";
            else
                cout << aux[i] << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------------"<< endl;
    cout << "Number of page faults = " << pf << endl;
}

void _fifo() {
    int pf = 0;
    //used for printing purpose
    //as at first, I thought I must process all input first and then print all output at once
    vector< pair< int, pair< bool, vector< int > > > > ans;
    vector<int> arr;
    //1-indexed, so just insert a garbage value that will not be needed
    arr.push_back(INT_MIN);
    //stores current elements in the buffer to look-up for them in O(1)
    unordered_set<int> current;
    //mostOcc: points to the index that should be replaced with new pricess/input if needed
    //i: points to the place to insert new element to the buffer at if there still exist place in the buffer
    int mostOcc = 1, i = 1;
    int prog = 0;
    while(1) {
        cin >> prog;
        //check if all input processed and we finished
        if(prog == -1)
            break;
        //if its already in the buffer
        if(current.count(prog)) {
            /*put thus program/file number, 0 marks as there is no page fault occured,
            arr is current view of the buffer for printing*/
            ans.push_back({prog, {0, arr}});
            continue;
        }
        //insert this new element with us to look-up for it in O(1) if needed
        current.insert(prog);
        //reached max buffer size and can not insert new input to the buffer
        if(i == pages + 1) {
            //remove the mostOccured element from the buffer pointed to by mostOcc
            current.erase(current.find(arr[mostOcc]));
            //insert the new file/input to its correct place in the buffer
            arr[mostOcc] = prog;
            mostOcc++;
            //make sure mostOcc is in a valid range
            if(mostOcc == pages + 1)
                mostOcc = 1;
            /*put thus program/file number, 1 marks as there is a page fault occured,
            arr is current view of the buffer for printing*/
            ans.push_back({prog, {1, arr}});
            //increment the page fault counter
            pf++;
        }
        //there is still extra empty places in the buffer to insert the newly needed process
        else {
            arr.push_back(prog);
            i++;
            /*put thus program/file number, 0 marks as there is no page fault occured,
            arr is current view of the buffer for printing*/
            ans.push_back({prog, {0, arr}});
        }
    }
    //go and print the data as needed
    viewFIFO(ans, pf);
}

void _lru() {
    cout << "Replacement Policy = LRU" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Page   Content of Frames" << endl;
    cout << "----   -----------------" << endl;
    //stored input that are in the buffer but will not be used in the future ( i.e NOT on my right side )
    unordered_set< int > current;
    //current buffer to store the data/inputs number/name, and stores with it its time sitting in the buffer
    vector< pair< int, int > > arr(pages + 1);
    //pf: number of page faults
    //i: last index which I inserted to in the buffer
    int pf = 0, i = 1;
    int proc = 0;
    //to check if page fault occured while processing the current input
    bool f;
    while(1) {
        cin >> proc;
        if(proc == -1)
            break;
        f = false;
        //if current process is already in the buffer used
        if(current.count(proc)) {
            //reset its time as this means it has been used now
            //as time increase, this means this input is not used too much
            for(int j = 1; j <= pages; j++) {
                if(arr[j].first == proc) {
                    arr[j] = {proc, 0};
                }
            }
        }
        //if its new element
        else {
            //if there is no empty place in the buffer
            if(i == pages + 1) {
                int maxIndex = -1, maxV = 0;
                for(int j = 1; j <= pages; j++) {
                    if(arr[j].second > maxV) {
                        maxIndex = j;
                        maxV = arr[j].second;
                    }
                }
                f = true;
                current.erase(current.find(arr[maxIndex].first));
                current.insert(proc);
                arr[maxIndex] = {proc, 0};
                pf++;
            }
            //insert this new element to the buffer and set its used time to zero, as its least one stayed in the buffer
            //so as its last active one used
            else {
                arr[i++] = {proc, 0};
                current.insert(proc);
            }
        }
        //increase the time of all other process that are in the buffer and not same as newly inserted file/input
        for(int j = 1; j < i; j++) {
            if(arr[j].first != proc) {
                arr[j].second++;
            }
        }
        //priting validations, making sure if either print 0 if the current process number is less tahn 10
        //if occured a page fault, then we will print F
        if(proc < 10) {
            cout << "0" << proc;
            if(f)
                cout << " " << "F" << "   ";
            else
                cout << "     ";
        }
        else {
            cout << proc;
            if(f)
                cout << " " << "F" << "   ";
            else
                cout << "     ";
        }
        //printing the current data stored in the buffer
        for(int j = 1; j < i; j++) {
            if(arr[j].first < 10)
                cout << "0" << arr[j].first << " ";
            else
                cout << arr[j].first << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl;
    cout << "Number of page faults = " << pf << endl;
}

void _optimal() {
    cout << "Replacement Policy = OPTIMAL" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Page   Content of Frames" << endl;
    cout << "----   -----------------" << endl;
    //elements: store the input which will be processed
    //arr: current buffer
    vector< int > elements(1), arr(pages + 1); // 1-indexed
    //contains the current stored files/data from the input to lock-up them in O(1)
    unordered_set< int > current;
    int proc = 0;
    // take the whole inputat once as its needed for the optimal algorithm
    while(1) {
        cin >> proc;
        if(proc == -1)
            break;
        elements.push_back(proc);
    }
    //i: last index which I inserted to in the buffer
    //pf: number of page faults
    int i = 1, pf = 0;
    //if a page fault occured
    bool _override;
    //run and process all the input
    for(int k = 1; k < elements.size(); k++) {
        //current input to be process saveit to proc
        proc = elements[k];
        //still no page fault occured
        _override = false;
        //if this process not saved/used in the buffer
        if(!current.count(proc)) {
            //if there is no extra place in the buffer to add this new file/input
            if(i == pages + 1) {
                //pag fault occured
                _override = true;
                //aux: stores the files that are in the buffer currently, and also will be used again inthe future
                vector< pair< int, int > > aux;
                //stored input that are in the buffer but will not be used in the future ( i.e NOT on my right side )
                unordered_set< int > notFound;
                for(int notFoundIter = 1; notFoundIter <= pages; notFoundIter++)
                    notFound.insert(arr[notFoundIter]);
                for(int ele = 1; ele <= pages; ele++) {
                    for(int j = k + 1; j < elements.size(); j++) {
                        //if current file occures to be found also on my right( i.e to be used in the future )
                        if(arr[ele] == elements[j]) {
                            //erase it, as we know are sure it will be used later
                            notFound.erase(notFound.find(arr[ele]));
                            aux.push_back({j, arr[ele]});
                            break;
                        }
                    }
                }
                //if all of the elements in the buffer will be used again in the future
                if(!notFound.size()) {
                    //sort the aux according to their positions and get for me tha maximum/farthest one away from me
                    sort(aux.rbegin(), aux.rend());
                    //the element/file that will be removed and swapped by the new input
                    int toSwap = aux[0].second;
                    int ii;
                    //get the index of the element that will be removed
                    for(ii = 1; ii <= pages; ii++) {
                        if(arr[ii] == toSwap)
                            break;
                    }
                    //remove this element as we will not need to look-up for it again
                    //insert the new elemnt with us
                    current.erase(current.find(toSwap));
                    current.insert(proc);
                    //store the new element in the buffer at position of removed one
                    arr[ii] = proc;
                }
                //at least one of the elements in the buffer is not presenton my right, so it will not be used again
                else {
                    //get the first one in the buffer that will not be used in the future, and replace it with the new process
                    int j;
                    for(j = 1; j <= pages; j++) {
                        if(notFound.count(arr[j]))
                            break;
                    }
                    current.erase(current.find(arr[j]));
                    current.insert(proc);
                    arr[j] = proc;
                }
                //increment as a new page fault occured
                pf++;
            }
            //there is an extra space, add this new input to the buffer
            else {
                arr[i++] = proc;
                //insert this process with us to look-up for it if needed in O(1)
                current.insert(proc);
            }
        }
        //priting validations, making sure if either print 0 if the current process number is less tahn 10
        //if occured a page fault, then we will print F
        if(proc < 10) {
            cout << "0" << proc;
            if(_override)
                cout << " " << "F" << "   ";
            else
                cout << "     ";
        }
        else {
            cout << proc;
            if(_override)
                cout << " " << "F" << "   ";
            else
                cout << "     ";
        }
        //printing the current data stored in the buffer
        for(int iter = 1; iter < i; iter++) {
            if(arr[iter] < 10)
                cout << "0" << arr[iter] << " ";
            else
                cout << arr[iter] << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl;
    cout << "Number of page faults = " << pf << endl;
}

void _clock() {
    cout << "Replacement Policy = CLOCK" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Page   Content of Frames" << endl;
    cout << "----   -----------------" << endl;
    //contains the current stored files/data from the input to lock-up them in O(1)
    unordered_set< int > current;
    //arr: main buffer that stores/holds teh current used files
    //use_bit: holds the current bit state of the corresponding buffer
    vector< int > arr(pages + 1), use_bit(pages + 1); // using 1-indexed
    for(int i = 0; i <= pages; i++) {
        arr[i] = 0; // initialize buffer
        use_bit[i] = 1; // initialize use_bit
    }
    //proc: current input new data from the input to be processed
    //i: indicate the last index in the buffer for the last stored
    //pf: count for page faults
    //mostOccur: pointer that points to the next place in the buffer which should be replaced if needed
    int proc = 0, i = 1, pf = 0, mostOccur = 1;
    bool fault; // indicate whether a page fault occured or not
    while(1) {
        cin >> proc;
        fault = false;
        if(proc == -1)
            break;
        //if its already used in the buffer
        if(current.count(proc)) {
            int j;
            for(j = 1; j <= pages; j++) {
                if(arr[j] == proc)
                    break;
            }
            //get the position of this proc in the buffer and set ots use_bit
            use_bit[j] = 1;
        }
        else {
            //insert the new proc/input with us to the buffer
            current.insert(proc);
            //if the buffr s now maximum/full
            if(i == pages + 1) {
                //page fault occured
                fault = true;
                pf++;
                //gte the first zero use_bit so as to get replaced with the new process/input
                while(use_bit[mostOccur]) {
                    use_bit[mostOccur] = 0;
                    mostOccur++;
                    //make sure mostOccur is in a valid range
                    if(mostOccur == pages + 1)
                        mostOccur = 1;
                }
                //remove the chosen file from the buffer in O(1)
                current.erase(current.find(arr[mostOccur]));
                //set the new process to its position in the buffer and set ots use_bit
                use_bit[mostOccur] = 1;
                arr[mostOccur] = proc;
                mostOccur++;
                if(mostOccur == pages + 1)
                    mostOccur = 1;
            }
            //there is still more empty places in the buffer just add the new input to the buffer and set its use_bit
            else {
                arr[i] = proc;
                use_bit[i++] = 1;
            }
        }
        //priting validations, making sure if either print 0 if the current process number is less tahn 10
        //if occured a page fault, then we will print F
        if(proc < 10) {
            cout << "0" << proc;
            if(fault)
                cout << " " << "F" << "   ";
            else
                cout << "     ";
        }
        else {
            cout << proc;
            if(fault)
                cout << " " << "F" << "   ";
            else
                cout << "     ";
        }
        //printing the current data stored in the buffer
        for(int j = 1; j < i; j++) {
            if(arr[j] < 10)
                cout << "0" << arr[j] << " ";
            else
                cout << arr[j] << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl;
    cout << "Number of page faults = " << pf << endl;
}

int main () {
    IO;
    cin >> pages >> method;
    // validate a certain form for me to check validinput method and its one of the four types provided
    transform(method.begin(), method.end(), method.begin(), ::toupper);
    if(method == "FIFO") {
        _fifo();
    }
    else if(method == "LRU") {
        _lru();
    }
    else if(method == "OPTIMAL") {
        _optimal();
    }
    else if(method == "CLOCK") {
        _clock();
    }
    return 0;
}
