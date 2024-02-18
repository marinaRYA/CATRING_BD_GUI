#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/grid.h>
#include "DishBd.h"
#include "TableStrategy.h"
class DishTableStrategy : public TableStrategy {
private:
    wxWindow* _view;
    wxGrid* _dishGrid;
    DishBd _dishBd;

public:
    DishTableStrategy(wxWindow* view, const std::string& bd)
        : _dishBd(bd), _view(view) {
        
        
    }
    void LoadData() override
    {
        wxGrid* grid = new wxGrid(_view, wxID_ANY);
        grid->SetColLabelValue(0, "Вид блюда");
        grid->CreateGrid(_dishBd.Dishes.size(), 1);
        for (int row = 0; row < _dishBd.Dishes.size(); row++)
            grid->SetCellValue(row, 0, wxString(_dishBd.Dishes[row].Name));
    }
    void AddRecord() override {
        wxString dishName = GetDishNameFromUser("Введите название блюда:");

        if (!dishName.IsEmpty()) {
            Dish newDish;
            newDish.Name = dishName.ToStdString();

            _dishBd.AddDish(newDish);
            UpdateGrid();
        }
    }

    void EditRecord() override {
        int selectedRow = _dishGrid->GetSelectedRows()[0];
        if (selectedRow != wxNOT_FOUND) {
            wxString currentDishName = _dishGrid->GetCellValue(selectedRow, 1);

            wxString newDishName = GetDishNameFromUser("Введите новое название блюда:", currentDishName);

            if (!newDishName.IsEmpty()) {
                Dish updatedDish;
                updatedDish.DishID = std::stoi(_dishGrid->GetCellValue(selectedRow, 0).ToStdString());
                updatedDish.Name = newDishName.ToStdString();

                _dishBd.UpdateDish(updatedDish);
                UpdateGrid();
            }
        }
    }

    void DeleteRecord() override {
        
        int selectedRow = _dishGrid->GetSelectedRows()[0];
        if (selectedRow != wxNOT_FOUND) {
            wxMessageDialog confirmDialog(_view, "Точно хотите удалить блюдо?", "Подтверждение", wxYES_NO | wxICON_QUESTION);
            if (confirmDialog.ShowModal() == wxID_YES) {
                int dishID = std::stoi(_dishGrid->GetCellValue(selectedRow, 0).ToStdString());
                try {
                    _dishBd.DeleteDish(dishID);
                    UpdateGrid();
                }
                catch (const std::exception& ex) {
                    wxMessageBox(wxString::FromUTF8("Ошибка при удалении блюда: ") + wxString::FromUTF8(ex.what()), "Ошибка", wxICON_ERROR | wxOK);
                }
            }
        }
    }

    void Search(const wxString& searchTerm)  override {
      //  std::vector<DishBd::Dish> filteredList = _dishBd.SearchDishes(searchTerm.ToStdString());

    //        UpdateGrid(filteredList);
    }

private:
    void UpdateGrid(const std::vector<Dish>& dishes = {}) {
        _dishGrid->DeleteRows(0, _dishGrid->GetNumberRows());
        _dishGrid->AppendRows(dishes.size());

        for (size_t i = 0; i < dishes.size(); ++i) {
            _dishGrid->SetCellValue(i, 0, std::to_string(dishes[i].DishID));
            _dishGrid->SetCellValue(i, 1, wxString::FromUTF8(dishes[i].Name.c_str()));
        }

        _dishGrid->ForceRefresh();
    }

    wxString GetDishNameFromUser(const wxString& prompt, const wxString& defaultValue = "") {
        wxTextEntryDialog dialog(_view, prompt, "User Input", defaultValue);
        if (dialog.ShowModal() == wxID_OK) {
            return dialog.GetValue();
        }
        return wxEmptyString;
    }
};
