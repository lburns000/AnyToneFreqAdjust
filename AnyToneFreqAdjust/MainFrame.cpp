#include "MainFrame.h"



MainControl* MainControl::mainControlPtr = nullptr;

MainControl *MainControl::GetMainControl()
{
    if (mainControlPtr == nullptr) {
        mainControlPtr = new MainControl();
    }

    return mainControlPtr;
}

bool MainControl::OpenFile(const std::string &filename)
{
    m_file = new CSVFile(filename);

    if (!m_file->CategoryExists("Channel Name")) {
        delete m_file;
        m_file = nullptr;
        return false;
    }
    if (!m_file->CategoryExists("Transmit Frequency")) {
        delete m_file;
        m_file = nullptr;
        return false;
    }    
    if (!m_file->CategoryExists("Correct Frequency[Hz]")) {
        delete m_file;
        m_file = nullptr;
        return false;
    }
    if (!m_file->IsValid()) {
        delete m_file;
        m_file = nullptr;
        return false;
    }

    return true;
}

void MainControl::GetFileData(ChannelOffsetDataList& vhfData, ChannelOffsetDataList& uhfData)
{
    std::vector<std::string> fileChannelNameData = m_file->GetItemsInCategory("Channel Name");
    std::cout << "Channel Name vector has size: " << fileChannelNameData.size() << std::endl;
    std::vector<std::string> fileTxFreqData = m_file->GetItemsInCategory("Transmit Frequency");
    std::cout << "Tx Freq vector has size: " << fileTxFreqData.size() << std::endl;
    std::vector<std::string> fileOffsetData = m_file->GetItemsInCategory("Correct Frequency[Hz]");
    std::cout << "Offset vector has size: " << fileOffsetData.size() << std::endl;

    assert(fileChannelNameData.size() == fileTxFreqData.size() && fileChannelNameData.size() == fileOffsetData.size());

    vhfData.RemoveAllItems();
    uhfData.RemoveAllItems();

    for (unsigned int i = 0; i < fileChannelNameData.size(); ++i) {
        ChannelOffsetData data;
        data.SetSelectedValue(false);
        data.SetName(fileChannelNameData.at(i));
        data.SetOffset(std::stoi(fileOffsetData.at(i)));
        double freq = std::stod(fileTxFreqData.at(i));
        if (freq >= 330.0) {
            uhfData.AddItem(data);
        }
        else {
            vhfData.AddItem(data);
        }
    }
}

void MainControl::SetFileData(const ChannelOffsetDataList& vhfData, const ChannelOffsetDataList& uhfData)
{
    std::vector<bool> vhfChannelSelectedData;
    std::vector<std::string> vhfChannelNameData;
    std::vector<std::string> vhfChannelOffsetData;
    std::vector<bool> uhfChannelSelectedData;
    std::vector<std::string> uhfChannelNameData;
    std::vector<std::string> uhfChannelOffsetData;

    vhfData.GetColumnData(vhfChannelSelectedData, vhfChannelNameData, vhfChannelOffsetData);
    uhfData.GetColumnData(uhfChannelSelectedData, uhfChannelNameData, uhfChannelOffsetData);

    std::vector<std::pair<std::string, std::string>> allChannelNameOffsetPairs;

    for (unsigned int i = 0; i < vhfData.GetSize(); ++i) {
        if (vhfChannelSelectedData.at(i) == false)
            continue;
        allChannelNameOffsetPairs.push_back(std::make_pair(vhfChannelNameData.at(i), vhfChannelOffsetData.at(i)));
    }

    for (unsigned int i = 0; i < uhfData.GetSize(); ++i) {
        if (uhfChannelSelectedData.at(i) == false)
            continue;
        allChannelNameOffsetPairs.push_back(std::make_pair(uhfChannelNameData.at(i), uhfChannelOffsetData.at(i)));
    }

    for (const auto& it : allChannelNameOffsetPairs) {
        m_file->SetOneValueInRow(it.first, "Correct Frequency[Hz]", it.second);
    }
}

bool MainControl::SaveFile()
{
    return m_file->WriteChanges();
}

bool MainControl::SaveFileAs(const std::string& filename)
{
    m_file->Rename(filename);
    return m_file->WriteChanges();
}

void MainControl::ShowMessage(const std::string &message, const std::string& title)
{
    wxMessageBox(message, title);
}

void MainControl::ShowWarning(const std::string &warning, const std::string& title)
{
    wxMessageBox(warning, title, wxICON_WARNING);
}

void MainControl::ShowError(const std::string &error, const std::string& title)
{
    wxMessageBox(error, title, wxICON_ERROR);
}

MainControl::MainControl() :
m_file(nullptr)
{
}

MainControl::~MainControl()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

FilePanel::FilePanel(MainFrame* parent, const wxPoint& position, MainControl* control) :
wxPanel(parent, wxID_ANY, position, wxSize(332, 100)),
m_openFileButton(nullptr),
m_saveFileButton(nullptr),
m_saveFileAsButton(nullptr),
m_control(control),
m_frame(parent)
{
    assert(m_control != nullptr);
    assert(m_frame != nullptr);

    DrawControls();
    BindControls();
    DisableSaveButtons();
}

FilePanel::~FilePanel()
{
}

void FilePanel::OnOpenFileButtonClicked(wxCommandEvent &event)
{
    wxFileDialog* openDialog = new wxFileDialog(this, "Open Channel CSV File", "", "", "CSV Files (*.csv;*.CSV)|*.csv;*.CSV", wxFD_OPEN);

    if (openDialog->ShowModal() == wxID_CANCEL)
        return;

    if (m_control->OpenFile(openDialog->GetPath().ToStdString())) {
        std::cout << "Opened CSV file successfully" << std::endl;
        ChannelOffsetDataList vhfData, uhfData;
        m_control->GetFileData(vhfData, uhfData);
        m_frame->SetChannelOffsetData(vhfData, uhfData);
        EnableSaveButtons();
    }
    else {
        m_control->ShowError("Invalid CSV File!");
    }
}

void FilePanel::OnSaveFileButtonClicked(wxCommandEvent &event)
{
    wxMessageDialog confirmSaveDialog(this, "Overwrite file with new offset data?", "Save File?", wxOK | wxCANCEL);
    if (confirmSaveDialog.ShowModal() == wxID_OK) {
        ChannelOffsetDataList vhfData, uhfData;
        m_frame->GetChannelOffsetData(vhfData, uhfData);
        m_control->SetFileData(vhfData, uhfData);
        m_control->SaveFile();
    }
}

void FilePanel::OnSaveAsFileButtonClicked(wxCommandEvent &event)
{
    ChannelOffsetDataList vhfData, uhfData;
    m_frame->GetChannelOffsetData(vhfData, uhfData);
    m_control->SetFileData(vhfData, uhfData);

    wxFileDialog* saveAsDialog = new wxFileDialog(this, "Save Channel CSV File As...", "", "", "CSV Files (*.csv;*.CSV)|*.csv;*.CSV", wxFD_SAVE);
    
    if (saveAsDialog->ShowModal() == wxID_CANCEL)
        return;

    std::string path = saveAsDialog->GetPath().ToStdString();
    if (path.length() > 0)
        m_control->SaveFileAs(path);
}

void FilePanel::OnOpenFileButtonMouseOver(wxEvent &event)
{
    m_frame->SetStatusText("Open a channel CSV file that was exported from the CPS");
}

void FilePanel::OnSaveFileButtonMouseOver(wxEvent &event)
{
    m_frame->SetStatusText("Save new offsets in file. This will overwrite the data in the current file.");
}

void FilePanel::OnSaveAsFileButtonMouseOver(wxEvent &event)
{
    m_frame->SetStatusText("Save new offsets in a different file");
}

void FilePanel::DrawControls()
{
    m_openFileButton = new wxButton(this, wxID_ANY, "Open File...", wxPoint(32, 32), wxSize(100, 32));
    m_saveFileButton = new wxButton(this, wxID_ANY, "Save File...", wxPoint(132, 32), wxSize(100, 32));    
    m_saveFileAsButton = new wxButton(this, wxID_ANY, "Save As...", wxPoint(232, 32), wxSize(100, 32));
    
}

void FilePanel::BindControls()
{
    m_openFileButton->Bind(wxEVT_BUTTON, &FilePanel::OnOpenFileButtonClicked, this);
    m_openFileButton->Bind(wxEVT_MOTION, &FilePanel::OnOpenFileButtonMouseOver, this);

    m_saveFileButton->Bind(wxEVT_BUTTON, &FilePanel::OnSaveFileButtonClicked, this);
    m_saveFileButton->Bind(wxEVT_MOTION, &FilePanel::OnSaveFileButtonMouseOver, this);

    m_saveFileAsButton->Bind(wxEVT_BUTTON, &FilePanel::OnSaveAsFileButtonClicked, this);
    m_saveFileAsButton->Bind(wxEVT_MOTION, &FilePanel::OnSaveAsFileButtonMouseOver, this);
}

void FilePanel::EnableSaveButtons()
{
    m_saveFileButton->Enable();
    m_saveFileAsButton->Enable();
}

void FilePanel::DisableSaveButtons()
{
    m_saveFileButton->Disable();
    m_saveFileAsButton->Disable();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame(const wxString& title) :
wxFrame(nullptr, wxID_ANY, title),
m_filePanel(nullptr),
m_vhfPanel(nullptr),
m_uhfPanel(nullptr),
m_control(nullptr)
{
    m_control = MainControl::GetMainControl();
    DrawWidgets();
    BindWidgets();
}

MainFrame::~MainFrame()
{

}

void MainFrame::GetChannelOffsetData(ChannelOffsetDataList &vhfData, ChannelOffsetDataList &uhfData)
{
    m_vhfPanel->GetChannelOffsetData(vhfData);
    m_uhfPanel->GetChannelOffsetData(uhfData);
}

void MainFrame::SetChannelOffsetData(const ChannelOffsetDataList &vhfData, const ChannelOffsetDataList &uhfData)
{
    m_vhfPanel->SetChannelOffsetData(vhfData);
    m_uhfPanel->SetChannelOffsetData(uhfData);
}

void MainFrame::DrawWidgets()
{
    m_filePanel = new FilePanel(this, wxPoint(0, 600), m_control);
    m_vhfPanel = new ChannelOffsetDataPanel(this, wxPoint(32, 16), "VHF Channels", m_control);
    m_uhfPanel = new ChannelOffsetDataPanel(this, wxPoint(364, 16), "UHF Channels", m_control);

    CreateStatusBar(1);
    SetStatusText("Open an AnyTone 578/868/878/878II channel CSV file and select channels to modify offsets on");
}

void MainFrame::BindWidgets()
{
    m_vhfPanel->Bind(wxEVT_BUTTON, &MainFrame::OnVhfPanelButtonClicked, this);
    m_uhfPanel->Bind(wxEVT_BUTTON, &MainFrame::OnUhfPanelButtonClicked, this);
    this->Bind(wxEVT_MOTION, &MainFrame::OnMouseLeaveItem, this);
    m_vhfPanel->Bind(wxEVT_MOTION, &MainFrame::OnMouseLeaveItem, this);
    m_uhfPanel->Bind(wxEVT_MOTION, &MainFrame::OnMouseLeaveItem, this);
}

void MainFrame::OnVhfPanelButtonClicked(wxCommandEvent &event)
{
    int vhfOffset = event.GetInt();
    std::cout << "MainFrame processing button press event with ID: " << event.GetId() << " and int: " << vhfOffset << std::endl;

    if (event.GetId() == ChannelOffsetPanelButtonIds::IdApplyToSelected)
        UpdateSelectedVhfData(vhfOffset);
    else if (event.GetId() == ChannelOffsetPanelButtonIds::IdApplyToAll)
        UpdateAllVhfData(vhfOffset);

    m_vhfPanel->PrintDataToConsole();
}

void MainFrame::OnUhfPanelButtonClicked(wxCommandEvent &event)
{
    int uhfOffset = event.GetInt();
    std::cout << "MainFrame processing button press event with ID: " << event.GetId() << " and int: " << uhfOffset << std::endl;

    if (event.GetId() == ChannelOffsetPanelButtonIds::IdApplyToSelected)
        UpdateSelectedUhfData(uhfOffset);
    else if (event.GetId() == ChannelOffsetPanelButtonIds::IdApplyToAll)
        UpdateAllUhfData(uhfOffset);
    
    m_uhfPanel->PrintDataToConsole();
}

void MainFrame::OnMouseLeaveItem(wxEvent &event)
{
    SetStatusText("");
}

void MainFrame::UpdateSelectedVhfData(int offset)
{
    ChannelOffsetDataList data;
    m_vhfPanel->GetChannelOffsetData(data);

    for (unsigned int i = 0; i < data.GetSize(); ++i) {
        if (data.GetItemAtIndex(i).GetSelectedValue() == true) {
            data.GetItemAtIndex(i).SetOffset(offset);
        }
    }

    m_vhfPanel->SetChannelOffsetData(data);
}

void MainFrame::UpdateSelectedUhfData(int offset)
{
    ChannelOffsetDataList data;
    m_uhfPanel->GetChannelOffsetData(data);

    for (unsigned int i = 0; i < data.GetSize(); ++i) {
        if (data.GetItemAtIndex(i).GetSelectedValue() == true) {
            data.GetItemAtIndex(i).SetOffset(offset);
        }
    }

    m_uhfPanel->SetChannelOffsetData(data);
}

void MainFrame::UpdateAllVhfData(int offset)
{
    ChannelOffsetDataList data;
    m_vhfPanel->GetChannelOffsetData(data);

    for (unsigned int i = 0; i < data.GetSize(); ++i) {
        data.GetItemAtIndex(i).SetOffset(offset);
    }

    m_vhfPanel->SetChannelOffsetData(data);
}

void MainFrame::UpdateAllUhfData(int offset)
{
    ChannelOffsetDataList data;
    m_uhfPanel->GetChannelOffsetData(data);

    for (unsigned int i = 0; i < data.GetSize(); ++i) {
        data.GetItemAtIndex(i).SetOffset(offset);
    }

    m_uhfPanel->SetChannelOffsetData(data);
}
