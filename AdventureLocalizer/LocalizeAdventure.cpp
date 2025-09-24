#include "stdafx.h"
#include "stdio.h"
#include "shlobj.h"
#include "LocalizeAdventure.h"
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>
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
	if (Simulator::IsScenarioMode() && ScenarioMode.GetMode() == App::cScenarioMode::Mode::EditMode) {
		ScenarioMode.GetData()->StartHistoryEntry();
		cScenarioResourcePtr resource = ScenarioMode.GetResource();
		// Instance IDs for the locale table
		uint32_t stringCount = 1;
		string16 data;

		// Get the filepath
		string16 path = Resource::Paths::GetDirFromID(Resource::PathID::Creations);
		string16 folder = u"Locales/";

		path += folder;

		if (!IO::Directory::Exists(path.c_str())) {
			IO::Directory::Create(path.c_str());
		}

		

		// Get name, author, tags and description of the adventure
		string16 name, description, tags, author = u"";
		cAssetMetadataPtr metadata;
		if (ScenarioMode.GetData()->mName != u"") {
			name = ScenarioMode.GetData()->mName, description = ScenarioMode.GetData()->mDescription, tags = ScenarioMode.GetData()->mTags;
			if (Pollinator::GetMetadata(resource->GetResourceKey().instanceID, resource->GetResourceKey().groupID, metadata)) {
				author = metadata->GetAuthor();
			}
		}
		else {
			App::ConsolePrintF("The adventure requires a name first.");
			return;
		}
		// Final filepath
		string16 filename = path + name + u".locale";

		RemoveNewLines(description);

		// Written data goes to string16 data. Starts with adventure name as first instance ID.
		data.append_sprintf(u"# Auto-generated localization file for: %ls (ID: %#X)\n",name.c_str(), id(name.c_str()));

		data.append_sprintf(u"# Author\n%#010x %ls\n\n", stringCount, author);
		stringCount++;
		data.append_sprintf(u"# Adventure name\n%#010x %ls\n\n",stringCount,name);
		stringCount++;
		data.append_sprintf(u"# Adventure description\n%#010x %ls\n\n", stringCount, description);
		stringCount++;
		data.append_sprintf(u"# Adventure tags\n%#010x %ls\n\n", stringCount, tags);
		stringCount++;

		if (resource->mIntroText.mNonLocalizedString != u"") {
			RemoveNewLines(resource->mIntroText.mNonLocalizedString);
			data.append_sprintf(u"# Intro text\n");
			data.append_sprintf(u"%#010x %ls\n\n", stringCount, resource->mIntroText.mNonLocalizedString);
			resource->mIntroText.mLocalizedStringInstanceID = stringCount;
			resource->mIntroText.mLocalizedStringTableID = id(name.c_str());
			resource->mIntroText.mNonLocalizedString = u"";
			stringCount++;
		}
		if (resource->mWinText.mNonLocalizedString != u"") {
			RemoveNewLines(resource->mWinText.mNonLocalizedString);
			data.append_sprintf(u"# Win text\n");
			data.append_sprintf(u"%#010x %ls\n\n", stringCount, resource->mWinText.mNonLocalizedString);
			resource->mWinText.mLocalizedStringInstanceID = stringCount;
			resource->mWinText.mLocalizedStringTableID = id(name.c_str());
			resource->mWinText.mNonLocalizedString = u"";
			stringCount++;
		}
		if (resource->mLoseText.mNonLocalizedString != u"") {
			RemoveNewLines(resource->mLoseText.mNonLocalizedString);
			data.append_sprintf(u"# Lose text\n");
			data.append_sprintf(u"%#010x %ls\n\n", stringCount, resource->mLoseText.mNonLocalizedString);
			resource->mLoseText.mLocalizedStringInstanceID = stringCount;
			resource->mLoseText.mLocalizedStringTableID = id(name.c_str());
			resource->mLoseText.mNonLocalizedString = u"";
			stringCount++;
		}
		auto& acts = resource->mActs;
		auto& classes = resource->mClasses;
		uint32_t actCount = 1;
		uint32_t goalCount = 1;

		for (auto& act : acts) {
			// Act number
			data.append_sprintf(u"# Act %d\n",actCount);

			// Act name
			if (act.mName.mNonLocalizedString != u"") {
				data.append_sprintf(u"%#010x %ls\n", stringCount, act.mName.mNonLocalizedString);
				act.mName.mLocalizedStringInstanceID = stringCount;
				act.mName.mLocalizedStringTableID = id(name.c_str());
				act.mName.mNonLocalizedString = u"";
				stringCount++;
			}

			// Act description
			if (act.mDescription.mNonLocalizedString != u"") {
				RemoveNewLines(act.mDescription.mNonLocalizedString);
				data.append_sprintf(u"%#010x %ls\n\n", stringCount, act.mDescription.mNonLocalizedString);
				act.mDescription.mLocalizedStringInstanceID = stringCount;
				act.mDescription.mLocalizedStringTableID = id(name.c_str());
				act.mDescription.mNonLocalizedString = u"";
				stringCount++;
			}

			// Act goals
			goalCount = 1;

			for (auto& goal : act.mGoals) {
				if (goal.mType == Simulator::ScenarioGoalType::TalkTo) {
					data.append_sprintf(u"# Goal %d\n", goalCount);
					for (auto& dialog : goal.mDialogs) {
						if (dialog.mText.mNonLocalizedString != u"") {
							RemoveNewLines(dialog.mText.mNonLocalizedString);
							data.append_sprintf(u"%#010x %ls\n", stringCount, dialog.mText.mNonLocalizedString);
							dialog.mText.mLocalizedStringInstanceID = stringCount;
							dialog.mText.mLocalizedStringTableID = id(name.c_str());
							dialog.mText.mNonLocalizedString = u"";
							stringCount++;
						}
					}
					data.append_sprintf(u"\n");
				}
				goalCount++;
			}
			data.append_sprintf(u"\n");

			actCount++;
		}

		uint32_t classCount = 1;

		for (auto& classObject : classes) {
			// Object name
			if (classObject.second.mCastName.mNonLocalizedString != u"") {
				data.append_sprintf(u"# Prop #%u - %ls\n%#010x %ls\n\n", classCount, classObject.second.mCastName.mNonLocalizedString.c_str(), stringCount, classObject.second.mCastName.mNonLocalizedString.c_str());
				classObject.second.mCastName.mLocalizedStringInstanceID = stringCount;
				classObject.second.mCastName.mLocalizedStringTableID = id(name.c_str());
				classObject.second.mCastName.mNonLocalizedString = u"";
			}
			else {
				cAssetMetadataPtr metadata;
				PropertyListPtr propList;
				LocalizedString str;
				if (Pollinator::GetMetadata(classObject.second.mAsset.mKey.instanceID, classObject.second.mAsset.mKey.groupID, metadata)) {
					data.append_sprintf(u"# Prop #%u - %ls\n%#010x %ls\n\n", classCount, metadata->GetName().c_str(), stringCount, metadata->GetName().c_str());
					classObject.second.mCastName.mLocalizedStringInstanceID = stringCount;
					classObject.second.mCastName.mLocalizedStringTableID = id(name.c_str());
				}
				else if (PropManager.GetPropertyList(classObject.second.mAsset.mKey.instanceID, classObject.second.mAsset.mKey.groupID, propList)
					&& App::Property::GetText(propList.get(), 0x071E9BD1, str))
				{
					data.append_sprintf(u"# Prop #%u - %ls\n%#010x %ls\n\n", classCount, str.GetText(),stringCount, str.GetText());
					classObject.second.mCastName.mLocalizedStringInstanceID = stringCount;
					classObject.second.mCastName.mLocalizedStringTableID = id(name.c_str());
				}
				else {
					App::ConsolePrintF("Name data for asset %u could not be found - Leaving name blank.", classCount);
					data.append_sprintf(u"# Prop #%u\n%#010x \n\n", classCount,stringCount);
					classObject.second.mCastName.mLocalizedStringInstanceID = stringCount;
					classObject.second.mCastName.mLocalizedStringTableID = id(name.c_str());
				}
			}
			stringCount++;
			actCount = 1;
			Simulator::cScenarioClassAct previousAct = classObject.second.mActs[0];
			Simulator::cScenarioClassAct currentAct = previousAct;
			Simulator::cScenarioClassAct previousActAfterLocalization;

			for (auto& act : classObject.second.mActs) {
				bool foundChatter = false, foundInspect = false;

				if (actCount > 1) {
					previousAct = currentAct;
					currentAct = act;
				}

				// Check if chatter and inspect bubbles are filled.
				for (auto& chatter : act.mDialogsChatter) {
					if (chatter.mText.mNonLocalizedString != u"") {
						foundChatter = true;
						break;
					}
				}

				for (auto& inspect : act.mDialogsInspect) {
					if (inspect.mText.mNonLocalizedString != u"") {
						foundInspect = true;
						break;
					}
				}

				int dialogueIndex = 0;

				// Object chatter
				if (foundChatter) {
					data.append_sprintf(u"# Chatter for Act %d\n", actCount);

					for (auto& chatter : act.mDialogsChatter) {
						if (chatter.mText.mNonLocalizedString != u"") {
							if (actCount > 1 && chatter.mText.mNonLocalizedString == previousAct.mDialogsChatter[dialogueIndex].mText.mNonLocalizedString) {
								chatter.mText.mLocalizedStringInstanceID = previousActAfterLocalization.mDialogsChatter[dialogueIndex].mText.mLocalizedStringInstanceID;
								chatter.mText.mLocalizedStringTableID = id(name.c_str());
								chatter.mText.mNonLocalizedString = u"";
							}
							else {
								RemoveNewLines(chatter.mText.mNonLocalizedString);
								data.append_sprintf(u"%#010x %ls\n", stringCount, chatter.mText.mNonLocalizedString);
								chatter.mText.mLocalizedStringInstanceID = stringCount;
								chatter.mText.mLocalizedStringTableID = id(name.c_str());
								chatter.mText.mNonLocalizedString = u"";
								stringCount++;
							}
							
						}
						dialogueIndex++;
					}
					data.append_sprintf(u"\n");
				}

				dialogueIndex = 0;

				// Object inspect
				if (foundInspect) {
					data.append_sprintf(u"# Inspect for Act %d\n", actCount);

					for (auto& inspect : act.mDialogsInspect) {
						if (inspect.mText.mNonLocalizedString != u"") {
							if (actCount > 1 && inspect.mText.mNonLocalizedString == previousAct.mDialogsInspect[dialogueIndex].mText.mNonLocalizedString) {
								inspect.mText.mLocalizedStringInstanceID = previousActAfterLocalization.mDialogsInspect[dialogueIndex].mText.mLocalizedStringInstanceID;
								inspect.mText.mLocalizedStringTableID = id(name.c_str());
								inspect.mText.mNonLocalizedString = u"";
							}
							else {
								RemoveNewLines(inspect.mText.mNonLocalizedString);
								data.append_sprintf(u"%#010x %ls\n", stringCount, inspect.mText.mNonLocalizedString);
								inspect.mText.mLocalizedStringInstanceID = stringCount;
								inspect.mText.mLocalizedStringTableID = id(name.c_str());
								inspect.mText.mNonLocalizedString = u"";
								stringCount++;
							}
						}
						dialogueIndex++;
					}
					data.append_sprintf(u"\n");
				}
				previousActAfterLocalization = act;
				actCount++;
			}
			classCount++;
		}

		ScenarioMode.GetData()->CommitHistoryEntry();
		
		// The following two lines of code change our script into UTF-8 encoding, which Spore can read later when it is loaded into a database file.
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
		std::string utf8string = conv.to_bytes(data.c_str());


		FileStreamPtr stream = new IO::FileStream(filename.c_str());
		if (stream->Open(IO::AccessFlags::ReadWrite,IO::CD::CreateAlways)) {
			stream->Write(utf8string.c_str(),utf8string.size());
			stream->Close();
			App::ConsolePrintF("Locale file written successfully. It can be found at %ls\nID: %#X", path.c_str(), id(name.c_str()));
			App::ConsolePrintF("Save the adventure, quit the game, find the resulted locale file and pack it into a mod with SporeModder FX for the localization to take effect.\nNOTE: MAKE SURE to NOT change the locale file name unless to its FNV hash!");
		}
		
	}
	else App::ConsolePrintF("You have to be in adventure edit mode to use this cheat.");
}

void LocalizeAdventure::RemoveNewLines(string16& string) {
	//string.replace(string.begin(),string.end(),u"\n",u"~br~");
	
	for (size_t i = 0; i < string.length(); i++) {
		if (string.substr(i, 1) == u"\n") {
			string.erase(i,1);
			string.insert(i,u"~br~");
		}
		else if (string.substr(i,2) == u"\r\n") {
			string.erase(i,2);
			string.insert(i, u"~br~");
		}
	}
}

const char* LocalizeAdventure::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Converts an adventure into a localized version.";
	}
	else {
		return "LocalizeAdventure: Converts adventure's text into a localized version, and saves a locale file (formatted UTF-16) containing the text to the hard drive. Requires saving the adventure afterwards.";
	}
}


