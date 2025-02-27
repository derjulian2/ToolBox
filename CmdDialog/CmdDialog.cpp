////////////////////////////////////////
#include "CmdDialog.hpp"
////////////////////////////////////////
cmd::DialogFlags cmd::operator|(const DialogFlags& f, const DialogFlags& v)
{
	return static_cast<DialogFlags>(static_cast<long>(f) | static_cast<long>(v));
}

cmd::Dialog::Dialog(const std::string& name, const DialogFlags& flags) : dialogname(name)
{
	if ((flags & 0b1))
	{
		AddFunction("q", [this](const ArgCount&, const Arguments&) { this->close(); }, "default generated quit function - terminates the dialog", { "[none]" });
		AddFunction("quit", [this](const ArgCount&, const Arguments&) { this->close(); }, "default generated quit function - terminates the dialog", { "[none]" });
	}
	if (((flags >> 1) & 0b1))
	{
		AddFunction("h", [this](const ArgCount&, const Arguments&) { PrintHelp(); }, "default generated help function - shows this function listing", { "[none]" });
		AddFunction("help", [this](const ArgCount&, const Arguments&) { PrintHelp(); }, "default generated help function - shows this function listing", { "[none]" });
	}
}

void cmd::Dialog::AddFunction(const std::string& name, 
	const std::function<void(const ArgCount&, const Arguments&)>& func,
	const std::string& description,
	const Arguments& expected_arguments)
{
	functions.emplace_back(DialogFunction({
		.name = name,
		.description = description,
		.expected_arguments = expected_arguments,
		.func = func
		}));
}

void cmd::Dialog::query(std::ostream& out, std::istream& in)
{
	std::string input;
	terminate_dialog = false;

	while (!terminate_dialog)
	{
		out << dialogname << "> ";
		std::getline(in, input);


		std::vector<std::string> input_parsed = utility::split_string(input, " ");

		if (input_parsed.empty() && !input.empty())
		{
			input_parsed.emplace_back(input);
		}

		if (!input_parsed.empty())
		{
			std::string name = input_parsed[0];
			cmd::Arguments args;

			input_parsed.erase(input_parsed.begin());

			if (!input_parsed.empty())
			{
				args = input_parsed;
			}

			bool found_function = false;
			for (const DialogFunction& func : functions)
			{
				if (name == func.name)
				{
					try
					{
						if (func.func)
						{
							func.func(args.size(), args);
						}
					}
					catch (const std::exception& exception)
					{
						out << "error in DialogFunction: " << func.name << " : " << exception.what() << std::endl;
					}
					found_function = true;
					break;
				}
			}
			if (!found_function)
			{
				out << "no matching function found: '" << name << "'" << std::endl;
			}
		}
	}
}

void cmd::Dialog::close()
{
	terminate_dialog = true;
}

void cmd::Dialog::PrintHelp()
{
	std::cout << dialogname << " - function listing\n";
	for (const DialogFunction& func : functions)
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
////////////////////////////////////////
cmd::YesNoAll::YesNoAll(
	const std::string& message,
	std::function<void(void)> onYes,
	std::function<void(void)> onNo,
	std::function<void(void)> onAll
)	: message(message), onYes(onYes), onNo(onNo), onAll(onAll)
{

}
void cmd::YesNoAll::query(std::ostream& out, std::istream& in)
{
	bool terminate_dialog = false;
	std::string input;

	while (!terminate_dialog)
	{
		out << message << " [y/n";
		if (onAll)
		{
			out << "/a";
		}
		out << "] ";
		std::getline(in, input);

		try
		{
			if (utility::touppercase(input) == "Y" || utility::touppercase(input) == "YES")
			{
				onYes();
				terminate_dialog = true;
			}
			else if (utility::touppercase(input) == "N" || utility::touppercase(input) == "NO")
			{
				onNo();
				terminate_dialog = true;
			}
			else if (utility::touppercase(input) == "A" || utility::touppercase(input) == "ALL")
			{
				if (onAll)
				{
					onAll();
					terminate_dialog = true;
				}
				else
				{
					throw std::runtime_error("");
				}
			}
			else
			{
				throw std::runtime_error("");
			}
		}
		catch (const std::exception& e)
		{
			out << "invalid input" << std::endl;
		}
	}
}
////////////////////////////////////////
