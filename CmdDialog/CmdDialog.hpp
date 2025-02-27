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
#define UTIL_MOD_STRINGMANIP
#include "utilitylib.hpp"
////////////////////////////////////////
#define DIALOG_ARGS const cmd::ArgCount& argcount, const cmd::Arguments& arguments
////////////////////////////////////////
namespace cmd
{
	enum DialogFlags
	{
		NONE         = 0b0,
		DEFAULT_QUIT = 0b1,
		DEFAULT_HELP = 0b10
	};
	DialogFlags operator|(const DialogFlags& f, const DialogFlags& v);

	typedef uint8_t ArgCount;
	typedef std::vector<std::string> Arguments;

	class Dialog
	{
	public:
		Dialog(const std::string& name = "dialog", const DialogFlags& flags = DEFAULT_QUIT | DEFAULT_HELP);

		/*
		* adds a function to the user dialog
		* NOTE: you have to perform argument-checks yourself in your callbacks, Dialog will not ensure expected arguments
		* are the same as the actually passed arguments.
		*/
		void AddFunction(const std::string& name, 
			const std::function<void(const ArgCount&, const Arguments&)>& func = NULL,
			const std::string& description = "no description",
			const Arguments& expected_arguments = { "[none]" });
		/*
		* [blocking]
		* will start the dialog with the user in the specified in/outstream.
		* the user can call the previously defined callback functions by entering the
		* corresponding function name and the required arguments.
		* 
		* NOTE: this function will NOT return unless close() is called, so either call it yourself in
		* one of your callback-functions, or specify DialogFlags::DEFAULT_QUIT in the constructor.
		*/
		void query(std::ostream& out, std::istream& in);
		void close();
	private:
		bool terminate_dialog = true;
		std::string dialogname = "dialog";

		struct DialogFunction
		{
			std::string name;
			std::string description;
			Arguments expected_arguments;
			std::function<void(const ArgCount&, const Arguments&)> func;
		};

		std::vector<DialogFunction> functions;

		void PrintHelp();
	};
	////////////////////////////////////////
	class YesNoAll
	{
	public:
		YesNoAll(
			const std::string& message = "answer:",
			std::function<void(void)> onYes = []() {},
			std::function<void(void)> onNo  = []() {},
			std::function<void(void)> onAll = NULL
		);
		/*
		* [blocking]
		* will repeatedly ask the user for [Y/N/A] in the specified in/out-stream
		* and call the previosly set callbacks on events.
		*
		* will return when one of the callbacks has been called.
		*/
		void query(std::ostream& out, std::istream& in);
	private:
		std::string message;
		std::function<void(void)> onYes;
		std::function<void(void)> onNo;
		std::function<void(void)> onAll;
	};
}
////////////////////////////////////////
#endif
////////////////////////////////////////
