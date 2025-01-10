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

typedef uint64_t ArgCount;
typedef std::vector<std::string> Arguments;

class CmdDialog
{
public:
	CmdDialog();
	CmdDialog(const std::string& name);
	CmdDialog(const CmdDialogFlags& flags);
	CmdDialog(const std::string& name, const CmdDialogFlags& flags);

	void QueryInput();
	void AddCmdDialogFunction(const std::string& name, const std::function<void(const ArgCount&, const Arguments&)>& func);
	void AddCmdDialogFunction(const std::string& name, const Arguments& expected_arguments, const std::function<void(const ArgCount&, const Arguments&)>& func);
	void AddCmdDialogFunction(const std::string& name, const std::string& description, const std::function<void(const ArgCount&, const Arguments&)>& func);
	void AddCmdDialogFunction(const std::string& name, const std::string& description, const Arguments& expected_arguments, const std::function<void(const ArgCount&, const Arguments&)>& func);

	void AddDefaultQuit();
	void AddDefaultHelp();
private:
	bool terminate = false;
	std::string dialogname;

	struct CmdDialogFunction
	{
		std::string name;
		std::string description;
		Arguments expected_arguments;
		std::function<void(const ArgCount&, const Arguments&)> func;
	};

	std::vector<CmdDialogFunction> functions;
	
	void PrintHelp();
	std::vector<std::string> parseInput(const std::string& input);
};
////////////////////////////////////////
#endif
////////////////////////////////////////
