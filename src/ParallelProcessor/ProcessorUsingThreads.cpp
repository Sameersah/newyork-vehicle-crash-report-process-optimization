#include "ProcessorUsingThreads.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread>
#include <mutex>
#include <omp.h>

void ProcessorUsingThreads::loadData(const std::string& filename) {
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header

    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    int num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads for parallel data loading.\n";

    #pragma omp parallel for num_threads(num_threads)
    for (size_t i = 0; i < lines.size(); i++) {
        std::istringstream ss(lines[i]);
        std::string token;
        CrashRecord record;

        std::getline(ss, record.crash_date, ',');
        std::getline(ss, record.crash_time, ',');
        std::getline(ss, record.borough, ',');
        std::getline(ss, record.zip_code, ',');

        std::getline(ss, token, ',');
        record.latitude = token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        record.longitude = token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, record.location, ',');
        std::getline(ss, record.on_street_name, ',');
        std::getline(ss, record.cross_street_name, ',');
        std::getline(ss, record.off_street_name, ',');

        std::getline(ss, token, ',');
        try { record.persons_injured = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.persons_injured = 0; }

        try { record.cyclists_injured = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.cyclists_injured = 0; }

        try { record.cyclists_killed = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.cyclists_killed = 0; }

        try { record.motorists_injured = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.motorists_injured = 0; }

        try { record.motorists_killed = token.empty() ? 0 : std::stoi(token); }
        catch (const std::exception&) { record.motorists_killed = 0; }

        std::getline(ss, record.contributing_factor_vehicle_1, ',');
        std::getline(ss, record.contributing_factor_vehicle_2, ',');
        std::getline(ss, record.contributing_factor_vehicle_3, ',');
        std::getline(ss, record.contributing_factor_vehicle_4, ',');
        std::getline(ss, record.contributing_factor_vehicle_5, ',');

        try { record.collision_id= token.empty() ? 0 : std::stol(token); }
        catch (const std::exception&) { record.collision_id = 0; }
        std::getline(ss, record.vehicle_type_code_1, ',');
        std::getline(ss, record.vehicle_type_code_2, ',');
        std::getline(ss, record.vehicle_type_code_3, ',');
        std::getline(ss, record.vehicle_type_code_4, ',');
        std::getline(ss, record.vehicle_type_code_5, ',');

        #pragma omp critical
        {
            records.push_back(record);
            // dateIndex[record.crash_date].push_back(record);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    data_load_duration = end - start;
    std::cout << "Parallel Data Load Time: " << data_load_duration.count() << " seconds\n";
}

// Multi-threaded search for date range
int ProcessorUsingThreads::getCrashesInDateRange(const std::string& start_date, const std::string& end_date) {
    auto start = std::chrono::high_resolution_clock::now();
    int crash_count = 0;

    #pragma omp parallel for reduction(+:crash_count)
    for (size_t i = 0; i < records.size(); i++) {
        if (records[i].crash_date >= start_date && records[i].crash_date <= end_date) {
            crash_count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    date_range_Searching_duration = end - start;
    return crash_count;
}

// Multi-threaded search for injury count range
int ProcessorUsingThreads::getCrashesByInjuryCountRange(int min_injuries, int max_injuries) {
    auto start = std::chrono::high_resolution_clock::now();
    int crash_count = 0;

    #pragma omp parallel for reduction(+:crash_count)
    for (size_t i = 0; i < records.size(); i++) {
        if (records[i].persons_injured >= min_injuries && records[i].persons_injured <= max_injuries) {
            crash_count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    injury_range_Searching_duration = end - start;
    return crash_count;
}

// Multi-threaded search for location range
int ProcessorUsingThreads::getCrashesByLocationRange(float lat, float lon, float radius) {
    auto start = std::chrono::high_resolution_clock::now();
    int crash_count = 0;

    #pragma omp parallel for reduction(+:crash_count)
    for (size_t i = 0; i < records.size(); i++) {
        float dist = std::sqrt(std::pow(records[i].latitude - lat, 2) + std::pow(records[i].longitude - lon, 2));
        if (dist <= radius) {
            crash_count++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    location_range_Searching_duration = end - start;
    return crash_count;
}
