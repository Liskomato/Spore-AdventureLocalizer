#pragma once

#include <Spore\BasicIncludes.h>

class LocalizeAdventure 
	: public ArgScript::ICommand
{
public:
	LocalizeAdventure();
	~LocalizeAdventure();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	string16 RemoveNewLines(string16 string);

	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

};

