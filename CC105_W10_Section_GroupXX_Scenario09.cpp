
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace chrono;

const int BASE_KEY = 190000;

int linearSearch(const vector<int>& data, int target, long long* probes = nullptr) {
    if (probes != nullptr)
        *probes = 0;

    for (int i = 0; i < static_cast<int>(data.size()); i++) {
        if (probes != nullptr)
            (*probes)++;

        if (data[i] == target)
            return i;
    }

    return -1;
}

int binarySearch(const vector<int>& data, int target, long long* probes = nullptr) {
    if (probes != nullptr)
        *probes = 0;

    int low = 0;
    int high = static_cast<int>(data.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (probes != nullptr)
            (*probes)++;

        if (data[mid] == target)
            return mid;

        if (data[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}

vector<int> generateData(int base, int size) {
    vector<int> data;
    data.reserve(size);

    for (int i = 0; i < size; i++)
        data.push_back(base + 2 * i);

    return data;
}

void displayData(const vector<int>& data) {
    for (size_t i = 0; i < data.size(); i++) {
        cout << data[i];

        if (i + 1 < data.size())
            cout << ", ";
    }

    cout << '\n';
}

void verificationTests(const vector<int>& data) {
    cout << "\n========== VERIFICATION TESTS ==========\n";

    int targets[] = {
        data[0],
        data[5],
        data[11],
        BASE_KEY + 1
    };

    string descriptions[] = {
        "Key at index 0",
        "Key at index 5",
        "Key at index 11",
        "Missing key (B + 1)"
    };

    int expected[] = {0, 5, 11, -1};

    for (int i = 0; i < 4; i++) {
        int linearResult = linearSearch(data, targets[i]);
        int binaryResult = binarySearch(data, targets[i]);

        cout << "\n" << descriptions[i] << '\n';
        cout << "Target: " << targets[i] << '\n';
        cout << "Expected Index: " << expected[i] << '\n';
        cout << "Linear Search: " << linearResult
             << (linearResult == expected[i] ? " [PASS]" : " [FAIL]") << '\n';
        cout << "Binary Search: " << binaryResult
             << (binaryResult == expected[i] ? " [PASS]" : " [FAIL]") << '\n';
    }

    vector<int> emptyData;

    cout << "\nEmpty Vector Test\n";
    cout << "Linear Search: "
         << linearSearch(emptyData, BASE_KEY) << " [EXPECTED -1]\n";
    cout << "Binary Search: "
         << binarySearch(emptyData, BASE_KEY) << " [EXPECTED -1]\n";
}

void scenarioTest(const vector<int>& data) {
    cout << "\n========== SCENARIO 9 ==========\n";
    cout << "Parcel Pickup Counter\n";
    cout << "Task: Verify parcel claim numbers.\n";

    int claims[] = {190020, 190023};

    for (int claim : claims) {
        int linearResult = linearSearch(data, claim);
        int binaryResult = binarySearch(data, claim);

        cout << "\nClaim Number: " << claim << '\n';
        cout << "Linear Search Index: " << linearResult << '\n';
        cout << "Binary Search Index: " << binaryResult << '\n';

        if (linearResult != -1)
            cout << "Status: Claim number found.\n";
        else
            cout << "Status: Claim number not found.\n";
    }

    cout << "\nPreceding key for 190020: 190018\n";
}

void shuffledTest(const vector<int>& data) {
    cout << "\n========== SHUFFLED DATA TEST ==========\n";

    vector<int> shuffled = data;

    random_device rd;
    mt19937 generator(rd());
    shuffle(shuffled.begin(), shuffled.end(), generator);

    cout << "Shuffled claim numbers:\n";
    displayData(shuffled);

    int target = 190010;
    int index = linearSearch(shuffled, target);

    cout << "\nTarget Claim: " << target << '\n';
    cout << "Linear Search Index in Shuffled Vector: " << index << '\n';

    if (index != -1)
        cout << "Claim verified successfully.\n";
    else
        cout << "Claim not found.\n";
}

void linearTrace(const vector<int>& data, int target) {
    cout << "\n========== LINEAR SEARCH TRACE ==========\n";
    cout << "Target: " << target << '\n';

    int result = -1;
    long long probes = 0;

    for (int i = 0; i < static_cast<int>(data.size()); i++) {
        probes++;

        cout << "Index: " << i
             << " | Key Examined: " << data[i];

        if (data[i] == target) {
            cout << " | FOUND\n";
            result = i;
            break;
        }

        cout << " | Not found\n";
    }

    cout << "Result Index: " << result << '\n';
    cout << "Total Probes: " << probes << '\n';
}

void binaryTrace(const vector<int>& data, int target) {
    cout << "\n========== BINARY SEARCH TRACE ==========\n";
    cout << "Target: " << target << '\n';

    int low = 0;
    int high = static_cast<int>(data.size()) - 1;
    int result = -1;
    long long probes = 0;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        probes++;

        cout << "Low: " << low
             << " | High: " << high
             << " | Mid: " << mid
             << " | Key at Mid: " << data[mid];

        if (data[mid] == target) {
            cout << " | FOUND\n";
            result = mid;
            break;
        }

        if (data[mid] < target) {
            cout << " | Search right half\n";
            low = mid + 1;
        }
        else {
            cout << " | Search left half\n";
            high = mid - 1;
        }
    }

    cout << "Result Index: " << result << '\n';
    cout << "Total Probes: " << probes << '\n';
}

double medianOfThree(double values[3]) {
    sort(values, values + 3);
    return values[1];
}

double benchmarkLinear(const vector<int>& data, int target) {
    const int repetitions = 100;
    double batchTimes[3];
    volatile int checksum = 0;

    for (int batch = 0; batch < 3; batch++) {
        auto start = steady_clock::now();

        for (int i = 0; i < repetitions; i++)
            checksum = checksum + linearSearch(data, target);

        auto stop = steady_clock::now();

        batchTimes[batch] =
            duration<double, nano>(stop - start).count() / repetitions;
    }

    return medianOfThree(batchTimes);
}

double benchmarkBinary(const vector<int>& data, int target) {
    const int repetitions = 100;
    double batchTimes[3];
    volatile int checksum = 0;

    for (int batch = 0; batch < 3; batch++) {
        auto start = steady_clock::now();

        for (int i = 0; i < repetitions; i++)
            checksum = checksum + binarySearch(data, target);

        auto stop = steady_clock::now();

        batchTimes[batch] =
            duration<double, nano>(stop - start).count() / repetitions;
    }

    return medianOfThree(batchTimes);
}

void benchmark(int base) {
    cout << "\n========== PERFORMANCE BENCHMARK ==========\n";
    cout << "Each batch: 100 searches | 3 batches | Median reported\n";

    vector<int> sizes = {1000, 10000, 100000, 1000000};

    cout << left
         << setw(12) << "Size"
         << setw(18) << "Target"
         << setw(16) << "Linear Probes"
         << setw(16) << "Binary Probes"
         << setw(20) << "Linear ns/search"
         << setw(20) << "Binary ns/search"
         << '\n';

    cout << string(102, '-') << '\n';

    for (int size : sizes) {
        vector<int> data = generateData(base, size);

        int hitTarget = data.back();
        int missTarget = base + 1;

        long long linearHitProbes = 0;
        long long binaryHitProbes = 0;
        long long linearMissProbes = 0;
        long long binaryMissProbes = 0;

        linearSearch(data, hitTarget, &linearHitProbes);
        binarySearch(data, hitTarget, &binaryHitProbes);

        linearSearch(data, missTarget, &linearMissProbes);
        binarySearch(data, missTarget, &binaryMissProbes);

        double linearHitTime = benchmarkLinear(data, hitTarget);
        double binaryHitTime = benchmarkBinary(data, hitTarget);

        double linearMissTime = benchmarkLinear(data, missTarget);
        double binaryMissTime = benchmarkBinary(data, missTarget);

        cout << fixed << setprecision(2);

        cout << left
             << setw(12) << size
             << setw(18) << "Last key (hit)"
             << setw(16) << linearHitProbes
             << setw(16) << binaryHitProbes
             << setw(20) << linearHitTime
             << setw(20) << binaryHitTime
             << '\n';

        cout << left
             << setw(12) << size
             << setw(18) << "Missing (miss)"
             << setw(16) << linearMissProbes
             << setw(16) << binaryMissProbes
             << setw(20) << linearMissTime
             << setw(20) << binaryMissTime
             << '\n';
    }
}

int main() {
    cout << "============================================\n";
    cout << " CC105 WEEK 10 - SEARCHING ALGORITHMS\n";
    cout << " SCENARIO 9: PARCEL PICKUP COUNTER\n";
    cout << "============================================\n";

    cout << "Base Key: " << BASE_KEY << '\n';

    vector<int> data = generateData(BASE_KEY, 12);

    cout << "\n12-KEY SORTED DATASET\n";
    displayData(data);

    verificationTests(data);
    scenarioTest(data);
    shuffledTest(data);

    int lastKey = data.back();

    linearTrace(data, lastKey);
    binaryTrace(data, lastKey);

    benchmark(BASE_KEY);

    cout << "\n========== PROGRAM COMPLETED ==========\n";

    return 0;
}
