#include "stdafx.h"
#include "stdio.h"
#include "shlobj.h"
#include "LocalizeAdventure.h"
#include <string>
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
		string16 data = (const char16_t)stringCount + ' ' + name + u"\n";
		stringCount++;
		data += (const char16_t)stringCount + ' ' + description + u"\n\n";
		stringCount++;

		data += u"# Intro text\n";
		data += (const char16_t)stringCount + ' ' + resource->mIntroText.mString.GetText() + u'\n' + u'\n';
		stringCount++;

		data += u"# Win text\n";
		data += (const char16_t)stringCount + ' ' + resource->mWinText.mString.GetText() + u'\n' + u'\n';
		stringCount++;

		data += u"# Lose text\n";
		data += (const char16_t)stringCount + ' ' + resource->mLoseText.mString.GetText() + u'\n' + u'\n';
		stringCount++;

		auto& acts = resource->mActs;
		auto& classes = resource->mClasses;
		uint32_t actCount = 1;

		for (auto& act : acts) {
			// Act number
			data += u"# Act " + (const char16_t)actCount + u'\n';

			// Act name
			data += (const char16_t)stringCount + ' ' + act.mName.mString.GetText() + u'\n';
			stringCount++;

			// Act description
			data += (const char16_t)stringCount + ' ' + act.mDescription.mString.GetText() + u'\n' + u'\n';
			stringCount++;

			// Act goals
			for (auto& goal : act.mGoals) {
				for (auto& dialog : goal.mDialogs) {
					data += (const char16_t)stringCount + ' ' + dialog.mText.mString.GetText() + u'\n';
					stringCount++;
				}
				data += u'\n';
			}
			data += u'\n' + u'\n';

			actCount++;
		}

		for (auto& classObject : classes) {
			// Object name
			data += (const char16_t)stringCount + ' ' + classObject.second.mCastName.mString.GetText() + u'\n';
			stringCount++;
			actCount = 1;

			for (auto& act : classObject.second.mActs) {
				// Object chatter
				data += u"# Chatter for Act " + (const char16_t)actCount + u'\n';

				for (auto& chatter : act.mDialogsChatter) {
					data += (const char16_t)stringCount + ' ' + chatter.mText.mString.GetText() + u'\n';
					stringCount++;
				}
				data += u'\n';

				// Object inspect
				data += u"# Inspect for Act " + (const char16_t)actCount + u'\n';

				for (auto& inspect : act.mDialogsInspect) {
					data += (const char16_t)stringCount + ' ' + inspect.mText.mString.GetText() + u'\n';
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


