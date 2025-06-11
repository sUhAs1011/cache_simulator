#include <iostream>
#include <unordered_map>
#include <bitset>
#include <iomanip>

using namespace std;

struct CacheLine{
    bool valid;
    int tag;
    bool dirtyBit;
};

class CacheSimulator{
public:
    int tagBits, indexBits, offsetBits, hits, misses, evictions;
    unordered_map<int, CacheLine> cache;
    CacheSimulator(int cacheSize, int blockSize, int memorySize){
        this->hits = 0;
        this->misses = 0;
        this->evictions = 0;

        this->indexBits = log2(cacheSize / blockSize);
        this->offsetBits = log2(blockSize);
        this->tagBits = log2(memorySize) - indexBits - offsetBits;

        for(int i = 0; i < cacheSize; i++){
            cache[i].valid = false;
            cache[i].dirtyBit = false;
        }
    }

    void accessMemory(int address){
        int tag = address >> (indexBits + offsetBits);
        int index = (address >> offsetBits) & ((1 << indexBits) - 1);
        int offset = address & ((1 << offsetBits) - 1);

        if(cache.count(index) && cache[index].valid && cache[index].tag == tag){
            cout << "Cache HIT";
            hits++;
        }
        else{
            cout << "Cache MISS ";
            misses++;
            if(cache.count(index) && cache[index].valid){
                cout << "-> Eviction";
                evictions++;
            }
            
            cache[index].valid = true;
            cache[index].tag = tag;
        }

        cout << endl << "Instruction Breakdown: " << endl;
        cout << "Tag bits:    " << bitset<16>(tag) << endl;
        cout << "Index bits:  " << bitset<16>(index) << endl;
        cout << "Offset bits: " << bitset<16>(offset) << endl << endl;
    }

    void updateMemory(int address){
        int tag = address >> (indexBits + offsetBits);
        int index = (address >> offsetBits) & ((1 << indexBits) - 1);
        int offset = address & ((1 << offsetBits) - 1);

        if(cache.count(index) && cache[index].valid && cache[index].tag == tag){
            cache[index].dirtyBit = true;
            cout << "Address is updated" << endl;
        }
        else
            cout << "Invalid, address is not present in cache." << endl;   
    }

    void printCacheTable(){
        cout << "Cache Table:" << endl;
        cout << "Index | Valid | Tag | Dirty Bit" << endl;
        for(const auto& entry : cache)
            cout << entry.first << " | " << (entry.second.valid ? "True" : "False") << " | " << bitset<32>(entry.second.tag) << " | " << (entry.second.dirtyBit ? "True" : "False") << endl;
    }

    void printStats(){
        cout << "Cache Statistics: " << endl;
        cout << "Hits:      " << hits << endl;
        cout << "Misses:    " << misses << endl;
        cout << "Evictions: " << evictions << endl << endl;

        cout << "Hit %:  " << setprecision(2) << (float) 100 * hits / (hits + misses) << endl;
        cout << "Miss %: " << setprecision(3) << (float) 100 * misses / (hits + misses) << endl;
    }

    void printDirtyBit(){
        // cout << "Dirty Bit Table:" << endl;
        // cout << "Index | Dirty Bit" << endl;
        // for(const auto& entry : cache)
        //     cout << entry.first << " | " << (entry.second.dirtyBit ? "True" : "False") << endl;
        cout << "Updated Cache Table:" << endl;
        cout << "Index | Valid | Tag | Dirty Bit" << endl;
        for(const auto& entry : cache)
            cout << entry.first << " | " << (entry.second.valid ? "True" : "False") << " | " << bitset<32>(entry.second.tag) << " | " << (entry.second.dirtyBit ? "True" : "False") << endl;
    }
};

int main() {
    int cacheSize, memorySize, offset, n, ins;

    cout << "NOTE: Please ensure that you enter all the SIZED in POWERS OF 2" << endl;
    cout << "Please enter the Cache Size: ";
    cin >> cacheSize;
    cout << "Please enter the Memory Size: ";
    cin >> memorySize;
    cout << "Please enter the Offset Bits: ";
    cin >> offset;

    if(cacheSize > memorySize){
         cout << "Invalid as Cache Size is greater than the Memory Size" << endl;
         exit(0);
    }

    CacheSimulator cache(cacheSize, pow(2, offset), memorySize);

    cout << "Please enter the number of instructions: ";
    cin >> n;

    for(int i = 0; i < n; i++){
        cout << "Please enter instruction " << i + 1 << " : ";
        cin >> hex >> ins;
        cache.accessMemory(ins);
    }

    cache.printCacheTable();
    cout << endl;
    cache.printStats();

    cout << endl << "Please enter the number of instructions you wish to update: ";
    cin >> n;
    for(int i = 0; i < n; i++){
        cout << "Please enter instruction " << i + 1 << " : ";
        cin >> hex >> ins;
        cache.updateMemory(ins);
    }

    cout << endl;
    cache.printDirtyBit();
    cout << endl;
    return 0;
}
