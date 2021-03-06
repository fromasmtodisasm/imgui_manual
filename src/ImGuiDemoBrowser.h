#pragma once
#include "WindowWithEditor.h"
#include "source_parse/Sources.h"
#include "source_parse/GuiHeaderTree.h"

// This is the window that shows imgui_demo.cpp code,
// with a callback that will point to the correct line number
// (see implImGuiDemoCallbackDemoCallback)
class ImGuiDemoBrowser: public WindowWithEditor
{
public:
    ImGuiDemoBrowser();
    void gui();

private:
    void guiHelp();
    void guiSave();
    void guiGithubButton();
    void guiDemoCodeTags();

    SourceParse::AnnotatedSource mAnnotatedSource;
    SourceParse::GuiHeaderTree mGuiHeaderTree;
};
