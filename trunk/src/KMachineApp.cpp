#include <stdlib.h>

#include "KMSong_TSE3.h"
#include "KMBackend_SDL.h"

#include "KMachine_Basic.h"


//#include <dirent.h>

using namespace KaraokeMachine;

int main ( int argc, char** argv )
{
    if (argc < 2)
    {
        std::cout << "Useage: KaraokeMachine <directory>\n";
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

    machine.Songs().LoadPath(argv[1]);
    machine.Images().LoadPath(argv[1]);

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

