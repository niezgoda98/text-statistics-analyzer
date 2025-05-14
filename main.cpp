#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

class TextStatistics {
private:
    std::string filename;
    int wordCount;
    int charCount;
    int charCountNoSpaces;
    int sentenceCount;
    std::map<std::string, int> wordFrequency;

    bool isSentenceEnd(char c) {
        return c == '.' || c == '!' || c == '?';
    }

    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

public:
    TextStatistics(const std::string& fname) : filename(fname), wordCount(0), 
        charCount(0), charCountNoSpaces(0), sentenceCount(0) {}

    bool analyzeFile() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            charCount += line.length();
            charCountNoSpaces += std::count_if(line.begin(), line.end(), 
                [](char c) { return !std::isspace(c); });

            // Count sentences
            for (char c : line) {
                if (isSentenceEnd(c)) {
                    sentenceCount++;
                }
            }

            // Process words
            std::istringstream iss(line);
            std::string word;
            while (iss >> word) {
                // Remove punctuation from the end of words
                while (!word.empty() && std::ispunct(word.back())) {
                    word.pop_back();
                }
                if (!word.empty()) {
                    wordCount++;
                    wordFrequency[toLower(word)]++;
                }
            }
        }

        file.close();
        return true;
    }

    void displayStatistics() const {
        std::cout << "\nText Statistics for: " << filename << "\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Word Count: " << wordCount << "\n";
        std::cout << "Character Count (with spaces): " << charCount << "\n";
        std::cout << "Character Count (without spaces): " << charCountNoSpaces << "\n";
        std::cout << "Sentence Count: " << sentenceCount << "\n";
    }

    void displayTopWords(int n = 10) const {
        std::cout << "\nTop " << n << " Most Frequent Words:\n";
        std::cout << "----------------------------------------\n";

        // Convert map to vector for sorting
        std::vector<std::pair<std::string, int>> wordVec(wordFrequency.begin(), wordFrequency.end());
        
        // Sort by frequency (descending)
        std::sort(wordVec.begin(), wordVec.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });

        // Display top N words
        for (size_t i = 0; i < std::min(static_cast<size_t>(n), wordVec.size()); ++i) {
            std::cout << std::setw(15) << wordVec[i].first << ": " 
                      << wordVec[i].second << " occurrences\n";
        }
    }

};

int main() {
    std::string filename;
    std::cout << "Enter the name of the text file to analyze: ";
    std::getline(std::cin, filename);

    TextStatistics stats(filename);
    
    if (!stats.analyzeFile()) {
        return 1;
    }

    stats.displayStatistics();
    stats.displayTopWords();


    return 0;
} 