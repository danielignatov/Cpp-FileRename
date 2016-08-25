#include <iostream>
#include <ctype.h>
#include <string>

std::string convertToUppercase(std::string text)
{
	std::string textToReturn = "";

	for (int i = 0; i < text.length(); i++)
	{
		textToReturn += toupper(text[i]);
	}

	return textToReturn;
}

int main()
{
	// Setting locale
	std::locale::global(std::locale(""));  // (*)
	std::wcout.imbue(std::locale());
	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());

	std::string englishString = "English String";
	std::string bulgarianString = "Български Стринг";

	std::string rewriteEnglishString = convertToUppercase(englishString);
	std::string rewriteBulgarianString = convertToUppercase(bulgarianString);

	std::cout << rewriteEnglishString << std::endl;
	std::cout << rewriteBulgarianString << std::endl;

	// Expected output:
	// ENGLISH STRING
	// БЪЛГАРСКИ СТРИНГ

	return 0;
}