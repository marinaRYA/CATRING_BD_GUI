#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include "MenuBuilder.h"
#include "PresenterDish.h"
#pragma once
class Frame : public wxFrame {
public:
    Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size, wxDEFAULT_FRAME_STYLE & ~(wxMAXIMIZE_BOX | wxRESIZE_BORDER)) {

        // Создание вертикального контейнера для размещения элементов
       

        MenuBuilder builder;
        SetMenuBar(builder.menu);

     
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        //mainSizer->Add(dataGrid, 1, wxEXPAND | wxALL, 5);

        // Создание поисковой строки
        wxTextCtrl* searchCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        mainSizer->Add(searchCtrl, 0, wxEXPAND | wxALL, 20);

        // Создание кнопок "Поиск", "Добавить", "Редактировать" и "Удалить"
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* searchButton = new wxButton(this, wxID_ANY, "Поиск");
        wxButton* addButton = new wxButton(this, wxID_ANY, "Добавить");
        wxButton* editButton = new wxButton(this, wxID_ANY, "Редактировать");
        wxButton* deleteButton = new wxButton(this, wxID_ANY, "Удалить");

        buttonSizer->Add(searchButton, 0, wxALL, 5);
        buttonSizer->Add(addButton, 0, wxALL, 5);
        buttonSizer->Add(editButton, 0, wxALL, 5);
        buttonSizer->Add(deleteButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);

        SetSizer(mainSizer);
        Layout();

        // Привязка событий
        Bind(wxEVT_BUTTON, &Frame::OnSearchButtonClick, this, searchButton->GetId());
        Bind(wxEVT_BUTTON, &Frame::OnAddButtonClick, this, addButton->GetId());
        Bind(wxEVT_BUTTON, &Frame::OnEditButtonClick, this, editButton->GetId());
        Bind(wxEVT_BUTTON, &Frame::OnDeleteButtonClick, this, deleteButton->GetId());
        Bind(wxEVT_TEXT_ENTER, &Frame::OnSearchEnter, this, searchCtrl->GetId());
    }
    
private:
    void OnSearchButtonClick(wxCommandEvent& event) {
       
    }

    void OnAddButtonClick(wxCommandEvent& event) {
       
    }

    void OnEditButtonClick(wxCommandEvent& event) {
      
    }

    void OnDeleteButtonClick(wxCommandEvent& event) {
        
    }

    void OnSearchEnter(wxCommandEvent& event) {
       
    }
};



