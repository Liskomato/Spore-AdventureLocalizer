#include "stdafx.h"
#include "stdio.h"
#include "shlobj.h"
#include "LocalizeAdventure.h"
#include <string>
#include <sstream>
#include <Spore/Resource/Paths.h>

LocalizeAdventure::LocalizeAdventure()
{
}


LocalizeAdventure::~LocalizeAdventure()
{
}


void LocalizeAdventure::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	if (Simulator::IsScenarioMode()) {
		cScenarioResourcePtr resource = ScenarioMode.GetResource();
		// Instance IDs for the locale table
		uint32_t stringCount = 0x00000001;
		std::stringstream sstream;

		sstream << std::hex << stringCount;

		string16 whitespace = u" ";

		// Get the filepath
		string16 path = Resource::Paths::GetDirFromID(Resource::PathID::Creations);
		string16 folder = u"\\Locales\\";

		path += folder;

		if (!IO::Directory::Exists(path.c_str())) {
			IO::Directory::Create(path.c_str());
		}

		// Get name and description of the adventure
		string16 name = ScenarioMode.GetData()->mName, description = ScenarioMode.GetData()->mDescription;
		
		// Final filepath
		string16 filename = path + name + u".locale";

		// Written data goes to string16 data. Starts with adventure name as first instance ID.
		string16 data = sstream.str() + whitespace + name + u"\n";
		stringCount++;
		data += stringCount + ' ' + description + u"\n\n";
		stringCount++;

		data += u"# Intro text\n";
		data += stringCount + ' ' + resource->mIntroText.mNonLocalizedString + u'\n' + u'\n';
		stringCount++;

		data += u"# Win text\n";
		data += stringCount + ' ' + resource->mWinText.mNonLocalizedString + u'\n' + u'\n';
		stringCount++;

		data += u"# Lose text\n";
		data += stringCount + ' ' + resource->mLoseText.mNonLocalizedString + u'\n' + u'\n';
		stringCount++;

		auto& acts = resource->mActs;
		auto& classes = resource->mClasses;
		uint32_t actCount = 1;

		for (auto& act : acts) {
			// Act number
			data += u"# Act " + actCount + u'\n';

			// Act name
			data += stringCount + ' ' + act.mName.mNonLocalizedString + u'\n';
			stringCount++;

			// Act description
			data += stringCount + ' ' + act.mDescription.mNonLocalizedString + u'\n' + u'\n';
			stringCount++;

			// Act goals
			for (auto& goal : act.mGoals) {
				for (auto& dialog : goal.mDialogs) {
					data += stringCount + ' ' + dialog.mText.mNonLocalizedString + u'\n';
					stringCount++;
				}
				data += u'\n';
			}
			data += u'\n' + u'\n';

			actCount++;
		}

		for (auto& classObject : classes) {
			// Object name
			data += stringCount + ' ' + classObject.second.mCastName.mNonLocalizedString + u'\n';
			stringCount++;
			actCount = 1;

			for (auto& act : classObject.second.mActs) {
				// Object chatter
				data += u"# Chatter for Act " + actCount + u'\n';

				for (auto& chatter : act.mDialogsChatter) {
					data += stringCount + ' ' + chatter.mText.mNonLocalizedString + u'\n';
					stringCount++;
				}
				data += u'\n';

				// Object inspect
				data += u"# Inspect for Act " + actCount + u'\n';

				for (auto& inspect : act.mDialogsInspect) {
					data += stringCount + ' ' + inspect.mText.mNonLocalizedString + u'\n';
					stringCount++;
				}
				data += u'\n';
				actCount++;
			}
		}

		FileStreamPtr stream = new IO::FileStream(filename.c_str());
		if (stream->Open(IO::AccessFlags::ReadWrite,IO::CD::CreateAlways)) {
			stream->Write(data.c_str(),data.size());
			stream->Close();
			App::ConsolePrintF("Data written successfully.");
		}
	}
}

const char* LocalizeAdventure::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "LocalizeAdventure: Elaborate description of what this cheat does.";
	}
}


