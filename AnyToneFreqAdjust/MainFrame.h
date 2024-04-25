#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/dataview.h>

#include "ChannelOffsetListBox.h"
#include "CSVFile.h"


class MainControl
{
public:
    MainControl(const MainControl& other) = delete;

    static MainControl* GetMainControl();

    bool OpenFile(const std::string& filename);

    /**Read through all the file data and populate the VHF/UHF ChannelOffsetDataLists*/
    void GetFileData(ChannelOffsetDataList& vhfData, ChannelOffsetDataList& uhfData);

    /**Go through the given VHF/UHF ChannelOffsetDataLists and populate the matching channels in 
       the file with the given offset data*/
    void SetFileData(const ChannelOffsetDataList& vhfData, const ChannelOffsetDataList& uhfData);

    /**Write the file in its current state to disk.*/
    bool SaveFile();

    /**Simply change the filename so the file on disk is different, but the data is the same.*/
    bool SaveFileAs(const std::string& filename);

    void ShowMessage(const std::string& message, const std::string& title = "");
    void ShowWarning(const std::string& warning, const std::string& title = "Warning");
    void ShowError(const std::string& error, const std::string& title = "Error");

protected:    
    MainControl();
    ~MainControl();

    static MainControl* mainControlPtr;

private:
    CSVFile* m_file;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class MainFrame;

class FilePanel : public wxPanel
{
public:
    FilePanel(MainFrame* parent, const wxPoint& position, MainControl* control);
    ~FilePanel();

private:
    void OnOpenFileButtonClicked(wxCommandEvent& event);
    void OnSaveFileButtonClicked(wxCommandEvent& event);
    void OnSaveAsFileButtonClicked(wxCommandEvent& event);
    void OnOpenFileButtonMouseOver(wxEvent& event);
    void OnSaveFileButtonMouseOver(wxEvent& event);
    void OnSaveAsFileButtonMouseOver(wxEvent& event);

    void DrawControls();
    void BindControls();

    void EnableSaveButtons();
    void DisableSaveButtons();

    wxButton*    m_openFileButton;
    wxButton*    m_saveFileButton;
    wxButton*    m_saveFileAsButton;
    MainControl* m_control;
    MainFrame*   m_frame;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class MainFrame : public wxFrame
{
    public:
    MainFrame(const wxString& title);
    ~MainFrame();

    void GetChannelOffsetData(ChannelOffsetDataList& vhfData, ChannelOffsetDataList& uhfData);
    void SetChannelOffsetData(const ChannelOffsetDataList& vhfData, const ChannelOffsetDataList& uhfData);

    private:
    void DrawWidgets();
    void BindWidgets();
    void OnVhfPanelButtonClicked(wxCommandEvent& event);
    void OnUhfPanelButtonClicked(wxCommandEvent& event);
    void OnMouseLeaveItem(wxEvent& event);
    void UpdateSelectedVhfData(int offset);    
    void UpdateSelectedUhfData(int offset);
    void UpdateAllVhfData(int offset);
    void UpdateAllUhfData(int offset);

    FilePanel*               m_filePanel;
    ChannelOffsetDataPanel*  m_vhfPanel;
    ChannelOffsetDataPanel*  m_uhfPanel;
    MainControl*             m_control;
};





















#endif