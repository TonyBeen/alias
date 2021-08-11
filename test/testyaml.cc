#include <yaml-cpp/yaml.h>
#include <fstream>
#include <string>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

void LoadYamlFile()
{
    YAML::Node config = YAML::LoadFile("./config.yaml");
    cout << config << endl;

    config["log"]["leval"] = "I";
    std::string fmt = config["log"]["dateformat"].as<std::string>();
    cout << fmt << endl;
    char buf[32] = {0};
    sprintf(buf, fmt.c_str(), 1, 1, 1, 1, 1, 1, 1);
    cout << "buf = " << buf << "\n";
    ofstream fout("./config.yaml");
    fout << config;
}

void ConstructYaml_seq()
{
    YAML::Emitter out;
    out << YAML::BeginSeq;
    out << "eggs bread milk";
    out << YAML::EndSeq;

    ofstream fout("./config.yaml");
    fout << out.c_str();
    std::cout << "内容\n" << out.c_str() << std::endl
    << out.size() << "\nstrlen = "<< strlen(out.c_str()) << std::endl;
    fout.close();
}

void ConstructYaml_map()
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "name";
    out << YAML::Value << "Ryan Braun";
    out << YAML::Key << "position";
    out << YAML::Value << "LF";
    out << YAML::EndMap;

    ofstream fout("./config.yaml");
    fout << out.c_str() << endl;
}

void map_seq()
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "name";
    out << YAML::Value << "Barack Obama";
    out << YAML::Key << "children";
    out << YAML::Value <<
        YAML::BeginSeq << "Sasha" << "Malia" << YAML::EndSeq;
    out << YAML::EndMap;

    ofstream fout("./config.yaml");
    fout << out.c_str() << endl;

}

int main()
{
    LoadYamlFile();
    return 0;
}