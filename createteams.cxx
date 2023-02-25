#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <team stats JSON file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    // Read the input JSON file and parse it
    std::ifstream ifs(filename);
    json data = json::parse(ifs);
   // Get the number of players


    // // Create a JSON object with the teams key
    json output = {{"teams", json::array()}};


    // Sort the teamStats array by winPercentage in the order of closest to 50 and farthest to 50
    std::sort(data["teamStats"].begin(), data["teamStats"].end(), [](const json& a, const json& b) {
        double aDiff = std::abs(a["winPercentage"].get<double>() - 50);
        double bDiff = std::abs(b["winPercentage"].get<double>() - 50);
        return aDiff < bDiff;
    });

  
  for (auto& team : data["teamStats"]) {
      int playerOne = team["playerOne"];
      int playerTwo = team["playerTwo"];
      bool used = false;
      for (auto& t : output["teams"]) {
          if ((t[0] == playerOne || t[0] == playerTwo) || (t[1] == playerOne || t[1] == playerTwo)) {
              used = true;
              break;
          }
      }
      if (!used) {
          output["teams"].push_back({playerOne, playerTwo});
      }
  }


    // Print the result
    std::cout << output.dump() << std::endl;

    return 0;
}