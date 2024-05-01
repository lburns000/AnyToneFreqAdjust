#ifndef CHANNELOFFSETLISTBOX_H
#define CHANNELOFFSETLISTBOX_H


#include <wx/wx.h>
#include <wx/dataview.h>

#include <string>
#include <vector>

/**A custom type of wxDataViewListCtrl that has 3 columns: The first column is a checkbox column 
 * for selecting rows, the second and third columns are meant to be used as name-value pairs for 
 * representing data.*/
class CheckBoxStringStringListBox : public wxDataViewListCtrl
{
public:
    CheckBoxStringStringListBox(wxWindow* parent, const wxPoint& position, const wxSize& size, 
        int leftColumnWidth = -1, int middleColumnWidth = -1, int rightcolumnWidth = -1,
        const std::string& leftColumnTitle = "", const std::string& middleColumnTitle = "", 
        const std::string& rightColumnTitle = "");
    wxDataViewListStore* GetData() { return GetStore(); }
    void SetData(const std::vector<bool>& checkBoxData, const std::vector<std::string>& middleColumnData, const std::vector<std::string>& rightColumnData);
    bool GetCheckBoxAtRow(unsigned int row);
    std::string GetMiddleColumnValueAtRow(unsigned int row);
    std::string GetRightColumnValueAtRow(unsigned int row);
    void SetCheckBoxValueAtRow(unsigned int row, bool value);
    void SetMiddleColumnValueAtRow(unsigned int row, const std::string& value);
    void SetRightColumnValueAtRow(unsigned int row, const std::string& value);

protected:
    void AddColumns();    

    std::vector<bool> m_checkBoxData;
    std::vector<std::string> m_middleColumnData;
    std::vector<std::string> m_rightColumnData;
    std::string m_leftTitle;
    int m_leftWidth;
    std::string m_middleTitle;
    int m_middleWidth;
    std::string m_rightTitle;
    int m_rightWidth;
};

/**A channel offset object has 3 parts: The channel name, the offset, and whether it's 
 * selected or not*/
class ChannelOffsetData
{
public:
    ChannelOffsetData();
    ChannelOffsetData(bool selected, std::string name, int offset);
    
    bool GetSelectedValue() const { return m_selected; }
    std::string GetName() const { return m_name; }
    int GetOffset() const { return m_offset; }
    void SetSelectedValue(bool selected);
    void SetName(const std::string& name);
    void SetOffset(int offset);

    std::string GetItemAsString() const;

private:
    bool m_selected;
    std::string m_name;
    int m_offset;
};

/**A list of ChannelOffsetData objects*/
class ChannelOffsetDataList
{
public:
    ChannelOffsetDataList();
    ChannelOffsetDataList& operator=(const ChannelOffsetDataList& other);
    void AddItem(const ChannelOffsetData& item);
    void SelectAllItems();
    void UnselectAllItems();
    void SetOffsetOnSelectedItems(int offset);
    void ResetOffsetOnSelectedItems();
    void GetColumnData(std::vector<bool>& checkBoxData, std::vector<std::string>& channelNameData, 
        std::vector<std::string>& offsetData) const;
    void SetColumnData(const std::vector<bool>& checkBoxData, const std::vector<std::string>& channelNameData, 
        const std::vector<std::string>& offsetData);
    void GetOneRowData(unsigned int row, bool& checkBoxValue, std::string& channelNameValue, std::string& offsetValue);
    void SetOneRowData(unsigned int row, bool checkBoxValue, const std::string& channelNameValue, const std::string& offsetValue);
    unsigned int GetSize() const { return m_data.size(); }
    ChannelOffsetData& GetItemAtIndex(unsigned int index);
    void SetItemAtIndex(unsigned int index, const ChannelOffsetData& item);
    void RemoveAllItems();

    void PrintToConsole();

private:
    std::vector<ChannelOffsetData> m_data;
};

/**A specific version of the CheckBoxStringStringListBox class that has been customized for 
 * representing a ChannelOffsetDataList, each row representing a single ChannelOffsetData object.*/
class ChannelOffsetListBox : public CheckBoxStringStringListBox
{
public:
    ChannelOffsetListBox(wxWindow* parent, const wxPoint& position, const wxSize& size);

    void SetData(const ChannelOffsetDataList& data);
    void GetData(ChannelOffsetDataList& data);    
    void SelectAll() override;
    void UnselectAll() override;

private:
    void UpdateData();

    ChannelOffsetDataList m_data;
};

enum ChannelOffsetPanelButtonIds
{
    IdApplyToSelected,
    IdApplyToAll,
    IdSelectAll,
    IdSelectNone,
    IdReset
};

class MainFrame;
class MainControl;

/**A wxPanel that contains a title, custom listbox, input text, and button.*/
class ChannelOffsetDataPanel : public wxPanel
{
public:
    ChannelOffsetDataPanel(MainFrame* parent, const wxPoint& position, const std::string& labelText, MainControl* control);
    ~ChannelOffsetDataPanel();

    void GetChannelOffsetData(ChannelOffsetDataList& data);
    void SetChannelOffsetData(const ChannelOffsetDataList& data);

    void PrintDataToConsole();

private:
    void DrawControls();
    void BindControls();

    void OnSelectAllButtonClicked(wxCommandEvent& evt);
    void OnSelectAllButtonMouseOver(wxEvent& evt);
    void OnSelectNoneButtonClicked(wxCommandEvent& evt);
    void OnSelectNoneButtonMouseOver(wxEvent& evt);
    void OnResetButtonClicked(wxCommandEvent& evt);
    void OnResetButtonMouseOver(wxEvent& evt);
    void OnApplySelectedButtonClicked(wxCommandEvent& evt);
    void OnSelectedButtonMouseOver(wxEvent& evt);
    void OnApplyAllButtonClicked(wxCommandEvent& evt);
    void OnApplyAllButtonMouseOver(wxEvent& evt);
    void OnOffsetInputTextMouseOver(wxEvent& evt);

    /**Get input text and sanitize it to be a valid integer.*/
    int GetOffsetInput();

    void HandleApplyButtonClickTasks(wxCommandEvent& evt);
    void HandleSelectButtonClickTasks(wxCommandEvent& evt);
    bool CheckInputValid(int offset);
    void HandleInvalidInput(int offset);

    std::string           m_labelText;
    wxStaticText*         m_label;
    ChannelOffsetListBox* m_channelListBox;
    wxButton*             m_selectAllButton;
    wxButton*             m_selectNoneButton;
    wxButton*             m_resetButton;
    wxStaticText*         m_offsetInputLabel;
    wxTextCtrl*           m_offsetInputText;
    wxButton*             m_applySelectedButton;
    wxButton*             m_applyAllButton;
    MainFrame*            m_frame;
    MainControl*          m_control;
};

#endif