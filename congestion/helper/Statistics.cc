/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Statistics.hh"
#include <fstream>
#include <string>

using namespace Congestion;

std::map<LinkId, std::list<std::tuple<Time,Time>>> Statistics::link_activity;

Statistics::Statistics() {}
Statistics::~Statistics() {}

void Statistics::recordEntry(LinkId link, std::tuple<Time, Time> entry) {
  //std::cout<<"Link: "<<link<<std::endl;
  link_activity[link].push_back(entry);
}

int Statistics::report() {
  // print the output of link activity to a file
  // Specify the file name
  std::string filename = "link-activity.txt";

  // Open the file for writing
  std::ofstream outputFile(filename);

  // Check if the file opened successfully
  if (!outputFile.is_open()) {
    std::cerr << "Failed to open the file." << std::endl;
    return 1;
  }

  for(const auto& pair: link_activity){
    LinkId link = pair.first;
    const std::list<std::tuple<double, double>>& activityBlocks = pair.second;

    outputFile << link;
    for (const auto& tuple : activityBlocks) {
      Time startTime = std::get<0>(tuple);
      Time endTime = std::get<1>(tuple);
      outputFile << ",(" << startTime << ":" << endTime<< ")";
    }
    outputFile<<std::endl;
  }

  // Close the file
  outputFile.close();

}