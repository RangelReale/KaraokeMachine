#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <boost/program_options.hpp>

#include "KMSong_TSE3.h"
#include "KMBackend_SDL.h"

#include "KMachine_Basic.h"


//#include <dirent.h>

using namespace KaraokeMachine;

int main ( int argc, char** argv )
{
    std::cout << "starting KaraokeMachine..." << std::endl;
    if (kmutil_endian()==KM_BIG_ENDIAN)
        std::cout << "big endian" << std::endl;
    else
        std::cout << "little endian" << std::endl;

    int midi_port=-1;

    // Declare a group of options that will be
    // allowed only on command line
    boost::program_options::options_description opt_generic("Generic options");
    opt_generic.add_options()
        ("version,v", "print version string")
        ("help", "produce help message")
        ;

    // Declare a group of options that will be
    // allowed both on command line and in
    // config file
    boost::program_options::options_description opt_config("Configuration");
    opt_config.add_options()
        ("midi-port", boost::program_options::value<int>(&midi_port)->default_value(-1),
              "midi port")
        ("file-path,f",
             boost::program_options::value< std::vector<std::string> >()->composing(),
             "package files path")
        ;

    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    boost::program_options::options_description opt_hidden("Hidden options");
/*
    opt_hidden.add_options()
        ("input-file", boost::program_options::value< std::vector<std::string> >(), "input file")
        ;
*/

    boost::program_options::options_description cmdline_options;
    cmdline_options.add(opt_generic).add(opt_config).add(opt_hidden);

    boost::program_options::options_description config_file_options;
    config_file_options.add(opt_config).add(opt_hidden);

    boost::program_options::options_description visible("Allowed options");
    visible.add(opt_generic).add(opt_config);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, cmdline_options), vm);
    std::ifstream ifs("karaokemachine.cfg");
    boost::program_options::store(boost::program_options::parse_config_file(ifs, config_file_options), vm);

    boost::program_options::notify(vm);

    if (vm.count("help")) {
#ifdef __WIN32__
        // redirect stdout/stderr that SDL redirected
        freopen( "CON", "w", stdout );
        freopen( "CON", "w", stderr );
#endif //__WIN32__

        std::cout << "help" << visible << "\n";
        return 1;
    }


    if (!vm.count("file-path"))
    //if (argc < 2)
    {
#ifdef __WIN32__
        // redirect stdout/stderr that SDL redirected
        freopen( "CON", "w", stdout );
        freopen( "CON", "w", stderr );
#endif //__WIN32__

        std::cout << "No file paths specified.\n";
        exit(1);
    }

    KMBackend_SDL backend;
    //KMSong_TSE3 song(argv[1]);

    KMachine_Basic machine(backend);

    //machine.Songs().Load("e:\\transfer\\karaoke\\test.kps");
    //machine.Songs().Load(argv[1]);
    //machine.Songs().LoadPath("e:\\transfer\\karaoke\\");
    //machine.Songs().LoadPath("c:\\transfer\\files\\");
    //machine.Images().LoadPath("c:\\transfer\\files\\");

    std::vector<std::string> paths=vm["file-path"].as< std::vector<std::string> >();
    for (std::vector<std::string>::const_iterator ipath=paths.begin(); ipath!=paths.end(); ipath++)
    {
        std::cout << "loading from path " << *ipath << "..." << std::endl;


        //machine.Songs().LoadPath(argv[1]);
        //machine.Images().LoadPath(argv[1]);
        machine.Songs().LoadPath(*ipath);
        machine.Images().LoadPath(*ipath);
    }

    machine.Options().SetMidiPort(midi_port);

/*
    unsigned int pid=machine.Songs().Add();

    DIR *pdir;
    struct dirent *pent;

    //string songsdir("E:\\transfer\\karaoke\\");
    string songsdir("c:\\temp\\install\\");
    pdir=opendir(songsdir.c_str()); //"." refers to the current dir
    if (!pdir){
        std::cout << "opendir() failure; terminating";
        exit(1);
    }
    errno=0;
    std::cout << "Readind files..." << std::endl;
    while ((pent=readdir(pdir)))
    {
        machine.Songs().Get(pid).Add(pent->d_name,
            songsdir+string(pent->d_name));
        std::cout << pent->d_name << std::endl;
    }
    if (errno){
        std::cout << "readdir() failure; terminating";
        exit(1);
    }
    closedir(pdir);
*/

    machine.Run();

    return 0;
}

