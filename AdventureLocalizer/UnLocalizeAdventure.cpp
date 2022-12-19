#include "stdafx.h"
#include "UnLocalizeAdventure.h"

UnLocalizeAdventure::UnLocalizeAdventure()
{
}


UnLocalizeAdventure::~UnLocalizeAdventure()
{
}


void UnLocalizeAdventure::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	if (Simulator::IsScenarioMode() && ScenarioMode.GetMode() == App::cScenarioMode::Mode::EditMode) {
		ScenarioMode.GetData()->StartHistoryEntry();
		cScenarioResourcePtr resource = ScenarioMode.GetResource();
		if (resource->mIntroText.mLocalizedStringInstanceID != 0 && resource->mIntroText.mLocalizedStringTableID != 0) {
			resource->mIntroText.mNonLocalizedString = resource->mIntroText.mString.GetText();
			resource->mIntroText.mLocalizedStringInstanceID = 0;
			resource->mIntroText.mLocalizedStringTableID = 0;
		}
		if (resource->mWinText.mLocalizedStringInstanceID != 0 && resource->mWinText.mLocalizedStringTableID != 0) {
			resource->mWinText.mNonLocalizedString = resource->mWinText.mString.GetText();
			resource->mWinText.mLocalizedStringInstanceID = 0;
			resource->mWinText.mLocalizedStringTableID = 0;
		}
		if (resource->mLoseText.mLocalizedStringInstanceID != 0 && resource->mLoseText.mLocalizedStringTableID != 0) {
			resource->mLoseText.mNonLocalizedString = resource->mLoseText.mString.GetText();
			resource->mLoseText.mLocalizedStringInstanceID = 0;
			resource->mLoseText.mLocalizedStringTableID = 0;
		}
		auto& acts = resource->mActs;
		auto& classes = resource->mClasses;

		for (auto& act : acts) {
			if (act.mName.mLocalizedStringInstanceID != 0 && act.mName.mLocalizedStringTableID != 0) {
				act.mName.mNonLocalizedString = act.mName.mString.GetText();
				act.mName.mLocalizedStringInstanceID = 0;
				act.mName.mLocalizedStringTableID = 0;
			}
			if (act.mDescription.mLocalizedStringInstanceID != 0 && act.mDescription.mLocalizedStringTableID != 0) {
				act.mDescription.mNonLocalizedString = act.mDescription.mString.GetText();
				act.mDescription.mLocalizedStringInstanceID = 0;
				act.mDescription.mLocalizedStringTableID = 0;
			}

			for (auto& goal : act.mGoals) {
				for (auto& dialog : goal.mDialogs) {
					if (dialog.mText.mLocalizedStringInstanceID != 0 && dialog.mText.mLocalizedStringTableID != 0) {
						dialog.mText.mNonLocalizedString = dialog.mText.mString.GetText();
						dialog.mText.mLocalizedStringInstanceID = 0;
						dialog.mText.mLocalizedStringTableID = 0;
					}
				}
			}
		}

		for (auto& classObject : classes) {
			if (classObject.second.mCastName.mLocalizedStringInstanceID != 0 && classObject.second.mCastName.mLocalizedStringTableID != 0) {
				classObject.second.mCastName.mNonLocalizedString = classObject.second.mCastName.mString.GetText();
				classObject.second.mCastName.mLocalizedStringInstanceID = 0;
				classObject.second.mCastName.mLocalizedStringTableID = 0;
			}

			for (auto& act : classObject.second.mActs) {
				for (auto& chatter : act.mDialogsChatter) {
					if (chatter.mText.mLocalizedStringInstanceID != 0 && chatter.mText.mLocalizedStringTableID != 0) {
						chatter.mText.mNonLocalizedString = chatter.mText.mString.GetText();
						chatter.mText.mLocalizedStringInstanceID = 0;
						chatter.mText.mLocalizedStringTableID = 0;
					}
				}

				for (auto& inspect : act.mDialogsInspect) {
					if (inspect.mText.mLocalizedStringInstanceID != 0 && inspect.mText.mLocalizedStringTableID != 0) {
						inspect.mText.mNonLocalizedString = inspect.mText.mString.GetText();
						inspect.mText.mLocalizedStringInstanceID = 0;
						inspect.mText.mLocalizedStringTableID = 0;
					}
				}

			}

		}
		ScenarioMode.GetData()->CommitHistoryEntry();
		App::ConsolePrintF("Adventure successfully unlocalized. Save the adventure for it to take effect.");
	}
	else App::ConsolePrintF("You have to be in adventure edit mode to use this cheat.");
}

const char* UnLocalizeAdventure::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "UnLocalizeAdventure: Elaborate description of what this cheat does.";
	}
}
