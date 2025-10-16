/*
 * Problem 1: Parking Lot Management System
 *
 * Difficulty: Basic
 * Topic: Mutex
 *
 * PROBLEM DESCRIPTION:
 * ==================
 * You need to implement a parking lot management system that tracks available parking spaces.
 * Multiple entry gates try to assign parking spaces simultaneously, which can cause race conditions.
 *
 * REQUIREMENTS:
 * ============
 * 1. Initial parking capacity: 10 spaces
 * 2. Create 15 threads representing 15 cars trying to park
 * 3. Each thread should:
 *    - Check if parking space is available
 *    - If available, occupy one space (decrement available count)
 *    - Print success or failure message
 * 4. Use MUTEX to prevent race conditions
 * 5. Print final statistics (parked cars vs. rejected cars)
 *
 * EXPECTED OUTPUT:
 * ===============
 * - Exactly 10 cars should successfully park
 * - Exactly 5 cars should be rejected
 * - No race condition should occur
 *
 * HINTS:
 * =====
 * 1. Use CreateMutex() to create a mutex object
 * 2. Use WaitForSingleObject() before accessing shared variable (availableSpaces)
 * 3. Use ReleaseMutex() after modifying shared variable
 * 4. Don't forget to CloseHandle() for cleanup
 *
 * COMPILATION:
 * ===========
 * cl.exe 01-parking-lot.c
 */

#include <stdio.h>
#include <windows.h>

#define TOTAL_SPACES 10
#define TOTAL_CARS 15

// Shared resources
int availableSpaces = TOTAL_SPACES;
int parkedCars = 0;
int rejectedCars = 0;

// TODO: Add mutex declaration here
// API: HANDLE CreateMutex(LPSECURITY_ATTRIBUTES, BOOL bInitialOwner, LPCTSTR lpName);
HANDLE hMutex = NULL;


DWORD WINAPI CarThread(LPVOID lpParam) {
    int carId = (int)lpParam;

    Sleep(rand() % 100);  // Simulate random arrival time

    // TODO: Add mutex lock here to protect the critical section
    // API: WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
    DWORD result = WaitForSingleObject(hMutex, INFINITE);

    // Critical Section - Check and update parking spaces
    if (availableSpaces > 0) {
        availableSpaces--;
        parkedCars++;
        printf("[Car %2d] Successfully parked! (Available: %d)\n", carId, availableSpaces);
    } else {
        rejectedCars++;
        printf("[Car %2d] Parking full! (Available: %d)\n", carId, availableSpaces);
    }

    // TODO: Add mutex unlock here
    // API: ReleaseMutex(HANDLE hMutex);
    ReleaseMutex(hMutex);


    return 0;
}

int main() {
    printf("=== Parking Lot Management System ===\n");
    printf("Total Spaces: %d\n", TOTAL_SPACES);
    printf("Total Cars: %d\n\n", TOTAL_CARS);

    // TODO: Create mutex here
    // API: CreateMutex(NULL, FALSE, NULL);
    hMutex = CreateMutex(NULL, FALSE, NULL);

    HANDLE threads[TOTAL_CARS];

    // Create threads for each car
    for (int i = 0; i < TOTAL_CARS; i++) {
        threads[i] = CreateThread(NULL, 0, CarThread, (LPVOID)(i + 1), 0, NULL);
    }

    // Wait for all threads to complete
    WaitForMultipleObjects(TOTAL_CARS, threads, TRUE, INFINITE);

    // Print statistics
    printf("\n=================================\n");
    printf("Final Statistics:\n");
    printf("  Parked Cars: %d\n", parkedCars);
    printf("  Rejected Cars: %d\n", rejectedCars);
    printf("  Available Spaces: %d\n", availableSpaces);

    if (parkedCars == TOTAL_SPACES && rejectedCars == (TOTAL_CARS - TOTAL_SPACES)) {
        printf("\n SUCCESS: Correct synchronization!\n");
    } else {
        printf("\n FAILED: Race condition detected!\n");
    }

    // TODO: Cleanup - close all handles
    // API: CloseHandle(HANDLE hObject);
    CloseHandle(hMutex);


    return 0;
}
