/*	
 *	AnyTone Frequency Offset Tool
 * 
 *	Written by: Lucas Burns
 * 
 */


#include <iostream>
#include "CSVFile.h"

int main(int argc, char* argv[])
{
	if (argc != 4) {
		std::cout << "Usage: " << argv[0] << " <filename>" << " vhf|uhf|all" << " <offset>|reset" << std::endl;
		return 0;
	}

	CSVFile channelFile(argv[1]);

	std::string band(argv[2]);
	std::string offsetStr(argv[3]);
	int offset{ 0 };
	if (band == "vhf") {
		if (offsetStr == "reset") {
			offset = 0;
		}
		else {
			offset = std::stoi(offsetStr);
		}
		std::cout << "Applying offset of " << offset << " to each VHF channel..." << std::endl;
		channelFile.SetOffsetsVHF(offset);
	}
	else if (band == "uhf") {
		if (offsetStr == "reset") {
			offset = 0;
		}
		else {
			offset = std::stoi(offsetStr);
		}
		std::cout << "Applying offset of " << offset << " to each UHF channel..." << std::endl;
		channelFile.SetOffsetsUHF(offset);
	}
	else if (band == "all") {
		if (offsetStr == "reset") {
			std::cout << "Resetting all offsets..." << std::endl;
			channelFile.RemoveAllOffsets();
		}
		else {
			offset = std::stoi(offsetStr);
			std::cout << "Applying offset of " << offset << " to every channel..." << std::endl;
			channelFile.SetOffsetsVHF(offset);
			channelFile.SetOffsetsUHF(offset);
		}
	}
	else {
		std::cout << "Error with offset syntax." << std::endl;
		return 0;
	}

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