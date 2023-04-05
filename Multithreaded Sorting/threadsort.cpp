#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;

int array_section = 0;

void
thread_sort(vector<int>* v, size_t start, size_t finish) {
    qsort(v->data(), finish - start, sizeof((*v)[0]),
        [](const void* nums, const void* b) {
            int ia = *((int*)nums);
            int ib = *((int*)b);
            if (ia < ib) return -1;
            if (ia > ib) return 1;
            return 0;
        });
}

void merge(int low, int mid, int high, vector<int> nums) {
    int* left = new int[mid - low + 1];
    int* right = new int[high - mid];
    int n1 = mid - low + 1, n2 = high - mid, i, j;

    for (i = 0; i < n1; i++)
        left[i] = nums[i + low];
 
    for (i = 0; i < n2; i++)
        right[i] = nums[i + mid + 1];
 
    int k = low;
    i = j = 0;
 
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            nums[k++] = left[i++];
        else
            nums[k++] = right[j++];
    }
 
    while (i < n1) {
        nums[k++] = left[i++];
    }
 
    while (j < n2) {
        nums[k++] = right[j++];
    }
}

void merge_sort(int low, int high, vector<int> nums) {
    int mid = low + (high - low) / 2;
    if (low < high) {
 
        merge_sort(low, mid, nums);
        merge_sort(mid + 1, high, nums);
        merge(low, mid, high, nums);
    }
}

void merge_entry(vector<int>& nums) {
    int section = array_section++;
    int low = section * (20 / 4);
    int high = (section + 1) * (20 / 4) - 1;
    int mid = low + (high - low) / 2;

    if (low < high) {
        merge_sort(low, mid, nums);
        merge_sort(mid + 1, high, nums);
        merge(low, mid, high, nums);
    }
}

void test_sort(vector<int>& nums) {
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] > nums[i + 1]) {
            int temp = nums[i];
            nums[i] = nums[i + 1];
            nums[i + 1] = temp;
        }
    }
}

void
do_the_sort(vector<int>& nums) {
    // This is what you need to modify. This is so slow...
    // It's single threaded, we can do better, right?

    /*
    thread sorter(thread_sort, &nums, (size_t)0, nums.size());
    sorter.join();
    */

    // keeping an integer for how many threads are available
    //int available_threads = thread::hardware_concurrency();
    int available_threads = 4;

    // nums vector to hold all of the threads that will be created
    vector<thread> threads;

    if(available_threads < 1) {
        cout << "Something went wrong with hardware_concurrency call." << endl;
    }

    //test_sort(nums);

    // https://stackoverflow.com/questions/30768216/c-stdvector-of-independent-stdthreads 
    for (int i = 0; i < available_threads; i++) {
        threads.push_back(thread(merge_entry, nums));
    }

    for (auto &th : threads) {
        th.join();
    }

    // You should determine the number of cores available and
    // create nums thread for each available core. Each thread
    // should be given nums part of the array to work on.
    // C++, look at thread::hardware_concurrency()

    // Your main thread will need to join the results together
    // to finish the sorting process. Note: you're welcome
    // to use sort() (C++) or the qsort() function (C) in
    // your thread. This isn't nums data structures class.
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
    // while I'm debugging and not wanting to input nums size for the
    // array every time.
    const int SIZE_OF_ARRAY = 20;
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

    auto finish = chrono::high_resolution_clock::now();

    // little code to output the array and check
    for (auto &num : nums) {
        printf("%d  ", num);
    }
    cout << endl;

    if (!verifysorted(nums)) {
        cout << "Your array isn't sorted. Fast and wrong is wrong." << endl;
        return 1;
    }
 
    chrono::duration<double> elapsed = finish - start;

    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    return 0;
}
