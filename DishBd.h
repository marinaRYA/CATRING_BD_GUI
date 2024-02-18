#include "sqlite3.h"
#include <vector>
#include <string>
#include <wx/msgdlg.h>
struct Dish {
    int DishID;
    std::string Name;
};
class DishBd {
public:
   

    std::vector<Dish> Dishes;

    DishBd(const std::string& bdName) {
        connectionString = "Data Source=" + bdName + ";Version=3;";
        Dishes = GetDishData();
    }

    void AddDish(Dish& newDish) {
        std::string insertQuery = "INSERT INTO Dish (Name) VALUES (?);";
        std::string selectIdQuery = "SELECT last_insert_rowid();";

        try {
            sqlite3* db;
            int rc = sqlite3_open(connectionString.c_str(), &db);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при добавлении типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            sqlite3_stmt* stmt;
            rc = sqlite3_prepare_v2(db, insertQuery.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при добавлении типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            rc = sqlite3_bind_text(stmt, 1, newDish.Name.c_str(), -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при добавлении типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                wxMessageBox(wxString::FromUTF8("Ошибка при добавлении типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            sqlite3_finalize(stmt);

            
            sqlite3_prepare_v2(db, selectIdQuery.c_str(), -1, &stmt, nullptr);
            newDish.DishID = sqlite3_column_int64(stmt, 0);


            sqlite3_finalize(stmt);

            sqlite3_close(db);

            Dishes.push_back(newDish);
        }
        catch (const std::exception& ex) {
            wxMessageBox(wxString::FromUTF8("Error adding dish: ") + wxString::FromUTF8(ex.what()), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
        }
    }

    void UpdateDish(const Dish& updatedDish) {
        std::string updateQuery = "UPDATE Dish SET Name = ? WHERE DishID = ?;";

        try {
            sqlite3* db;
            int rc = sqlite3_open(connectionString.c_str(), &db);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при редактировании типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            sqlite3_stmt* stmt;
            rc = sqlite3_prepare_v2(db, updateQuery.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при редактировании типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            rc = sqlite3_bind_text(stmt, 1, updatedDish.Name.c_str(), -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при редактировании типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            rc = sqlite3_bind_int(stmt, 2, updatedDish.DishID);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при редактировании типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                wxMessageBox(wxString::FromUTF8("Ошибка при редактировании типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            // Update the dish in the vector
            auto it = std::find_if(Dishes.begin(), Dishes.end(), [&updatedDish](const Dish& dish) {
                return dish.DishID == updatedDish.DishID;
                });

            if (it != Dishes.end()) {
                *it = updatedDish;
            }
        }
        catch (const std::exception& ex) {
            wxMessageBox(wxString::FromUTF8("Error updating dish: ") + wxString::FromUTF8(ex.what()), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
        }
    }

    void DeleteDish(int dishId) {
        std::string deleteQuery = "DELETE FROM Dish WHERE DishID = ?;";

        try {
            sqlite3* db;
            int rc = sqlite3_open(connectionString.c_str(), &db);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при удалении типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            sqlite3_stmt* stmt;
            rc = sqlite3_prepare_v2(db, deleteQuery.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при удалении типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            rc = sqlite3_bind_int(stmt, 1, dishId);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при удалении типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) { wxMessageBox(wxString::FromUTF8("Ошибка при удалении типа блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK); }

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            
            Dishes.erase(std::remove_if(Dishes.begin(), Dishes.end(), [dishId](const Dish& dish) {
                return dish.DishID == dishId;
                }), Dishes.end());
        }
        catch (const std::exception& ex) {
            wxMessageBox(wxString::FromUTF8("Ошибка при удалении блюда "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
        }
    }

private:
    std::string connectionString;

    std::vector<Dish> GetDishData() {
        std::vector<Dish> dishes;
        std::string query = "SELECT * FROM Dish;";

        try {
            sqlite3* db;
            int rc = sqlite3_open(connectionString.c_str(), &db);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при получении данных "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            sqlite3_stmt* stmt;
            rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                wxMessageBox(wxString::FromUTF8("Ошибка при получении "), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
            }

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                Dish dish;
                dish.DishID = sqlite3_column_int(stmt, 0);
                dish.Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

                dishes.push_back(dish);
            }

            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
        catch (const std::exception& ex) {
            wxMessageBox(wxString::FromUTF8("Error getting dish data: ") + wxString::FromUTF8(ex.what()), wxString::FromUTF8("Error"), wxICON_ERROR | wxOK);
        }

        return dishes;
    }
};


