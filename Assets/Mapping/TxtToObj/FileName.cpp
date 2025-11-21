#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    std::string inputFile = "lasdata.txt";
    std::string outputFile = "lasdata.obj";

    std::ifstream inFile(inputFile);

    std::ofstream outFile(outputFile);

    std::string line;
    std::getline(inFile, line);

    double x, y, z;
    while (std::getline(inFile, line)) 
    {
        std::istringstream iss(line);
        if (!(iss >> x >> y >> z)) 
        {
            continue; 
        }
        outFile << "v " << x << " " << y << " " << z << "\n";
    }

    inFile.close();
    outFile.close();

    std::cout << "Successfully converted " << inputFile << " to " 
              << outputFile << std::endl;
    return 0;
}
