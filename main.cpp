#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

enum PATH_FORMAT {
    POSIX,
    FISH
};

class PathGen {
    std::list<std::string> path_list;

public:
    std::string path_var;
    PATH_FORMAT path_format = POSIX;

    std::string getPathString(const char* delim = " ") {
        return boost::algorithm::join(path_list, delim);
    }

    std::string genPath() {
        if (path_format == FISH) {
            return std::string("set -x ") + path_var + " " + getPathString(" ");
        } else if (path_format == POSIX) {
            return std::string("export ") + path_var + "=" + getPathString(":");
        } else {
            return std::string();
        }
    }
    
    bool pathsContain(std::string& path_to_search) {
        return std::any_of(path_list.begin(), path_list.end(), [&path_to_search](const std::string& path) {
            return path == path_to_search;
        });
    }

    void addToPath(std::string path_to_add) {
        if (not pathsContain(path_to_add)) {
            path_list.push_front(path_to_add);
        }
    }

    explicit PathGen(const char* _path_var) {
        path_var = _path_var;
        boost::split(path_list, getenv(path_var.c_str()), boost::is_any_of(":"));
    }
};

PathGen pathGen = PathGen("PATH");

void handleAddToPath(const std::vector<std::string>& paths_to_add) {
    for (const auto& path: paths_to_add) {
        pathGen.addToPath(path);
    }
}

void handleSelectPathType(std::string type_str) {
    boost::algorithm::to_lower(type_str);
    // TODO: replace with map
    if (type_str == "posix") {

    } else if (type_str == "fish") {

    } else {
        std::cerr<<"Eh... what type is that? I only understand 'posix' and 'fish'!" << std::endl;
        exit(1);
    }
}

int main(int argc, char** argv) {

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show brief usage message")
        (
                "add-to-path,a",
                po::value<std::vector<std::string>>()->required()->notifier(&handleAddToPath),
                "Add path(s) to variable"
        )

        (
                "type,t",
                po::value<std::string>()->required()->notifier(&handleSelectPathType),
                "Set type."
         );

    po::variables_map args;
    po::store(po::parse_command_line(argc, argv, desc),args);

    try {
        po::notify(args);
    } catch (std::exception& e){
        std::cerr<<"Error encountered: "<<e.what()<<std::endl;
        return 1;
    }

    std::cout << pathGen.genPath() << std::endl;
    return 0;
}