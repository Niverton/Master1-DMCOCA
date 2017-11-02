#include "options.h"

#include <string>
#include <unistd.h>
#include <iostream>

const char *options::opts = ":o:gc";

void usage(const char* name) {
  std::cerr << "Usage:\n" << name
            << " [-o <output_file>] [-g] <k>\n"
            << "\t-o: optional output file\n"
            << "\t-g: display adjacency matrix\n"
            << "\t-c: enable commentary\n";
}

options::options(int argc, char** argv) {
  int c;
  while (true) {
    c = getopt(argc, argv, opts);
    if (c == ':' or c == '?') {
      std::cerr << "Unrecognized option or missing argument\n";
      exit(1);
      return;
    }
    if (c == -1) {
      break;
    }

    switch (c) {
      case 'o':
        output = optarg;
        break;
      case 'g':
        display_adj = true;
        break;
      case 'c':
        coms = true;
        break;
      default:
        std::cerr << "Error\n";
        usage(argv[0]);
        return;
    }
  }
  if (optind > argc) {
    std::cerr << "Missing k\n";
    usage(argv[0]);
    exit(1);
    return;
  }

  try { // Catch invalid_parameter
    k = std::stoi(argv[optind]);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    usage(argv[0]);
    exit(1);
  }
}
