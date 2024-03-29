// Copyright (c) 2020 DGB. All rights reserved.

#include <iostream>
#include "Menu.h"
#include "contact_book/ContactBook.h"
using namespace cb;

auto main() -> int
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ContactBook contactBook;

	menu::Menu menu(hConsole); 
	menu.AddItem("Show contacts",  [&contactBook]() { contactBook.ShowContact();   });
	menu.AddItem("Add contact",    [&contactBook]() { contactBook.AddContact();    });
	menu.AddItem("Find contact",   [&contactBook]() { contactBook.FindContact();   });
	menu.AddItem("Redact contact", [&contactBook]() { contactBook.RedactContact(); });
	menu.AddItem("Delete contact", [&contactBook]() { contactBook.DeleteContact(); });
	menu.GetInput();

	return 0;
}