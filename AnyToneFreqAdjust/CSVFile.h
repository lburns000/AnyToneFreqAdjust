#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <utility>

enum FILE_STATE {
	FS_NONE,
	FS_VALID,
	FS_ERROR
};

// Begin Vector of vector of pairs, each pair is: <std::string, std::string>
//		"Channel Name", "<name0>"
//		"Channel Name", "<name1>"
//		...
//		"Channel Name", "<nameN>"
//		"Receive Frequency", "<rx freq0>"
//		"Receive Frequency", "<rx freq1>"
//		...
//		"Receive Frequency", "<rx freqN>"
//		"Transmit Frequency", "<tx freq0>"
//		"Transmit Frequency", "<tx freq1>"
//		...
//		"Transmit Frequency", "<tx freqN>"
//
//
// For each category:
//		Under the category string of the current index:
//			Add the item to the vector with this string label
// 

// Data type for the list of key-value pairs
typedef std::vector<std::pair<std::string, std::string>> CSVData;

class CSVFile
{
public:
	CSVFile(std::string filename);
	~CSVFile();

	bool CategoryExists(std::string category);
	std::vector<std::string> GetItemsInCategory(std::string category);

	void SetOffsetsUHF(int offset);
	void SetOffsetsVHF(int offset);
	void RemoveAllOffsets();

	bool WriteChanges();

private:
	bool		Open(bool write = false);
	bool		ReadCategories();
	bool		PopulateData();
	bool		ParseLine(const std::string& line, bool categories = false);
	std::string ReadFrequencyOffset(const std::string& line, size_t column);
	void		Close();
	void		PrintCategories();

	void		StripQuotes(std::string& s);

	std::string					m_filename;
	std::fstream				m_file;
	std::vector<std::string>	m_categories;
	FILE_STATE					m_state;
	size_t						m_numberColumn;
	size_t						m_nameColumn;
	size_t						m_rxFrequencyColumn;
	size_t						m_txFrequencyColumn;
	size_t						m_offsetFrequencyColumn;
	int							m_vhfOffset;
	int							m_uhfOffset;
	CSVData						m_data;
	std::string					m_currentCategory;
};

