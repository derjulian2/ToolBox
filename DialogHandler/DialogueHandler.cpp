////////////////////////////////////////
#include "DialogueHandler.hpp"
////////////////////////////////////////
const DialogueFlags operator|(const DialogueFlags& f, const DialogueFlags& v)
{
	return static_cast<DialogueFlags>(static_cast<long>(f) | static_cast<long>(v));
}

DialogueHandler::DialogueHandler()
{

}

DialogueHandler::DialogueHandler(const std::string& name) : dialogname(name)
{

}

DialogueHandler::DialogueHandler(const DialogueFlags& flags)
{
	if ((flags & 0b1))
		AddDefaultQuit();
	if (((flags >> 1) & 0b1))
		AddDefaultHelp();
}

DialogueHandler::DialogueHandler(const std::string& name, const DialogueFlags& flags) : dialogname(name)
{
	if ((flags & 0b1))
		AddDefaultQuit();
	if (((flags >> 1) & 0b1))
		AddDefaultHelp();
}

void DialogueHandler::QueryInput()
{
	std::string input;

	while (!terminate)
	{
		if (!dialogname.empty())
			std::cout << dialogname << "> ";
		else
			std::cout << "dialog" << "> ";
		std::getline(std::cin, input);

		try
		{
			std::vector<std::string> input_parsed = parseInput(input);

			if (!input_parsed.empty())
			{
				std::string name = input_parsed[0];

				input_parsed.erase(input_parsed.begin());
				Arguments args = input_parsed;
				size_t argc = args.size();

				for (const DialogueFunction& func : functions)
				{
					if (name == func.name)
					{
						func.func(argc, args);
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception thrown: " << e.what() << std::endl;
		}
	}
}

void DialogueHandler::AddDialogueFunction(std::string name, std::function<void(const size_t&, const Arguments&)> func)
{
	functions.emplace_back(DialogueHandler::DialogueFunction({.name = name, .func = func}));
}

void DialogueHandler::AddDialogueFunction(std::string name, std::string description, std::function<void(const size_t&, const Arguments&)> func)
{
	functions.emplace_back(DialogueHandler::DialogueFunction({ .name = name, .description = description, .func = func }));
}

void DialogueHandler::AddDefaultQuit()
{
	AddDialogueFunction("q", [this](const size_t&, const Arguments&) {terminate = true; });
	AddDialogueFunction("quit", [this](const size_t&, const Arguments&) {terminate = true; });
}

void DialogueHandler::AddDefaultHelp()
{
	AddDialogueFunction("h", [this](const size_t&, const Arguments&) { PrintHelp(); });
	AddDialogueFunction("help", [this](const size_t&, const Arguments&) { PrintHelp(); });
}

void DialogueHandler::PrintHelp()
{
	std::cout << dialogname << " - function manual\n";
	for (const DialogueFunction& func : functions)
	{
		std::cout << " -" << func.name;
		if (!func.description.empty())
			std::cout << " : " << func.description;
		std::cout << std::endl;
	}
}

std::vector<std::string> DialogueHandler::parseInput(std::string input)
{
	std::vector<std::string> res;
	std::string parse_out;
	bool FINISHED_WORD_FLAG = false;
	bool READING_FLAG = false;

	while (!input.empty())
	{
		char token = getToken(input);
		switch (token)
		{
		case(' '):
			if (READING_FLAG)
				FINISHED_WORD_FLAG = true;
			break;
		default:
			READING_FLAG = true;
			parse_out.append(1, token);
			if (input.empty())
				FINISHED_WORD_FLAG = true;
			break;
		}

		if (FINISHED_WORD_FLAG)
		{
			res.emplace_back(parse_out);
			parse_out.clear();
			READING_FLAG = false;
			FINISHED_WORD_FLAG = false;
		}
	}

	return res;
}

char DialogueHandler::getToken(std::string& input)
{
	char res = input[0];
	input = input.substr(1);
	return res;
}

////////////////////////////////////////