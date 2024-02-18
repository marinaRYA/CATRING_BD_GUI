#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/notebook.h> 
#include "sqlite3.h"
#include <string>
#include "Frame.h"
class LoginFrame : public wxFrame {
public:
    LoginFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

        
private:
    void OnLogin(wxCommandEvent& event);
    void OnRegister(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    wxTextCtrl* loginUsernameCtrl;
    wxTextCtrl* loginPasswordCtrl;
    wxTextCtrl* registerUsernameCtrl;
    wxTextCtrl* registerPasswordCtrl;
    wxTextCtrl* registerRepeatPasswordCtrl;

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_LoginButton = 1,
    ID_RegisterButton
};

wxBEGIN_EVENT_TABLE(LoginFrame, wxFrame)
EVT_BUTTON(ID_LoginButton, LoginFrame::OnLogin)
EVT_BUTTON(ID_RegisterButton, LoginFrame::OnRegister)
EVT_MENU(wxID_EXIT, LoginFrame::OnExit)
wxEND_EVENT_TABLE()

LoginFrame::LoginFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

  
    wxPanel* loginPanel = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer* loginBoxSizer = new wxBoxSizer(wxVERTICAL);

    loginBoxSizer->Add(new wxStaticText(loginPanel, wxID_ANY, "�����: "), 0, wxALIGN_LEFT | wxTOP, 10);
    loginUsernameCtrl = new wxTextCtrl(loginPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    loginBoxSizer->Add(loginUsernameCtrl, 0, wxEXPAND | wxALL, 10);

    loginBoxSizer->Add(new wxStaticText(loginPanel, wxID_ANY, "������: "), 0, wxALIGN_LEFT | wxTOP, 10);
    loginPasswordCtrl = new wxTextCtrl(loginPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    loginBoxSizer->Add(loginPasswordCtrl, 0, wxEXPAND | wxALL, 10);

    wxButton* loginButton = new wxButton(loginPanel, ID_LoginButton, "�����");
    loginBoxSizer->Add(loginButton, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 0);

    loginPanel->SetSizer(loginBoxSizer);
    notebook->AddPage(loginPanel, "�����������");

  
    wxPanel* registerPanel = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer* registerBoxSizer = new wxBoxSizer(wxVERTICAL);

    registerBoxSizer->Add(new wxStaticText(registerPanel, wxID_ANY, "�����: "), 0, wxALIGN_LEFT | wxTOP, 10);
    registerUsernameCtrl = new wxTextCtrl(registerPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    registerBoxSizer->Add(registerUsernameCtrl, 0, wxEXPAND | wxALL, 10);

    registerBoxSizer->Add(new wxStaticText(registerPanel, wxID_ANY, "����� ������: "), 0, wxALIGN_LEFT | wxTOP, 10);
    registerPasswordCtrl = new wxTextCtrl(registerPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    registerBoxSizer->Add(registerPasswordCtrl, 0, wxEXPAND | wxALL, 10);
    registerBoxSizer->Add(new wxStaticText(registerPanel, wxID_ANY, "��������� ������: "), 0, wxALIGN_LEFT | wxTOP, 10);
    registerRepeatPasswordCtrl = new wxTextCtrl(registerPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    registerBoxSizer->Add(registerRepeatPasswordCtrl, 0, wxEXPAND | wxALL, 10);
    wxButton* registerButton = new wxButton(registerPanel, ID_RegisterButton, "�����������");
    registerBoxSizer->Add(registerButton, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    registerPanel->SetSizer(registerBoxSizer);
    notebook->AddPage(registerPanel, "�����������");

   
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(notebook, 1, wxEXPAND);
    SetSizer(mainSizer);

    Centre();
}

void LoginFrame::OnLogin(wxCommandEvent& event) {
    wxString username = loginUsernameCtrl->GetValue();
    wxString password = loginPasswordCtrl->GetValue();

    // ��������� ���������� � ����� ������
    sqlite3* db;
    int rc = sqlite3_open("CAT_COMPANY.db", &db);
    if (rc) {
        wxMessageBox("������ �������� ���� ������", "������", wxICON_ERROR | wxOK);
        return;
    }


    wxString sql = "SELECT * FROM Users WHERE username = '" + username + "' AND password = '" + password + "';";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.ToStdString().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        wxMessageBox("������ ���������� SQL-�������", "������", wxICON_ERROR | wxOK);
        sqlite3_close(db);
        return;
    }

    // ��������� SQL-������
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // ���� ���� ������������, �� ����������� �������
        wxMessageBox("�������� �����������", "�����", wxICON_INFORMATION | wxOK);
        Frame* secondFrame = new Frame("����������� ������������� �������", wxPoint(100, 100), wxSize(800, 600));
        secondFrame->Show(true);
        Close(true);
    }
    else {
        wxMessageBox("��������� �����������", "������", wxICON_ERROR | wxOK);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
}


void LoginFrame::OnRegister(wxCommandEvent& event) {
    std::string newUsername = (registerUsernameCtrl->GetValue()).ToStdString();
    std::string newPassword = (registerPasswordCtrl->GetValue()).ToStdString();
    std::string repeatPassword = (registerRepeatPasswordCtrl->GetValue()).ToStdString();

    if (newUsername.empty() || newPassword.empty()) {
        wxMessageBox("������� ����� � ������.", "��������������", wxOK | wxICON_WARNING, this);
        return;
    }

    if (newPassword != repeatPassword) {
        wxMessageBox("������ �� ���������", "Error", wxOK | wxICON_ERROR, this);
        return;
    }

    try {
        sqlite3* db;
        int rc = sqlite3_open("CAT_COMPANY.db", &db);
        if (rc) {
            wxMessageBox("������ �������� ���� ������", "������", wxOK | wxICON_ERROR, this);
            return;
        }

        // ������� ������������ � ���� ������
        const char* insertQuery = "INSERT INTO Users (Username, Password) VALUES (?, ?)";
        sqlite3_stmt* stmt;
        rc = sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            wxMessageBox("������ ���������� SQL-�������", "������", wxOK | wxICON_ERROR, this);
            sqlite3_close(db);
            return;
        }

        // �������� �������� � ����������
        rc = sqlite3_bind_text(stmt, 1, newUsername.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            wxMessageBox("������ �������� ��������� Username", "������", wxOK | wxICON_ERROR, this);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        rc = sqlite3_bind_text(stmt, 2, newPassword.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            wxMessageBox("������ �������� ��������� Password", "������", wxOK | wxICON_ERROR, this);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        // ���������� �������
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            wxMessageBox("������������ � ����� ������ ��� ����������", "������", wxOK | wxICON_ERROR, this);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
        
        sqlite3_finalize(stmt);
        // ��������� ID ������ ������������
        int newUserId;
        std::string selectUserIdQuery = "SELECT User_ID FROM Users WHERE Username = ?;";
        rc = sqlite3_prepare_v2(db, selectUserIdQuery.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            wxMessageBox("������ ���������� SQL-�������", "������", wxOK | wxICON_ERROR, this);
            sqlite3_close(db);
            return;
        }

        rc = sqlite3_bind_text(stmt, 1, newUsername.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            wxMessageBox("������ �������� ���������", "������", wxOK | wxICON_ERROR, this);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            newUserId = sqlite3_column_int(stmt, 0);
        }
        else {
            wxMessageBox("������ ���������� SQL-�������", "������", wxOK | wxICON_ERROR, this);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        std:: string selectMenuIdsQuery = "SELECT Id FROM Menu;";
        std::vector<int> menuIds;

        rc = sqlite3_prepare_v2(db, selectMenuIdsQuery.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            wxMessageBox("������ ���������� SQL-�������", "������", wxOK | wxICON_ERROR, this);
            sqlite3_close(db);
            return;
        }

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            menuIds.push_back(sqlite3_column_int(stmt, 0));
        }

        sqlite3_finalize(stmt);
        std::string insertRightsQuery = "INSERT INTO User_Rights (User_ID, Menu_Item_ID, R, W, E, D) VALUES (?, ?, 0, 0, 0, 0);";
        // ������� ���� ��� ������� �������� ����
        for (int menuId : menuIds) {
           
            rc = sqlite3_prepare_v2(db, insertRightsQuery.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                wxMessageBox("������ ���������� SQL-�������", "������", wxOK | wxICON_ERROR, this);
                sqlite3_close(db);
                return;
            }

            rc = sqlite3_bind_int(stmt, 1, newUserId);
            if (rc != SQLITE_OK) {
                wxMessageBox("������ �������� ���������", "������", wxOK | wxICON_ERROR, this);
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return;
            }
            rc = sqlite3_bind_int(stmt, 2, menuId);
            if (rc != SQLITE_OK) {
                wxMessageBox("������ �������� ���������", "������", wxOK | wxICON_ERROR, this);
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return;
            }
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                wxMessageBox("������ ���������� SQL-�������", "������", wxOK | wxICON_ERROR, this);
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return;
            }
                sqlite3_reset(stmt);
            
        }
        
        sqlite3_finalize(stmt);

        sqlite3_close(db);

        wxMessageBox("������������ ������� ���������������.", "����������", wxOK | wxICON_INFORMATION, this);

    }
    catch (...) {
        wxMessageBox("������ ��� �����������", "������", wxOK | wxICON_ERROR, this);
        return;
    }
}


void LoginFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

bool MyApp::OnInit() {
    //LoginFrame* frame = new LoginFrame("Login App", wxPoint(50, 50), wxSize(400, 320));
    //frame->Show(true);
    Frame* secondFrame = new Frame("����������� ������������� �������", wxPoint(100, 100), wxSize(800, 600));
    secondFrame->Show(true);
    //Close(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
