
#ifndef UTIL_TREE_UNIT_TEST_HPP
#define UTIL_TREE_UNIT_TEST_HPP

#include "tree.hpp"
#include <string>

namespace util
{

	static inline int _tree_unit_test_main()
	{
		printf("-----=== utilitylib tree unit test ===-----\n");
		printf("example 1: util::tree<std::string>\n");
		printf("sizeof tree<std::string>: %u bytes | sizeof node<std::string>: %u bytes | sizeof std::string: %u bytes\n",
			sizeof(tree<std::string>), sizeof(tree<std::string>::node), sizeof(std::string));
		printf("node metadata size: node<>: %u bytes\n\n",
			sizeof(tree<std::string>::node) - sizeof(std::string));

		tree<std::string> example_tree;

		// customer 1
		tree<std::string>::iterator customer_1 = example_tree.emplace_back("customer");
		tree<std::string>::iterator name_1 = customer_1->emplace_back("name");
		// customer 2
		tree<std::string>::iterator customer_2 = example_tree.emplace_back("customer");
		tree<std::string>::iterator name_2 = customer_2->emplace_back("name");
		// customer 3
		tree<std::string>::iterator customer_3 = example_tree.emplace_back("customer");
		tree<std::string>::iterator name_3 = customer_3->emplace_back("name");



		printf("example tree 1 : first iteration\n");
		for (tree<std::string>::iterator iter = example_tree.begin(); iter != example_tree.end(); ++iter)
		{
			for (std::size_t i = 0; i < iter->depth(); ++i) { printf("-"); }
			printf("%s\n", iter->value().c_str());
		}
		printf("\n");

		printf("retreiving all 'name' nodes and adding a name\n\n");
		std::vector<tree<std::string>::iterator> names = example_tree.find("name");
		for (auto& d : names) { d->emplace_back("john"); }

		printf("example tree 1 : second iteration\n");
		for (tree<std::string>::iterator iter = example_tree.begin(); iter != example_tree.end(); ++iter)
		{
			for (std::size_t i = 0; i < iter->depth(); ++i) { printf("-"); }
			printf("%s\n", iter->value().c_str());
		}
		printf("\n");

		return 0;
	}

}

#endif