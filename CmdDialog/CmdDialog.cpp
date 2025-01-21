////////////////////////////////////////
#include "CmdDialog.hpp"
////////////////////////////////////////
CmdDialogFlags operator|(const CmdDialogFlags& f, const CmdDialogFlags& v)
{
	return static_cast<CmdDialogFlags>(static_cast<long>(f) | static_cast<long>(v));
}

CmdDialog::CmdDialog()
{

}

CmdDialog::CmdDialog(const std::string& name) : dialogname(name)
{

}

CmdDialog::CmdDialog(const CmdDialogFlags& flags)
{
	if ((flags & 0b1))
		AddDefaultQuit();
	if (((flags >> 1) & 0b1))
		AddDefaultHelp();
}

CmdDialog::CmdDialog(const std::string& name, const CmdDialogFlags& flags) : dialogname(name)
{
	if ((flags & 0b1))
		AddDefaultQuit();
	if (((flags >> 1) & 0b1))
		AddDefaultHelp();
}

void CmdDialog::QueryInput()
{
	std::string input;

	while (!terminate)
	{
		std::cout << dialogname << "> ";
		std::getline(std::cin, input);

		try
		{
			std::vector<std::string> input_parsed = parseInput(input);

			if (!input_parsed.empty())
			{
				std::string name = input_parsed[0];
				Arguments args;

				input_parsed.erase(input_parsed.begin());
				
				if (!input_parsed.empty())
					args = input_parsed;

				bool found_function = false;
				for (const CmdDialogFunction& func : functions)
				{
					if (name == func.name)
					{
						try 
						{
							func.func(args.size(), args);
						}
						catch (const std::exception& exception)
						{
							std::cout << "exception thrown in CmdDialogFunction: " << exception.what() << std::endl;
						}
						found_function = true;
						break;
					}
				}
				if (!found_function)
					std::cout << "no matching function found: '" << name << "'" << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception thrown: " << e.what() << std::endl;
		}
	}
}

void CmdDialog::AddCmdDialogFunction(const std::string& name, const std::function<void(const ArgCount&, const Arguments&)>& func)
{
	functions.emplace_back(CmdDialogFunction({.name = name, .func = func}));
}
void CmdDialog::AddCmdDialogFunction(const std::string& name, const Arguments& expected_arguments, const std::function<void(const ArgCount&, const Arguments&)>& func)
{
	functions.emplace_back(CmdDialogFunction({.name = name, .expected_arguments = expected_arguments, .func = func}));
}
void CmdDialog::AddCmdDialogFunction(const std::string& name, const std::string& description, const std::function<void(const ArgCount&, const Arguments&)>& func)
{
	functions.emplace_back(CmdDialogFunction({.name = name, .description = description, .func = func}));
}
void CmdDialog::AddCmdDialogFunction(const std::string& name, const std::string& description, const Arguments& expected_arguments, const std::function<void(const ArgCount&, const Arguments&)>& func)
{
	functions.emplace_back(CmdDialogFunction({.name = name,.description = description, .expected_arguments = expected_arguments, .func = func}));
}

void CmdDialog::AddDefaultQuit()
{
	AddCmdDialogFunction("q", "default generated quit function - terminates the dialog", [this](const ArgCount&, const Arguments&) {terminate = true; });
	AddCmdDialogFunction("quit", "default generated quit function - terminates the dialog", [this](const ArgCount&, const Arguments&) {terminate = true; });
}

void CmdDialog::AddDefaultHelp()
{
	AddCmdDialogFunction("h", "default generated help function - shows this function listing", [this](const ArgCount&, const Arguments&) { PrintHelp(); });
	AddCmdDialogFunction("help", "default generated help function - shows this function listing", [this](const ArgCount&, const Arguments&) { PrintHelp(); });
}

void CmdDialog::PrintHelp()
{
	std::cout << dialogname << " - function listing\n";
	for (const CmdDialogFunction& func : functions)
	{
		std::cout << " -" << func.name;
        if (!func.expected_arguments.empty())
        {
            std::cout << " ";
	        for (std::vector<std::string>::const_iterator it = func.expected_arguments.begin(); it != func.expected_arguments.end(); it++)
	        {
	        	std::cout << *it;
                if (it != func.expected_arguments.end() - 1)
                  std::cout << ", ";
                else
                  std::cout << " ";
	        }
        }
		if (!func.description.empty())
			std::cout << " : " << func.description;
		std::cout << std::endl;
	}
}

std::vector<std::string> CmdDialog::parseInput(const std::string& input)
{
	uint64_t iterator = 0;
	std::vector<std::string> res;
	std::string parse_out;
	bool FINISHED_WORD_FLAG = false;
	bool READING_FLAG = false;

	while (iterator < input.length())
	{
		char token = input.at(iterator);
		iterator++;

		switch (token)
		{
		case(' '):
			if (READING_FLAG)
				FINISHED_WORD_FLAG = true;
			break;
		default:
			READING_FLAG = true;
			parse_out.append(1, token);
			if (iterator == input.length())
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
////////////////////////////////////////
