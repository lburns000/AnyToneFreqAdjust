#include "ChannelOffsetListBox.h"
#include "MainFrame.h"
#include <cassert>

CheckBoxStringStringListBox::CheckBoxStringStringListBox(wxWindow *parent, const wxPoint& position, 
    const wxSize& size, int leftColumnWidth, int middleColumnWidth, int rightColumnWidth,
        const std::string& leftColumnTitle, const std::string& middleColumnTitle, 
        const std::string& rightColumnTitle) : 
wxDataViewListCtrl(parent, wxID_ANY, position, size),
m_checkBoxData(),
m_middleColumnData(),
m_rightColumnData(),
m_leftTitle(leftColumnTitle),
m_leftWidth(leftColumnWidth),
m_middleTitle(middleColumnTitle),
m_middleWidth(middleColumnWidth),
m_rightTitle(rightColumnTitle),
m_rightWidth(rightColumnWidth)
{
    AddColumns();
}

void CheckBoxStringStringListBox::SetData(const std::vector<bool>& checkBoxData, const std::vector<std::string>& middleColumnData, 
    const std::vector<std::string>& rightColumnData)
{
    assert(checkBoxData.size() == middleColumnData.size() && checkBoxData.size() == rightColumnData.size());

    GetStore()->DeleteAllItems();

    for (unsigned int i = 0; i < checkBoxData.size(); ++i) {
        wxVector<wxVariant> rowData(3);
        rowData[0] = wxVariant(checkBoxData[i]);
        rowData[1] = wxVariant(middleColumnData[i]);
        rowData[2] = wxVariant(rightColumnData[i]);
        AppendItem(rowData);
    }
}

bool CheckBoxStringStringListBox::GetCheckBoxAtRow(unsigned int row)
{
    return GetToggleValue(row, 0);
}

std::string CheckBoxStringStringListBox::GetMiddleColumnValueAtRow(unsigned int row)
{
    return GetTextValue(row, 1).ToStdString();
}

std::string CheckBoxStringStringListBox::GetRightColumnValueAtRow(unsigned int row)
{
    return GetTextValue(row, 2).ToStdString();
}

void CheckBoxStringStringListBox::SetCheckBoxValueAtRow(unsigned int row, bool value)
{
    SetToggleValue(value, row, 0);
}

void CheckBoxStringStringListBox::SetMiddleColumnValueAtRow(unsigned int row, const std::string& value)
{
    SetTextValue(value, row, 1);
}

void CheckBoxStringStringListBox::SetRightColumnValueAtRow(unsigned int row, const std::string& value)
{
    SetTextValue(value, row, 2);
}

void CheckBoxStringStringListBox::AddColumns()
{
    AppendToggleColumn(m_leftTitle, wxDATAVIEW_CELL_ACTIVATABLE, m_leftWidth, wxALIGN_CENTER_HORIZONTAL);
    AppendTextColumn(m_middleTitle, wxDATAVIEW_CELL_INERT, m_middleWidth, wxALIGN_LEFT);
    AppendTextColumn(m_rightTitle, wxDATAVIEW_CELL_INERT, m_rightWidth, wxALIGN_CENTER_HORIZONTAL);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ChannelOffsetData::ChannelOffsetData() :
m_selected(false),
m_name(""),
m_offset(0)
{
}

ChannelOffsetData::ChannelOffsetData(bool selected, std::string name, int offset) : 
m_selected(selected),
m_name(name),
m_offset(offset)
{
}

void ChannelOffsetData::SetSelectedValue(bool selected)
{
    m_selected = selected;
}

void ChannelOffsetData::SetName(const std::string &name)
{
    m_name = name;
}

void ChannelOffsetData::SetOffset(int offset)
{
    m_offset = offset;
}

std::string ChannelOffsetData::GetItemAsString() const
{
    return std::string((m_selected ? "True" : "False")) + std::string(", ") + m_name + std::string(", ") + std::to_string(m_offset);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

ChannelOffsetDataList::ChannelOffsetDataList()
{
}

ChannelOffsetDataList &ChannelOffsetDataList::operator=(const ChannelOffsetDataList &other)
{
    m_data = other.m_data;
    return *this;
}

void ChannelOffsetDataList::AddItem(const ChannelOffsetData &item)
{
    // std::cout << "Adding item " << item.GetName() << std::endl;
    m_data.push_back(item);
}

void ChannelOffsetDataList::SelectAllItems()
{
    for (auto& it : m_data) {
        it.SetSelectedValue(true);
    }
}

void ChannelOffsetDataList::UnselectAllItems()
{
    for (auto& it : m_data) {
        it.SetSelectedValue(false);
    }
}

void ChannelOffsetDataList::SetOffsetOnSelectedItems(int offset)
{
    for (auto& it : m_data) {
        if (it.GetSelectedValue()) {
            it.SetOffset(offset);
        }
    }
}

void ChannelOffsetDataList::ResetOffsetOnSelectedItems()
{
    for (auto& it : m_data) {
        if (it.GetSelectedValue()) {
            it.SetOffset(0);
        }
    }
}

void ChannelOffsetDataList::GetColumnData(std::vector<bool> &checkBoxData, std::vector<std::string> &channelNameData, std::vector<std::string> &offsetData) const
{
    unsigned int size = m_data.size();
    checkBoxData.resize(size, false);
    channelNameData.resize(size, "");
    offsetData.resize(size, "");
    for (unsigned int i = 0; i < size; ++i) {
        std::cout << "Getting data for row " << i << std::endl;
        checkBoxData[i] = m_data[i].GetSelectedValue();
        std::cout << "Checkbox data is: " << (checkBoxData[i] ? "True" : "False") << std::endl;
        channelNameData[i] = m_data[i].GetName();
        std::cout << "Channel name data: " << channelNameData[i] << std::endl;
        offsetData[i] = std::to_string(m_data[i].GetOffset());
        std::cout << "Offset: " << offsetData[i] << std::endl;
    }
}

void ChannelOffsetDataList::SetColumnData(const std::vector<bool> &checkBoxData, const std::vector<std::string> &channelNameData, const std::vector<std::string> &offsetData)
{
    unsigned int size = checkBoxData.size();
    assert(size == channelNameData.size() && size == offsetData.size());

    for (unsigned int i = 0; i < size; ++i) {
        m_data.push_back(ChannelOffsetData(checkBoxData[i], channelNameData[i], std::stoi(offsetData[i])));
    }
}

void ChannelOffsetDataList::GetOneRowData(unsigned int row, bool &checkBoxValue, std::string &channelNameValue, std::string &offsetValue)
{
}

void ChannelOffsetDataList::SetOneRowData(unsigned int row, bool checkBoxValue, const std::string &channelNameValue, const std::string &offsetValue)
{
}

ChannelOffsetData& ChannelOffsetDataList::GetItemAtIndex(unsigned int index)
{
    return m_data.at(index);
}

void ChannelOffsetDataList::SetItemAtIndex(unsigned int index, const ChannelOffsetData &item)
{
    m_data[index] = item;
}

void ChannelOffsetDataList::RemoveAllItems()
{
    m_data.clear();
}

void ChannelOffsetDataList::PrintToConsole()
{
    for (const auto& listItem : m_data) {
        std::cout << listItem.GetItemAsString() << std::endl;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

ChannelOffsetListBox::ChannelOffsetListBox(wxWindow *parent, const wxPoint& position, const wxSize& size) :
CheckBoxStringStringListBox(parent, position, size, 100, 180, 120, "Apply Offset", "Channel Name", "Current Offset")
{
}

void ChannelOffsetListBox::SetData(const ChannelOffsetDataList &data)
{
    m_data = data;
    UpdateData();
}

void ChannelOffsetListBox::GetData(ChannelOffsetDataList &data)
{
    for (unsigned int i = 0; i < m_data.GetSize(); ++i) {
        bool checked = GetCheckBoxAtRow(i);
        std::string name = GetMiddleColumnValueAtRow(i);
        int offset = std::stoi(GetRightColumnValueAtRow(i));
        m_data.SetItemAtIndex(i, {checked, name, offset});
    }
    
    data = m_data;
}

void ChannelOffsetListBox::UpdateData()
{
    unsigned int size = m_data.GetSize();
    std::vector<bool> checkBoxData(size);
    std::vector<std::string> channelNameData(size);
    std::vector<std::string> offsetData(size);
    m_data.GetColumnData(checkBoxData, channelNameData, offsetData);
    
    for (const auto& it : channelNameData) {
        std::cout << "channelNameData at row is: " << it << std::endl;
    }

    CheckBoxStringStringListBox::SetData(checkBoxData, channelNameData, offsetData);
}

void ChannelOffsetListBox::SelectAll()
{
    m_data.SelectAllItems();
    UpdateData();
}

void ChannelOffsetListBox::UnselectAll()
{
    m_data.UnselectAllItems();
    UpdateData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

ChannelOffsetDataPanel::ChannelOffsetDataPanel(MainFrame *parent, const wxPoint &position, const std::string& labelText, MainControl* control) :
wxPanel(parent, wxID_ANY, position, wxSize(400, 520)),
m_labelText(labelText),
m_label(nullptr),
m_channelListBox(nullptr),
m_selectAllButton(nullptr),
m_selectNoneButton(nullptr),
m_resetButton(nullptr),
m_offsetInputLabel(nullptr),
m_offsetInputText(nullptr),
m_applySelectedButton(nullptr),
m_applyAllButton(nullptr),
m_frame(parent),
m_control(control)
{
    assert(m_frame != nullptr);
    assert(m_control != nullptr);

    DrawControls();
    BindControls();
}

ChannelOffsetDataPanel::~ChannelOffsetDataPanel()
{
}

void ChannelOffsetDataPanel::GetChannelOffsetData(ChannelOffsetDataList &data)
{
    m_channelListBox->GetData(data);
}

void ChannelOffsetDataPanel::SetChannelOffsetData(const ChannelOffsetDataList &data)
{
    m_channelListBox->SetData(data);
}

void ChannelOffsetDataPanel::PrintDataToConsole()
{
    ChannelOffsetDataList data;
    m_channelListBox->GetData(data);
    data.PrintToConsole();
}

void ChannelOffsetDataPanel::DrawControls()
{
    // TODO: Use a wxBoxSizer here for better fit

    m_label = new wxStaticText(this, wxID_ANY, m_labelText, wxPoint(0, 12));
    m_channelListBox = new ChannelOffsetListBox(this, wxPoint(0, 32), wxSize(400, 400));
    m_selectAllButton = new wxButton(this, ChannelOffsetPanelButtonIds::IdSelectAll, "Select All", wxPoint(0, 432), wxSize(144, 32));
    m_selectNoneButton = new wxButton(this, ChannelOffsetPanelButtonIds::IdSelectNone, "Select None", wxPoint(160, 432), wxSize(128, 32));
    m_resetButton = new wxButton(this, ChannelOffsetPanelButtonIds::IdReset, "Reset", wxPoint(300, 432), wxSize(100, 32));
    m_offsetInputLabel = new wxStaticText(this, wxID_ANY, "New Offset: ", wxPoint(0, 476), wxSize(80, 32), wxALIGN_CENTER_VERTICAL);
    m_offsetInputText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(80, 468), wxSize(64, 32));
    m_offsetInputText->SetMaxLength(5); // Allow for negative sign and 4 digits TODO: Make this range magnitude equal on both sides
    m_applySelectedButton = new wxButton(this, ChannelOffsetPanelButtonIds::IdApplyToSelected, "Apply to Selected", wxPoint(160, 468), wxSize(128, 32));
    m_applyAllButton = new wxButton(this, ChannelOffsetPanelButtonIds::IdApplyToAll, "Apply to All", wxPoint(300, 468), wxSize(100, 32));

    // For testing
    // SetBackgroundColour(wxColour(0x00ff00));
}

void ChannelOffsetDataPanel::BindControls()
{
    m_selectAllButton->Bind(wxEVT_BUTTON, &ChannelOffsetDataPanel::OnSelectAllButtonClicked, this);
    m_selectAllButton->Bind(wxEVT_MOTION, &ChannelOffsetDataPanel::OnSelectAllButtonMouseOver, this);

    m_selectNoneButton->Bind(wxEVT_BUTTON, &ChannelOffsetDataPanel::OnSelectNoneButtonClicked, this);
    m_selectNoneButton->Bind(wxEVT_MOTION, &ChannelOffsetDataPanel::OnSelectNoneButtonMouseOver, this);

    m_resetButton->Bind(wxEVT_BUTTON, &ChannelOffsetDataPanel::OnResetButtonClicked, this);
    m_resetButton->Bind(wxEVT_MOTION, &ChannelOffsetDataPanel::OnResetButtonMouseOver, this);

    m_applySelectedButton->Bind(wxEVT_BUTTON, &ChannelOffsetDataPanel::OnApplySelectedButtonClicked, this);
    m_applySelectedButton->Bind(wxEVT_MOTION, &ChannelOffsetDataPanel::OnSelectedButtonMouseOver, this);

    m_applyAllButton->Bind(wxEVT_BUTTON, &ChannelOffsetDataPanel::OnApplyAllButtonClicked, this);
    m_applyAllButton->Bind(wxEVT_MOTION, &ChannelOffsetDataPanel::OnApplyAllButtonMouseOver, this);

    m_offsetInputText->Bind(wxEVT_MOTION, &ChannelOffsetDataPanel::OnOffsetInputTextMouseOver, this);
}

void ChannelOffsetDataPanel::OnSelectAllButtonClicked(wxCommandEvent &evt)
{
    HandleSelectButtonClickTasks(evt);
}

void ChannelOffsetDataPanel::OnSelectAllButtonMouseOver(wxEvent &evt)
{
    m_frame->SetStatusText("Select all channels");
}

void ChannelOffsetDataPanel::OnSelectNoneButtonClicked(wxCommandEvent &evt)
{
    HandleSelectButtonClickTasks(evt);
}

void ChannelOffsetDataPanel::OnSelectNoneButtonMouseOver(wxEvent &evt)
{
    m_frame->SetStatusText("Unselect all channels");
}

void ChannelOffsetDataPanel::OnResetButtonClicked(wxCommandEvent &evt)
{
    int offset = 0;
    evt.SetInt(offset);
    evt.Skip();
}

void ChannelOffsetDataPanel::OnResetButtonMouseOver(wxEvent &evt)
{
    m_frame->SetStatusText("Reset offset on all channels in list");
}

void ChannelOffsetDataPanel::OnApplySelectedButtonClicked(wxCommandEvent& evt)
{
    HandleApplyButtonClickTasks(evt);
}

void ChannelOffsetDataPanel::OnSelectedButtonMouseOver(wxEvent &evt)
{
    m_frame->SetStatusText("Apply offset to selected channels only");
}

void ChannelOffsetDataPanel::OnApplyAllButtonClicked(wxCommandEvent &evt)
{
    HandleApplyButtonClickTasks(evt);
}

void ChannelOffsetDataPanel::OnApplyAllButtonMouseOver(wxEvent &evt)
{
    m_frame->SetStatusText("Apply offset to all channels in this list");
}

void ChannelOffsetDataPanel::OnOffsetInputTextMouseOver(wxEvent &evt)
{
    m_frame->SetStatusText("Enter an offset value. Valid range is [-9999, 99999]");
}

int ChannelOffsetDataPanel::GetOffsetInput()
{
    std::string input = m_offsetInputText->GetLineText(0).ToStdString();
    std::cout << "Entered offset string: " << input << std::endl;

    // TODO: Better sanitation of input, this works for now. Simply set offset value to higher than it can possibly be.

    int offset = 0;

    try {
        offset = std::stoi(input);
    }
    catch (...) {
        offset = 0x7FFFFFFF;
    }
    
    return offset;
}

void ChannelOffsetDataPanel::HandleApplyButtonClickTasks(wxCommandEvent& evt)
{
    if (m_offsetInputText->GetLineLength(0) < 1)
        return;

    int offset = GetOffsetInput();
    m_offsetInputText->Clear();

    if (!CheckInputValid(offset)) {
        HandleInvalidInput(offset);
        return;
    }

    evt.SetInt(offset);
    evt.Skip();
}

void ChannelOffsetDataPanel::HandleSelectButtonClickTasks(wxCommandEvent &evt)
{
    auto id = evt.GetId();

    if (id == ChannelOffsetPanelButtonIds::IdSelectAll)
        m_channelListBox->SelectAll();
    else if (id == ChannelOffsetPanelButtonIds::IdSelectNone)
        m_channelListBox->UnselectAll();
}

bool ChannelOffsetDataPanel::CheckInputValid(int offset)
{
    if (offset > 99999 || offset < -9999)
        return false;

    return true;
}

void ChannelOffsetDataPanel::HandleInvalidInput(int offset)
{
    // The only way the input can be outside of this 5-character range is if there was an error
    if (offset > 99999 || offset < -9999) {
        m_control->ShowError("Invalid input!");
        std::cerr << "Invalid input!" << std::endl;
        return;
    }
}
