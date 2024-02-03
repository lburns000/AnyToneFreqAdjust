#include <iostream>
#include "CSVFile.h"

// TODO: Command-line arguments


int main(int argc, char* argv[])
{
	if (argc < 5) {
		std::cout << "Usage: " << argv[0] << " <filename>" << " <operation>" << " vhf|uhf|all" << " <offset>|reset" << std::endl;
		return 0;
	}

	CSVFile channelFile(argv[1]);

	//std::vector<std::string> channelNumbers = channelFile.GetItemsInCategory("No.");
	//std::vector<std::string> channelNames = channelFile.GetItemsInCategory("Channel Name");
	//std::vector<std::string> channelOffsets = channelFile.GetItemsInCategory("Correct Frequency[Hz]");

	//std::cout << "\n\nNow printing some channel data..." << std::endl;

	//auto end = channelNumbers.size();
	//for (unsigned int i = 0; i < end; ++i) {
	//	std::cout << "Channel number: " << channelNumbers.at(i) << std::endl;
	//	std::cout << "Channel name:   " << channelNames.at(i) << std::endl;
	//	std::cout << "Channel offset: " << channelOffsets.at(i) << std::endl;
	//	std::cout << std::endl;
	//}

	if (std::string(argv[2]) == "offset") {
		std::string band(argv[3]);
		int offset{ 0 };
		if (band == "vhf") {
			//int offset;
			if (std::string(argv[4]) == "reset") {
				offset = 0;
			}
			else {
				offset = std::stoi(std::string(argv[4]));
			}
			std::cout << "Applying offset of " << offset << " to each VHF channel..." << std::endl;
			channelFile.SetOffsetsVHF(offset);
		}
		else if (band == "uhf") {
			//int offset;
			if (std::string(argv[4]) == "reset") {
				offset = 0;
			}
			else {
				offset = std::stoi(std::string(argv[4]));
			}
			std::cout << "Applying offset of " << offset << " to each UHF channel..." << std::endl;
			channelFile.SetOffsetsUHF(offset);
		}
		else if (band == "all") {
			//int offset;
			if (std::string(argv[4]) == "reset") {
				std::cout << "Resetting all offsets..." << std::endl;
				channelFile.RemoveAllOffsets();
			}
			else {
				offset = std::stoi(std::string(argv[4]));
				std::cout << "Applying offset of " << offset << " to every channel..." << std::endl;
				channelFile.SetOffsetsVHF(offset);
				channelFile.SetOffsetsUHF(offset);
			}
		}
		else {
			std::cout << "Error with offset syntax." << std::endl;
			return 0;
		}
	}

	//// Change something in the channels
	//std::cout << "Modifying channels..." << std::endl;

	//// Set an offset
	//channelFile.SetOffsetsUHF(200);
	//channelFile.SetOffsetsVHF(100);

	//// Clear all offsets
	//std::cout << "Resetting channel offsets..." << std::endl;
	//channelFile.RemoveAllOffsets();

	std::cout << "Writing changes..." << std::endl;
	if (!channelFile.WriteChanges()) {
		std::cout << "Error!" << std::endl;
	}
	else {
		std::cout << "Success!" << std::endl;
	}

	std::cout << "Finished." << std::endl;

	return 0;
}