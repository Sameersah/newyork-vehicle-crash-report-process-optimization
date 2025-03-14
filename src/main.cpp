#include <iostream>
#include "SequentialProcessor/Experiment1IfStream/ProcessorUsingIfStream.h"
#include "SequentialProcessor/Experiment2BufferRead/ProcessorUsingBufferedFileRead.h"
#include "SequentialProcessor/Experiment3BufferReadVectorReserve/ProcessorUsingBufferedFileReadVectorReserve.h"
#include "ParallelProcessor/Experiment1MultiThreads/ProcessorUsingThreads.h"
#include "ParallelProcessor/Experiment2BufferedRead/ParallelBufferRead.h"
#include "ParallelProcessor/Experiment3BufferReadVectorReserve/ParallelVectorReserve.h"
#include "OptimalProcessor/Experiment1ObjectofArrays/OptimalProcessorUsingThreads.h"
#include "OptimalProcessor/Experiment2BufferRead/OptimalBufferRead.h"
#include "OptimalProcessor/Experiment3BufferReadVectorReserve/OptimalVectorReserve.h"
#include "OptimalProcessor/Experiment4BufferReadVectorReserveThreadLocalBuffer/ProcessorUsingThreadLocalBuffer.h"
#include "OptimalProcessor/Experiment5BufferReadVectorReserveThreadLocalPartialRead/ProcessorUsingPartialRead.h"
#include "OptimalProcessor/Experiment6BufferReadVectorReserveThreadLocalPartialRead/ProcessorUsingEpochTime.h"


void runProcessor(std::unique_ptr<ICrashDataProcessor>& processor) {
    std::string filename = "../motor_vehicle_collisions.csv";

    std::cout << "Loading data..." << std::endl;
    processor->loadData(filename);
    std::cout << "Data load duration: " << processor->getDataLoadDuration().count() << " seconds" << std::endl;

    std::string start_date, end_date;
    std::cout << "Enter start date (MM/DD/YYYY): ";
    std::cin >> start_date;
    std::cout << "Enter end date (MM/DD/YYYY): ";
    std::cin >> end_date;

    int total_crashes = processor->getCrashesInDateRange(start_date, end_date);
    std::cout << "Number of crashes in date range: " << total_crashes << std::endl;
    std::cout << "Date range searching duration: " << processor->getDateRangeSearchingDuration().count() << " seconds" << std::endl;

    int min_injuries, max_injuries;
    std::cout << "Enter minimum injuries: ";
    std::cin >> min_injuries;
    std::cout << "Enter maximum injuries: ";
    std::cin >> max_injuries;

    total_crashes = processor->getCrashesByInjuryCountRange(min_injuries, max_injuries);
    std::cout << "Number of crashes in injury count range: " << total_crashes << std::endl;
    std::cout << "Injury count range searching duration: " << processor->getInjuryRangeSearchingDuration().count() << " seconds" << std::endl;

    float lat, lon, radius;
    std::cout << "Enter latitude: ";
    std::cin >> lat;
    std::cout << "Enter longitude: ";
    std::cin >> lon;
    std::cout << "Enter radius (km): ";
    std::cin >> radius;

    total_crashes = processor->getCrashesByLocationRange(lat, lon, radius);
    std::cout << "Number of crashes in location range: " << total_crashes << std::endl;
    std::cout << "Location range searching duration: " << processor->getLocationRangeSearchingDuration().count() << " seconds" << std::endl;
}

int main() {
    while (true) {
        std::cout << "\n=============== Crash Data Processing ===============\n";
        std::cout << "1. Single Thread Processor- ifstream\n";
        std::cout << "2. Single Thread Processor- Buffered File Read\n";
        std::cout << "3. Single Thread Processor- Buffered File Read and Vector memory reserve\n";
        std::cout << "4. Multi thread Processor\n";
        std::cout << "5. Multi thread Processor- Buffered File Read\n";
        std::cout << "6. Multi thread Processor- Buffered File Read and Vector memory reserve\n";
        std::cout << "7. Optimized Multi Thread Processor - object of Arrays\n";
        std::cout << "8. Optimized Multi Thread Processor - Object of Arrays, Buffer file Read\n";
        std::cout << "9. Optimized Multi Thread Processor- Object of Arrays, buffer file read and vector memory reserve\n";
        std::cout << "10. Optimized Multi Thread Processor- Object of Arrays, buffer file read, vector memory reserve "
                     "and thread local buffer\n";
        std::cout << "11. Optimized Multi Thread Processor- Object of Arrays, buffer file read, vector memory reserve "
                     ",thread local buffer and partial file read\n";
        std::cout << "12. Optimized Multi Thread Processor- Object of Arrays, buffer file read, vector memory reserve "
                     ",thread local buffer, partial file read and epoch time\n";
        std::cout << "13. Exit\n";
        std::cout << "=====================================================\n";
        std::cout << "Select processing method: ";

        int choice;
        std::cin >> choice;

        if (choice == 13) {
            std::cout << "Exiting program. Goodbye!\n";
            break;
        }

        std::unique_ptr<ICrashDataProcessor> processor;

        switch (choice) {
            case 1:
                std::cout << "\nUsing Single Threaded Processing (ifstream)...\n";
                processor = std::make_unique<ProcessorUsingIfStream>();
                runProcessor(processor);
                break;

            case 2:
                std::cout << "\nUsing Buffered File Read Processing...\n";
                processor = std::make_unique<ProcessorUsingBufferedFileRead>();
                runProcessor(processor);
                break;

            case 3:
                std::cout << "\nUsing Buffered File Read and Vector reserve Processing...\n";
               processor = std::make_unique<ProcessorUsingBufferedFileReadVectorReserve>();
               runProcessor(processor);
               break;

            case 4:
                std::cout << "\nUsing Multi-threaded Parsing...\n";
                processor = std::make_unique<ProcessorUsingThreads>();
                runProcessor(processor);
                break;

            case 5:
                std::cout << "\nUsing Multi-threaded Buffered File Read Parsing...\n";
                processor = std::make_unique<ProcessorUsingBufferedFileReadThreads>();
                runProcessor(processor);
                break;

            case 6:
                std::cout << "\nUsing Multi-threaded Buffered File Read and Vector reserve Parsing...\n";
                processor = std::make_unique<ProcessorUsingBufferedFileReadVectorReserveThreads>();
                runProcessor(processor);
                break;


            case 7:
                std::cout << "\nOptimized Multi Thread search...\n";
                processor = std::make_unique<OptimalProcessorUsingThreads>();
                runProcessor(processor);
                break;
            
            case 8:
                std::cout << "\nOptimized Multi Thread using buffer search...\n";
                processor = std::make_unique<OptimalBufferRead>();
                runProcessor(processor);
                break;
            
            case 9:
                std::cout << "\nOptimized Multi Thread using buffer file read and vector memory reserve...\n";
                processor = std::make_unique<OptimalVectorReserve>();
                runProcessor(processor);
                break;

            case 10:
                std::cout << "\nOptimized Multi Thread Processor- Object of Arrays, buffer file read,vector memory reserve "
                     "and thread local buffer\n";
            processor = std::make_unique<ProcessorUsingThreadLocalBuffer>();
            runProcessor(processor);
            break;

            case 11:
                std::cout << "\nOptimized Multi Thread Processor- Object of Arrays, buffer file read,vector memory reserve "
                     ",thread local buffer and partial file read\n";
            processor = std::make_unique<ProcessorUsingPartialRead>();
            runProcessor(processor);
            break;

            case 12:
                std::cout << "\nOptimized Multi Thread Processor- Object of Arrays, buffer file read,vector memory reserve "
                     ",thread local buffer,partial file read and epoch time\n";
            processor = std::make_unique<ProcessorUsingEpochTime>();
            runProcessor(processor);
            break;

            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
        }
    }

    return 0;
}
