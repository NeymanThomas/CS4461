#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;

// class-wide variable to hold number of threads available
int THREAD_NO = 4;

// old sorting method
void
thread_sort(vector<int>* v, size_t start, size_t finish) {
    qsort(v->data() + start, finish - start, sizeof((*v)[0]),
        [](const void* nums, const void* b) {
            int ia = *((int*)nums);
            int ib = *((int*)b);
            if (ia < ib) return -1;
            if (ia > ib) return 1;
            return 0;
        });
}

/*
basically, the way my threaded sorting algorithm is working is
the vector of integers is divided into sections equal to the 
number of available threads. Each one of those sections is 
individually sorted, then after every section has been sorted,
a broad sort over the entire vector is then used at the end. 

So if there are 8 available threads, and the vector is 200 
elements long, this algorithm will be first fed thread 0 from
position 0 to 24 (200 divided by 8 threads), then the next thread
will recieve elements from 25 - 49 to sort and so on.

Made use of the partial_sort function from the standard library
*/
void my_thread_sort(vector<int>* nums, int start, int end) {
    partial_sort(nums->begin() + start, nums->begin() + end, nums->begin());
}


void
do_the_sort(vector<int>& nums) {

    // keeping an integer for how many threads are available
    THREAD_NO = thread::hardware_concurrency();
    if(THREAD_NO < 1) {
        cout << "Something went wrong with hardware_concurrency call." << endl;
    }

    printf("Number of threads being used for sorting: %d\n", THREAD_NO);

    // vector to hold all of the threads that will be created
    vector<thread> threads;

    // the multipler indicates the range of each of the
    // sections of the list that will be sorted
    int multiplier = nums.size() / THREAD_NO;

    // create each thread and add them to the thread vector list
    for (int i = 0; i < THREAD_NO; i++) {
        threads.push_back(thread(my_thread_sort, &nums, (i * multiplier), ((i + 1) * multiplier)));
    }
 
    // join all the threads created in the vector of threads
    for (auto &th : threads) {
        th.join();
    }

    // One final sort is necessary, because only sections of
    // the vector have been sorted independent from one another.
    // in theory, sorting the whole thing when most of sections
    // are already sorted -should- make it faster?
    //
    // I say in theory because testing this multithreaded version with
    // just the basic sort over the entire array doesn't actually
    // seem any faster.
    sort(nums.begin(), nums.end());
}

bool
verifysorted(const vector<int>& nums) {
    // Make sure the array really is sorted
    for (size_t i = 1; i < nums.size(); i++) {
        if (nums[i - 1] > nums[i])
            return false;
    }
    return true;
}

int main()
{
    // const variable to keep track of how big the array will be
    // while I'm debugging and not wanting to input a size for the
    // array every time.
    const int SIZE_OF_ARRAY = 10000000;
    vector<int> nums;

    /*
    int x;
    while (cin >> x)
        nums.push_back(x);
    */

    // auto creates the array without input
    // rand() is apparently not random either, considering it
    // creates the same array every time...
    for (int i = 0; i < SIZE_OF_ARRAY; i++) {
        nums.push_back(rand() % SIZE_OF_ARRAY);
    }

    auto start = chrono::high_resolution_clock::now();

    do_the_sort(nums);
    //To compare it to just a basic sort
    //sort(nums.begin(), nums.end());

    auto finish = chrono::high_resolution_clock::now();

    // little code to output the array and check
    /*
    for (auto &num : nums) {
        printf("%d  ", num);
    }
    cout << endl;
    */

    if (!verifysorted(nums)) {
        cout << "Your array isn't sorted. Fast and wrong is wrong." << endl;
        return 1;
    }
 
    chrono::duration<double> elapsed = finish - start;

    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    return 0;
}
