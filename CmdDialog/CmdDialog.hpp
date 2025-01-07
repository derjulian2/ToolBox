////////////////////////////////////////
#ifndef CMDDIALOG_H
#define CMDDIALOG_H
////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
////////////////////////////////////////
enum CmdDialogFlags
{
	DEFAULT_QUIT = 0b1,
	DEFAULT_HELP = 0b10
};
CmdDialogFlags operator|(const CmdDialogFlags& f, const CmdDialogFlags& v);

typedef std::vector<std::string> Arguments;

class CmdDialog
{
public:
	CmdDialog();
	CmdDialog(const std::string& name);
	CmdDialog(const CmdDialogFlags& flags);
	CmdDialog(const std::string& name, const CmdDialogFlags& flags);

	void QueryInput();
	void AddCmdDialogFunction(std::string name, std::function<void(const uint64_t&, const Arguments&)> func);
	void AddCmdDialogFunction(std::string name, std::string description, std::function<void(const uint64_t&, const Arguments&)> func);

	void AddDefaultQuit();
	void AddDefaultHelp();
private:
	bool terminate = false;
	std::string dialogname;

	struct CmdDialogFunction
	{
		std::string name;
		std::string description;
		std::function<void(const uint64_t&, const Arguments&)> func;
	};

	std::vector<CmdDialogFunction> functions;
	
	void PrintHelp();
	std::vector<std::string> parseInput(std::string input);
	char getToken(std::string& input);
};
////////////////////////////////////////
#endif
////////////////////////////////////////
