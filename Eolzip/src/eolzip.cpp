#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "compress.hpp"

bool dflag, hflag, kflag, lflag, oflag, vflag;
std::string outputFile, inputFile;

namespace
{
    bool check_args(int argc, const char* const * argv)
    {
        if (argc <= 1)
            return false;
        for (int i = 1; i < argc; ++i)
        {
            if (argv[i][0] == '-')
            {
                if (strlen(argv[i]) >= 2)
                {
                    if (argv[i][1] != '-')
                    {
                        for (size_t j = 1; j < strlen(argv[i]); ++j)
                        {
                            switch (argv[i][j])
                            {
                                case 'c':
                                    dflag = false;
                                    break;
                                case 'd':
                                    dflag = true;
                                    break;
                                case 'h':
                                    hflag = true;
                                    return true;
                                case 'k':
                                    kflag = true;
                                    break;
                                case 'l':
                                    lflag = true;
                                    return true;
                                case 'o':
                                    // output file cannot be the last one
                                    oflag = true;
                                    if (i + 1 < argc - 1 && argv[i + 1][0] != '-')
                                        outputFile = std::string(argv[i + 1]);
                                    else
                                        return false;
                                    break;
                                case 'v':
                                    vflag = true;
                                    return true;
                            
                                default:
                                    return false;
                            }
                        }
                    }
                    else
                    {
                        if (!strcmp(argv[i], "--compress"))
                            dflag = false;
                        else if (!strcmp(argv[i], "--decompress"))
                            dflag = true;
                        else if (!strcmp(argv[i], "--help"))
                        {
                            hflag = true;
                            return true;
                        }
                        else if (!strcmp(argv[i], "--keep"))
                            kflag = true;
                        else if (!strcmp(argv[i], "--license"))
                        {
                            lflag = true;
                            return true;
                        }
                        else if (!strcmp(argv[i], "--version"))
                        {
                            vflag = true;
                            return true;
                        }
                        else if (!strcmp(argv[i], "--output"))
                        {
                            oflag = true;
                            if (i + 1 <= argc - 1 && argv[i + 1][0] != '-')
                                outputFile = std::string(argv[i + 1]);
                            else
                                return false;
                        }
                    }
                }
                else
                    return false;
            }
        }
        return argv[argc - 1][0] != '-'; // last arg must be file name
    }

    void print_basic_usage()
    {
        puts("Usage: eolzip [OPTION]... [FILE]...");
        puts("For more detail, use 'eolzip --help'");
    }

    void print_lisence()
    {
    }

    void print_version()
    {
        puts("Version: 0.1.0");
    }

    void delete_file(const std::string& file)
    {
        system((std::string("rm ") + file).c_str());
    }
}


int main(int argc, char* argv[])
{
    dflag = hflag = kflag = lflag = oflag = vflag = false;
    if (!check_args(argc, argv))
    {
        print_basic_usage();
        exit(EXIT_FAILURE);
    }

    if (hflag)
    {
        std::cout << "Usage: ezip [OPTION]... [FILE]...\n\
Compress or uncompress FILEs\n\
\n\
  -c, --compress    compress (default)\n\
  -d, --decompress  decompress\n\
  -h, --help        give this help\n\
  -k, --keep        keep (don't delete) input files\n\
  -l, --license     display software license\n\
  -o, --output      specify the output file name\n\
                    use when only one input file is given\n\
  -v, --version     display version number\n\
\n\
Report bugs to <hslphst@gmail.com>.\n\
\n\
SPECIAL USAGE: ezip <input> <output>" << std::endl;
    }
    else if (lflag)
        print_lisence();
    else if (vflag)
        print_version();
    else
    {
        // Normal Progress
        bool specialUsage(true);
        if (argc == 3)
            for (int i = 1; i < argc; ++i)
            {
                if (argv[i][0] == '-')
                {
                    specialUsage = false;
                    break;
                }
            }
        if (specialUsage)
        {
            kflag = true;
            inputFile = argv[1];
            outputFile = argv[2];
        }
        else
        {
            inputFile = argv[argc - 1];
            if (!oflag)
                outputFile = inputFile + ".ez";
        }
        if (!dflag)
        {
            try
            {
                compress(inputFile, outputFile);
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                exit(EXIT_FAILURE);
            }
            catch (const std::string& estr)
            {
                std::cerr << estr << std::endl;
            }
        }
        else
        {
            // decompress
            std::cout << "Not finished." << std::endl;
        }
        if (!kflag)
        {
            delete_file(inputFile);
        }
    }
    return 0;
}

