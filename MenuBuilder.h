#pragma once
#include <wx/wx.h>
#include <wx/menu.h>
#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>


class MenuBuilder {
public:
    wxMenuBar* menu;
    MenuBuilder() {
        menu = BuildMenu();
    }

private:
    struct MenuItemData {
        int Id;
        int ParentMenuItemId;
        std::string MenuText;
        std::string DLLName;
        std::string FunctionName;
        int DisplayOrder;
    };

    std::vector<MenuItemData> Items;

    wxMenuBar* BuildMenu() {
        ReadBd();

        wxMenuBar* menuBar = new wxMenuBar;

        // ��������� �������� �� DisplayOrder
        std::sort(Items.begin(), Items.end(), [](const auto& a, const auto& b) {
            return a.DisplayOrder < b.DisplayOrder;
            });

        // ������� ��������� ��� �������
        std::map<int, wxMenu*> subMenus;

        for (const auto& item : Items) {
            if (item.ParentMenuItemId == 0) {
                // ��� ������� �������� ������
                wxMenu* topLevelMenu = new wxMenu;
                //topLevelMenu->Append(item.Id, wxString::FromUTF8(item.MenuText));
                menuBar->Append(topLevelMenu, wxString::FromUTF8(item.MenuText));

                // ��������� ������� � ���������
                subMenus[item.Id] = topLevelMenu;
            }
            else {
                // ��� �������
                auto parentMenuIt = subMenus.find(item.ParentMenuItemId);
                if (parentMenuIt != subMenus.end()) {
                    parentMenuIt->second->Append(item.Id, wxString::FromUTF8(item.MenuText));
                }
            }
        }

        return menuBar;
    }

    void ReadBd() {
        std::string connectionString = "CAT_COMPANY.db";
        std::string queryStr = "SELECT Id, ParentMenuItemId, MenuText, DLLName, FunctionName, DisplayOrder FROM Menu";
        sqlite3* db = nullptr;
        sqlite3_stmt* stmt = nullptr;

        int rc = sqlite3_open(connectionString.c_str(), &db);
        if (rc != SQLITE_OK) {
            wxMessageBox("������ ��� �������� ���� ������", "Error", wxOK | wxICON_ERROR);
            sqlite3_close(db);
            return;
        }

        rc = sqlite3_prepare_v2(db, queryStr.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            wxMessageBox("������ ��� �������� ���������", "Error", wxOK | wxICON_ERROR);
            sqlite3_close(db);
            return;
        }

        // ��������� SQL-������
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            MenuItemData menuItem = {
                sqlite3_column_int(stmt, 0),
                sqlite3_column_int(stmt, 1),
                (sqlite3_column_text(stmt, 2) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : ""),
                (sqlite3_column_text(stmt, 3) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : ""),
                (sqlite3_column_text(stmt, 4) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) : ""),
                sqlite3_column_int(stmt, 5)
            };

            Items.push_back(menuItem);
        }

        
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
};
