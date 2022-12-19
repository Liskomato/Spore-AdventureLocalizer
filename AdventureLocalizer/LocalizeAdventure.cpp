#include "stdafx.h"
#include "stdio.h"
#include "shlobj.h"
#include "LocalizeAdventure.h"
#include <string>
#include <sstream>
#include <Spore/Resource/Paths.h>
#include <Spore\Properties.h>

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
		uint32_t stringCount = 1;
		string16 data;

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
		data.append_sprintf(u"%#010x %ls\n",stringCount,name);
		stringCount++;
		data.append_sprintf(u"%#010x %ls\n\n", stringCount, description);
		stringCount++;

		data.append_sprintf(u"# Intro text\n");
		data.append_sprintf(u"%#010x %ls\n\n",stringCount, resource->mIntroText.mNonLocalizedString);
		stringCount++;

		data.append_sprintf(u"# Win text\n");
		data.append_sprintf(u"%#010x %ls\n\n", stringCount, resource->mWinText.mNonLocalizedString); 
		stringCount++;

		data.append_sprintf(u"# Lose text\n");
		data.append_sprintf(u"%#010x %ls\n\n", stringCount, resource->mLoseText.mNonLocalizedString);
		stringCount++;

		auto& acts = resource->mActs;
		auto& classes = resource->mClasses;
		uint32_t actCount = 1;
		uint32_t goalCount = 1;

		for (auto& act : acts) {
			// Act number
			data.append_sprintf(u"# Act %d\n",actCount);

			// Act name
			data.append_sprintf(u"%#010x %ls\n",stringCount,act.mName.mNonLocalizedString);
			stringCount++;

			// Act description
			data.append_sprintf(u"%#010x %ls\n\n",stringCount, act.mDescription.mNonLocalizedString);
			stringCount++;

			// Act goals
			goalCount = 1;

			for (auto& goal : act.mGoals) {
				
				data.append_sprintf(u"# Goal %d\n",goalCount);
				for (auto& dialog : goal.mDialogs) {
					if (dialog.mText.mNonLocalizedString != u"") {
						data.append_sprintf(u"%#010x %ls\n", stringCount, dialog.mText.mNonLocalizedString);
						stringCount++;
					}
				}
				data.append_sprintf(u"\n");
				goalCount++;
			}
			data.append_sprintf(u"\n");

			actCount++;
		}

		for (auto& classObject : classes) {
			// Object name
			if (classObject.second.mCastName.mNonLocalizedString != u"")
			data.append_sprintf(u"%#010x %ls\n\n",stringCount,classObject.second.mCastName.mNonLocalizedString);
			else {
				cAssetMetadataPtr metadata;
				PropertyListPtr propList;
				LocalizedString str;
				if (Pollinator::GetMetadata(classObject.second.mAsset.mKey.instanceID,classObject.second.mAsset.mKey.groupID,metadata))
				data.append_sprintf(u"%#010x %ls\n\n", stringCount, metadata->GetName());
				else if (PropManager.GetPropertyList(classObject.second.mAsset.mKey.instanceID, classObject.second.mAsset.mKey.groupID, propList)
						&& App::Property::GetText(propList.get(),id("sporepediaName"),str)) {
					data.append_sprintf(u"%#010x %ls\n\n", stringCount, str.GetText());
				}
				else {
					App::ConsolePrintF("Name data for asset %#010x could not be found - Leaving name blank.",stringCount);
					data.append_sprintf(u"%#010x \n\n",stringCount);
				}
			}
			stringCount++;
			actCount = 1;

			for (auto& act : classObject.second.mActs) {
				// Object chatter
				data.append_sprintf(u"# Chatter for Act %d\n",actCount);

				for (auto& chatter : act.mDialogsChatter) {
					if (chatter.mText.mNonLocalizedString != u"") {
						data.append_sprintf(u"%#010x %ls\n", stringCount, chatter.mText.mNonLocalizedString);
						stringCount++;
					}
				}
				data.append_sprintf(u"\n");

				// Object inspect
				data.append_sprintf(u"# Inspect for Act %d\n", actCount);

				for (auto& inspect : act.mDialogsInspect) {
					if (inspect.mText.mNonLocalizedString != u"") {
						data.append_sprintf(u"%#010x %ls\n", stringCount, inspect.mText.mNonLocalizedString);
						stringCount++;
					}
				}
				data.append_sprintf(u"\n");
				actCount++;
			}
		}

		FileStreamPtr stream = new IO::FileStream(filename.c_str());
		if (stream->Open(IO::AccessFlags::ReadWrite,IO::CD::CreateAlways)) {
			stream->Write(data.c_str(),data.size()*2);
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


