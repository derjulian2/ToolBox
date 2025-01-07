////////////////////////////////////////
#ifndef DIALOGUE_HANDLER_H
#define DIALOGUE_HANDLER_H
////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
////////////////////////////////////////
enum DialogueFlags
{
	DEFAULT_QUIT = 0b1,
	DEFAULT_HELP = 0b10
};
const DialogueFlags operator|(const DialogueFlags& f, const DialogueFlags& v);

typedef std::vector<std::string> Arguments;

class DialogueHandler
{
public:
	DialogueHandler();
	DialogueHandler(const std::string& name);
	DialogueHandler(const DialogueFlags& flags);
	DialogueHandler(const std::string& name, const DialogueFlags& flags);

	void QueryInput();
	void AddDialogueFunction(std::string name, std::function<void(const size_t&, const Arguments&)> func);
	void AddDialogueFunction(std::string name, std::string description, std::function<void(const size_t&, const Arguments&)> func);

	void AddDefaultQuit();
	void AddDefaultHelp();
private:
	bool terminate = false;
	std::string dialogname;

	struct DialogueFunction
	{
		std::string name;
		std::string description;
		std::function<void(const size_t&, const Arguments&)> func;
	};

	std::vector<DialogueFunction> functions;
	
	void PrintHelp();
	std::vector<std::string> parseInput(std::string input);
	char getToken(std::string& input);
};
////////////////////////////////////////
#endif
////////////////////////////////////////