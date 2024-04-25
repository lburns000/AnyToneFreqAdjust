#include <iostream>
#include <sstream>
#include <cassert>

#include "CSVFile.h"

CSVFile::CSVFile(const std::string& filename) :
    m_filename(filename),
    m_state(FS_NONE),
    m_numberColumn(0),
    m_nameColumn(0),
    m_rxFrequencyColumn(0),
    m_txFrequencyColumn(0),
    m_offsetFrequencyColumn(0),
    m_vhfOffset(0),
    m_uhfOffset(0),
    m_currentCategory("")
{
    if (!Open()) {
        m_state = FS_ERROR;
        return;
    }

    if (!PopulateData()) {
        m_state = FS_ERROR;
        return;
    }

    //// Debug
    //for (const auto& it : m_categories) {
    //    std::cout << it << std::endl;
    //}

    bool foundFreqOffset = false;
    size_t n = 0;
    for (const auto& it : m_categories) {
        if (it == "Correct Frequency[Hz]") {
            foundFreqOffset = true;
            break;
        }

        n++;
    }

    //PrintCategories();
    
    if (foundFreqOffset) {
        m_state = FS_VALID;
        //std::cout << "Found frequency offset adjust at column " << n << std::endl;
    }
    else {
        m_state = FS_ERROR;
        //std::cout << "Did not find frequency offset adjust column!" << std::endl;
    }

    Close();
}

CSVFile::~CSVFile()
{
    Close();
}

bool CSVFile::CategoryExists(const std::string& category) const
{
    for (auto it : m_categories) {
        if (it == category)
            return true;
    }

    return false;
}

std::vector<std::string> CSVFile::GetItemsInCategory(const std::string& category) const
{
    if (!CategoryExists(category)) {
        return std::vector<std::string>();
    }

    std::vector<std::string> items;

    for (const auto& it : m_data) {
        if (it.first == category) {
            items.push_back(it.second);
        }
    }

    return items;
}

void CSVFile::SetItemsInCategory(const std::string &category, const std::vector<std::string> &items)
{
    if (!CategoryExists(category))
        return;

    std::vector<std::string> curItems;
    for (const auto& it : m_data) {
        if (it.first == category)
            curItems.push_back(it.second);
    }

    assert(curItems.size() <= items.size());
    
    unsigned int curItemIndex = 0;
    for (auto& it : m_data) {
        if (it.first == category) {
            assert(curItemIndex < curItems.size());
            it = std::make_pair(category, items[curItemIndex++]);
        }
    }
}

void CSVFile::SetItemsInBooleanCategory(const std::string &category, const std::vector<bool> &items)
{
    std::vector<std::string> strItems;

    for (const auto it : items) {
        if (it == true)
            strItems.push_back(std::string("1"));
        else
            strItems.push_back(std::string("0"));
    }

    SetItemsInCategory(category, strItems);
}

void CSVFile::SetOneValueInRow(const std::string &channelName, const std::string &category, const std::string &value)
{
    std::vector<std::string> channelNames = GetItemsInCategory("Channel Name");
    std::vector<std::string> channelDataCategory = GetItemsInCategory(category);
    bool foundChannelName = false;
    for (unsigned int i = 0; i < channelNames.size(); ++i) {
        if (channelNames.at(i) == channelName) {
            channelDataCategory[i] = value;
            foundChannelName = true;
            break;
        }
    }

    if (!foundChannelName)
        return;

    SetItemsInCategory(category, channelDataCategory);

}

void CSVFile::SetOffsetsUHF(int offset)
{
    bool isUHF{ false };
    for (auto& i : m_data) {
        // Reset isUHF if new line
        if (i.first == "No.") {
            isUHF = false;
        }

        // Tx is usually the one that is needing to be changed, rather than rx
        //// Get the Rx frequency
        //if (i.first == "Receive Frequency" && std::stoi(i.second) >= 400) {
        //    isUHF = true;
        //}
         
        // Get the Tx frequency
        if (i.first == "Transmit Frequency" && std::stoi(i.second) >= 400) {
            isUHF = true;
        }

        if (i.first == "Correct Frequency[Hz]" && isUHF) {
            i = std::make_pair(i.first, std::to_string(offset));
        }
    }
}

void CSVFile::SetOffsetsVHF(int offset)
{
    bool isVHF{ false };
    for (auto& i : m_data) {
        // Reset isUHF if new line
        if (i.first == "No.") {
            isVHF = false;
        }

        // Tx is usually the one that is needing to be changed, rather than rx
        //// Get the Rx frequency
        //if (i.first == "Receive Frequency" && std::stoi(i.second) < 400) {
        //    isVHF = true;
        //}

        // Get the Tx frequency
        if (i.first == "Transmit Frequency" && std::stoi(i.second) < 400) {
            isVHF = true;
        }

        if (i.first == "Correct Frequency[Hz]" && isVHF) {
            i = std::make_pair(i.first, std::to_string(offset));
        }
    }
}

void CSVFile::RemoveAllOffsets()
{
    for (auto& i : m_data) {

        if (i.first == "Correct Frequency[Hz]") {
            i = std::make_pair(i.first, std::to_string(0));
        }
    }
}

bool CSVFile::WriteChanges()
{
    Close();

    if (!Open(true)) {
        return false;
    }

    // Write categories line
    for (size_t i = 0; i < m_categories.size(); ++i) {
        m_file << '"' << m_categories[i] << '"';
        //std::cout << m_categories[i];
        if (i != m_categories.size() - 1) {
            m_file << ',' << std::flush;
            //std::cout << ',' << std::flush;
        }
        else {
            m_file << std::endl;
            //std::cout << std::endl;
        }
    }

    auto WriteLine = [=](size_t index) {
        size_t startPos = index * m_categories.size();
        for (size_t i = 0; i < m_categories.size(); ++i) {
            m_file << '"' << m_data.at(startPos + i).second << '"';
            //std::cout << m_data.at(startPos + i).second;
            if (i != m_categories.size() - 1) {
                m_file << ',' << std::flush;
                //std::cout << ',' << std::flush;
            }
            else {
                m_file << std::endl;
                //std::cout << std::endl;
            }
        }
    };

    for (size_t i = 0; i < (m_data.size() / m_categories.size()); ++i) {
        //std::cout << "Writing line " << i << std::endl;
        WriteLine(i);
    }

    Close();

    return true;
}

void CSVFile::Rename(const std::string &filename)
{
    assert(filename.length() > 0);

    m_filename = filename;
}

bool CSVFile::Open(bool write)
{
    if (write) {
        m_file.open(m_filename, std::ios::out);
    }
    else {
        m_file.open(m_filename, std::ios::in);
    }

    if (!m_file.is_open()) {
        return false;
    }

    return true;
}

bool CSVFile::ReadCategories()
{
    if (!m_file.is_open()) {
        return false;
    }

    std::string item;

    //while (std::getline(m_file, item)) {
    //    std::cout << item << ' ';
    //    m_categories.push_back(item);
    //}

    if (std::getline(m_file, item)) {
        //std::cout << item << ' ';

        /*  Each item will appear as : "item", so :
        *       Advance past the first quote
        *       Read the word up to the second quote
        *       Advance to after the second quote
        * 
        *   TODO: Handle the case of no quotes
        */

        size_t n = 0;
        size_t nPrev = 0;

        std::cout << "Categories:" << std::endl;

        while (n != std::string::npos) {
            //n = item.find('"', nPrev);
            nPrev = n;

            // Get the first quote
            n = item.find('"', nPrev) + 1;
            if (n == std::string::npos) {
                break;
            }
            nPrev = n;

            // Get the second quote
            n = item.find('"', nPrev);

            // Add the item to the list
            std::string s = item.substr(nPrev, n - nPrev);
            m_categories.push_back(s);
            //std::cout << "  " << s << std::endl;

            // Columns used, but not changed //////////////////////////
            // Check for channel number column
            if (s == "No.") {
                m_numberColumn = m_categories.size() - 1;
                //std::cout << "number column: " << m_numberColumn << std::endl;
            }
            // Check for name column
            else if (s == "Channel Name") {
                m_nameColumn = m_categories.size() - 1;
                //std::cout << "name column: " << m_nameColumn << std::endl;
            }
            // Check for receive frequency column
            else if (s == "Receive Frequency") {
                m_rxFrequencyColumn = m_categories.size() - 1;
                //std::cout << "rx frequency column: " << m_rxFrequencyColumn << std::endl;
            }
            // Check for transmit frequency column
            else if (s == "Transmit Frequency") {
                m_txFrequencyColumn = m_categories.size() - 1;
                //std::cout << "tx frequency column: " << m_txFrequencyColumn << std::endl;
            }

            // Columns that are changed ////////////////////////////////
            // Check for frequency offset column
            else if (s == "Correct Frequency[Hz]") {
                m_offsetFrequencyColumn = m_categories.size() - 1;
                //std::cout << "frequency offset column: " << m_offsetFrequencyColumn << std::endl;
            }


            // Get the comma
            nPrev = n;
            n = item.find(',', nPrev);
            if (n == std::string::npos) {
                break;
            }
            n++;           
        }

        // Line break after list of categories
        std::cout << std::endl;
    }    

    return true;
}

bool CSVFile::PopulateData()
{
    if (!m_file.is_open()) {
        return false;
    }

    std::string line;

    // Read the first line to get the categories
    if (!std::getline(m_file, line)) {
        return false;
    }

    ParseLine(line, true);

    // Read the rest of the lines to get the data for each category
    while (std::getline(m_file, line)) {
        // Parse the line for data for each category
        ParseLine(line);
    }

    return true;
}

bool CSVFile::ParseLine(const std::string& line, bool categories)
{
    size_t nb = 0;
    size_t ne = 0;
    std::string temp = line;
    std::string item;
    size_t index = 0;

    // Keep looking for quotes
    while (true) {

        // Look for the first quote
        nb = temp.find('"');
        if (nb == std::string::npos)
            break;

        //std::cout << "Begin quote found at position " << nb << std::endl;

        // Look for the second quote
        ne = temp.find('"', nb + 1);
        if (ne == std::string::npos)
            break;

        //std::cout << "End quote found at position " << ne << std::endl;

        // Get the next item
        item = temp.substr(nb, ne - nb);
        StripQuotes(item);
        //std::cout << "Current item is: " << item << std::endl;
        if (categories) {
            m_categories.push_back(item);
        }
        else {
            m_data.push_back(std::make_pair(m_categories.at(index), item));
        }

        // Continue parsing after the end quotes
        temp = temp.substr(ne + 1);

        // If there is no comma, we are done with the line
        if (temp.find(',') == std::string::npos)
            break;

        temp = temp.substr(1);

        //std::cout << "Now parsing next item in string: " << temp << std::endl;

        ++index;
    }

    return true;
}

void CSVFile::Close()
{
    if (m_file.is_open()) {
        m_file.close();
    }
}

void CSVFile::PrintCategories()
{
    for (const auto& it : m_categories) {
        std::cout << it << ":" << std::endl;
        for (const auto& itt : m_data) {
            if (itt.first == it) {
                std::cout << "\t" << itt.second << std::endl;
            }
        }
    }
}

void CSVFile::StripQuotes(std::string& s)
{
    auto begin = s.find('"');
    auto end = s.find('"', begin + 1);
    s = s.substr(begin + 1, end - begin - 1);
}
