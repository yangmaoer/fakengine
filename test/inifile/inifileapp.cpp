#include "fuck_game_server_engine.h"
#include "inifileapp.h"

bool inifileapp::ini( int argc, char *argv[] )
{
	return true;
}

bool inifileapp::heartbeat()
{
	myinifile ifile;
	ifile.load("config.ini");
	stringc ss;
	ifile.get("sec0", "key0", ss);

	std::cout<<"."<<ss.c_str();
	return true;
}

bool inifileapp::exit()
{
	return true;
}

inifileapp::inifileapp() : mainapp("inifile")
{

}
