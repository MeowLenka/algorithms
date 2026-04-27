#include "worker_db.hpp"
#include <cassert>
#include <iostream>


int test_worker_db() {
    WorkerDb db1;
    
    db1["Ivanov"] = WorkerData("Ivan", 34, 10.10);
    db1["Petrov"] = WorkerData("Petr", 43, 13.13);
    
    assert(db1["Ivanov"]._age == 34);
    assert(db1["Petrov"]._salary == 13.13);
    
    db1["Ivanov"] = WorkerData("Ivan", 42, 11.11);
    assert(db1["Ivanov"]._age == 42);
    

    WorkerDb db2;
    db2["A"] = WorkerData("A", 10, 100);
    db2["B"] = WorkerData("B", 20, 200);
    db2["C"] = WorkerData("C", 30, 300);
    
    int count = 0;
    for (auto it = db2.begin(); it != db2.end(); ++it) {
        count++;
    }
    assert(count == 3);
    
    assert(get_avg_age(db2) == 20);
    
    return 0;
}

int main() {
    std::cout << "\nTesting WorkerDb...\n";
    
    if (test_worker_db()) return 1;
    
    std::cout << "\nAll WorkerDb tests passed\n";
    return 0;
}