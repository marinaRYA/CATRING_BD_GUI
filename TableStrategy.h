#pragma once
class TableStrategy {
public:
    virtual void LoadData() = 0;
    virtual void AddRecord() = 0;
    virtual void EditRecord() = 0;
    virtual void DeleteRecord() = 0;
    virtual void Search(const wxString&) = 0;
};