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

// void CheckBoxStringStringListBox::GetCheckBoxColumnData(std::vector<bool>& data)
// {
// }

// void CheckBoxStringStringListBox::GetMiddleColumnData(std::vector<std::string>& data)
// {
// }

// void CheckBoxStringStringListBox::GetRightColumnData(std::vector<std::string>& data)
// {
// }

// void CheckBoxStringStringListBox::SetCheckBoxColumnData(const std::vector<bool>& data)
// {
// }

// void CheckBoxStringStringListBox::SetMiddleColumnData(const std::vector<std::string>& data)
// {
// }

// void CheckBoxStringStringListBox::SetRightColumnData(const std::vector<std::string>& data)
// {
// }

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

// void CheckBoxStringStringListBox::SetLeftColumnTitle(const std::string& title)
// {
//     GetColumn(0)->SetTitle(title);
// }

// void CheckBoxStringStringListBox::SetMiddleColumnTitle(const std::string& title)
// {
//     GetColumn(1)->SetTitle(title);
// }

// void CheckBoxStringStringListBox::SetRightColumnTitle(const std::string& title)
// {
//     GetColumn(2)->SetTitle(title);
// }

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

std::string ChannelOffsetData::GetItemAsString()
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
    for (auto it : m_data) {
        it.SetSelectedValue(true);
    }
}

void ChannelOffsetDataList::UnselectAllItems()
{
    for (auto it : m_data) {
        it.SetSelectedValue(false);
    }
}

void ChannelOffsetDataList::SetOffsetOnSelectedItems(int offset)
{
    for (auto it : m_data) {
        if (it.GetSelectedValue()) {
            it.SetOffset(offset);
        }
    }
}

void ChannelOffsetDataList::ResetOffsetOnSelectedItems()
{
    for (auto it : m_data) {
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
    for (auto listItem : m_data) {
        std::cout << listItem.GetItemAsString() << std::endl;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

ChannelOffsetListBox::ChannelOffsetListBox(wxWindow *parent, const wxPoint& position, const wxSize& size) :
CheckBoxStringStringListBox(parent, position, size, 60, 150, 90, "Apply\nOffset", "Channel Name", "Current\nOffset")
{
}

void ChannelOffsetListBox::SetData(const ChannelOffsetDataList &data)
{
    unsigned int size = data.GetSize();
    std::vector<bool> checkBoxData(size);
    std::vector<std::string> channelNameData(size);
    std::vector<std::string> offsetData(size);
    data.GetColumnData(checkBoxData, channelNameData, offsetData);
    
    for (auto it : channelNameData) {
        std::cout << "channelNameData at row is: " << it << std::endl;
    }

    CheckBoxStringStringListBox::SetData(checkBoxData, channelNameData, offsetData);
    m_data = data;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////

ChannelOffsetDataPanel::ChannelOffsetDataPanel(MainFrame *parent, const wxPoint &position, const std::string& labelText, MainControl* control) :
wxPanel(parent, wxID_ANY, position, wxSize(300, 520)),
m_labelText(labelText),
m_label(nullptr),
m_channelListBox(nullptr),
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

    m_label = new wxStaticText(this, wxID_ANY, m_labelText, wxPoint(0, 0));
    m_channelListBox = new ChannelOffsetListBox(this, wxPoint(0, 32), wxSize(300, 400));

    m_offsetInputLabel = new wxStaticText(this, wxID_ANY, "New Offset: ", wxPoint(0, 440), wxSize(80, 32), wxALIGN_CENTER_VERTICAL);
    m_offsetInputText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(80, 432), wxSize(64, 32));
    m_offsetInputText->SetMaxLength(5); // Allow for negative sign and 4 digits TODO: Make this range magnitude equal on both sides
    m_applySelectedButton = new wxButton(this, ChannelOffsetPanelButtonIds::IdApplyToSelected, "Apply to Selected", wxPoint(160, 432));
    m_applyAllButton = new wxButton(this, ChannelOffsetPanelButtonIds::IdApplyToAll, "Apply to All", wxPoint(160, 472));
}

void ChannelOffsetDataPanel::BindControls()
{
    m_applySelectedButton->Bind(wxEVT_BUTTON, &ChannelOffsetDataPanel::OnApplySelectedButtonClicked, this);
    m_applySelectedButton->Bind(wxEVT_MOTION, &ChannelOffsetDataPanel::OnSelectedButtonMouseOver, this);

    m_applyAllButton->Bind(wxEVT_BUTTON, &ChannelOffsetDataPanel::OnApplyAllButtonClicked, this);
    m_applyAllButton->Bind(wxEVT_MOTION, &ChannelOffsetDataPanel::OnApplyAllButtonMouseOver, this);

    m_offsetInputText->Bind(wxEVT_MOTION, &ChannelOffsetDataPanel::OnOffsetInputTextMouseOver, this);
}

void ChannelOffsetDataPanel::OnApplySelectedButtonClicked(wxCommandEvent& evt)
{
    HandleCommonButtonClickTasks(evt);
}

void ChannelOffsetDataPanel::OnSelectedButtonMouseOver(wxEvent &evt)
{
    m_frame->SetStatusText("Apply offset to selected channels only");
}

void ChannelOffsetDataPanel::OnApplyAllButtonClicked(wxCommandEvent &evt)
{
    HandleCommonButtonClickTasks(evt);
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

void ChannelOffsetDataPanel::HandleCommonButtonClickTasks(wxCommandEvent& evt)
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
