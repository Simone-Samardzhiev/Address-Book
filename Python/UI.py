from functools import partial

from PyQt6.QtGui import QCloseEvent

from Data import *
from PyQt6.QtWidgets import (QWidget, QGridLayout, QPushButton, QLabel, QMessageBox, QApplication, QLineEdit,
                             QVBoxLayout, QFrame, QScrollArea)


# Class that represent the window that is used to add a new address.
class AddAddressWindow(QWidget):
    def __init__(self, wnd: "MainWindow", data: Data) -> None:
        super().__init__()

        # setting attributes to the widow
        self.setGeometry(300, 300, 500, 500)
        self.setWindowTitle("Add new address")

        # passing the arguments
        self.wnd = wnd
        self.data = data

        # creating the widgets
        layout = QGridLayout()
        self.nameLineEdit = QLineEdit()
        self.phoneLineEdit = QLineEdit()
        self.emailLineEdit = QLineEdit()
        self.cityLineEdit = QLineEdit()
        self.neighbourhoodLineEdit = QLineEdit()
        self.apartmentBuildingLineEdit = QLineEdit()
        self.streetLineEdit = QLineEdit()
        self.floorLineEdit = QLineEdit()
        self.apartmentLineEdit = QLineEdit()
        add_address_button = QPushButton("Add Address")

        # setting attributes and connecting the widgets
        self.setLayout(layout)
        add_address_button.clicked.connect(self.on_add_address_button_clicked)

        # adding the widgets
        layout.addWidget(QLabel("Name:"), 0, 0)
        layout.addWidget(self.nameLineEdit, 0, 1)
        layout.addWidget(QLabel("Phone:"), 1, 0)
        layout.addWidget(self.phoneLineEdit, 1, 1)
        layout.addWidget(QLabel("Email:"), 2, 0)
        layout.addWidget(self.emailLineEdit, 2, 1)
        layout.addWidget(QLabel("City"), 3, 0)
        layout.addWidget(self.cityLineEdit, 3, 1)
        layout.addWidget(QLabel("Neighbourhood:"), 4, 0)
        layout.addWidget(self.neighbourhoodLineEdit, 4, 1)
        layout.addWidget(QLabel("Apartment building:"), 5, 0)
        layout.addWidget(self.apartmentBuildingLineEdit, 5, 1)
        layout.addWidget(QLabel("Street:"), 6, 0)
        layout.addWidget(self.streetLineEdit, 6, 1)
        layout.addWidget(QLabel("Floor:"), 7, 0)
        layout.addWidget(self.floorLineEdit, 7, 1)
        layout.addWidget(QLabel("Apartment:"), 8, 0)
        layout.addWidget(self.apartmentLineEdit, 8, 1)
        layout.addWidget(add_address_button, 9, 0)

        self.show()

    # Method that will be called when the add address button is clicked. It will add the address and close.
    def on_add_address_button_clicked(self) -> None:
        try:
            floor = int(self.floorLineEdit.text())
        except ValueError:
            QMessageBox.warning(self, "Error", "Please enter a valid floor")
            return
        try:
            apartment = int(self.apartmentLineEdit.text())
        except ValueError:
            QMessageBox.warning(self, "Error", "Please enter a apartment")
            return

        self.data.add_address(self.nameLineEdit.text(), self.phoneLineEdit.text(), self.emailLineEdit.text(),
                              self.cityLineEdit.text(), self.neighbourhoodLineEdit.text(),
                              self.apartmentBuildingLineEdit.text(), self.streetLineEdit.text(), floor, apartment)
        self.wnd.on_search()
        self.close()


# Class that will represent the window used for showing information about an address.
class AddressInfoWindow(QWidget):
    def __init__(self, wnd: "MainWindow", data: Data, address: Address) -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle(address.name)
        self.setGeometry(300, 300, 500, 500)

        # passing the arguments
        self.wnd = wnd
        self.address = address
        self.data = data

        # creating the widgets
        layout = QGridLayout()
        self.nameLineEdit = QLineEdit(address.name)
        self.phoneLineEdit = QLineEdit(address.phone)
        self.emailLineEdit = QLineEdit(address.email)
        self.cityLineEdit = QLineEdit(address.city)
        self.neighbourhoodLineEdit = QLineEdit(address.neighbourhood)
        self.apartmentBuildingLineEdit = QLineEdit(address.apartment_building)
        self.streetLineEdit = QLineEdit(address.street)
        self.floorLineEdit = QLineEdit(str(address.floor))
        self.apartmentLineEdit = QLineEdit(str(address.apartment))
        save_button = QPushButton("Save")
        delete_button = QPushButton("Delete")

        # setting attributes and connecting the widgets
        self.setLayout(layout)
        save_button.clicked.connect(self.on_save_button_clicked)
        delete_button.clicked.connect(self.on_delete_button_clicked)

        # adding the widgets
        layout.addWidget(QLabel("Name:"), 0, 0)
        layout.addWidget(self.nameLineEdit, 0, 1)
        layout.addWidget(QLabel("Phone:"), 1, 0)
        layout.addWidget(self.phoneLineEdit, 1, 1)
        layout.addWidget(QLabel("Email:"), 2, 0)
        layout.addWidget(self.emailLineEdit, 2, 1)
        layout.addWidget(QLabel("City:"), 3, 0)
        layout.addWidget(self.cityLineEdit, 3, 1)
        layout.addWidget(QLabel("Neighbourhood:"), 4, 0)
        layout.addWidget(self.neighbourhoodLineEdit, 4, 1)
        layout.addWidget(QLabel("Apartment Building:"), 5, 0)
        layout.addWidget(self.apartmentBuildingLineEdit, 5, 1)
        layout.addWidget(QLabel("Street:"), 6, 0)
        layout.addWidget(self.streetLineEdit, 6, 1)
        layout.addWidget(QLabel("Floor:"), 7, 0)
        layout.addWidget(self.floorLineEdit, 7, 1)
        layout.addWidget(QLabel("Apartment:"), 8, 0)
        layout.addWidget(self.apartmentLineEdit, 8, 1)
        layout.addWidget(save_button, 9, 0)
        layout.addWidget(delete_button, 10, 0)

        self.show()

    # Method that will be activated when the save button is clicked. It will change the addresses and have try block
    # if the input is incorrect.
    def on_save_button_clicked(self) -> None:
        try:
            self.address.floor = int(self.floorLineEdit.text())
        except ValueError:
            QMessageBox.warning(self, "Error", "The value in the floor should be a number")
            return

        try:
            self.address.apartment = str(self.apartmentLineEdit.text())
        except ValueError:
            QMessageBox.warning(self, "Error", "The value in the apartment should be a number")
            return

        self.address.name = self.nameLineEdit.text()
        self.address.phone = self.phoneLineEdit.text()
        self.address.email = self.emailLineEdit.text()
        self.address.neighbourhood = self.neighbourhoodLineEdit.text()
        self.address.apartment_building = self.apartmentBuildingLineEdit.text()
        self.address.street = self.streetLineEdit.text()

        self.wnd.on_search()
        self.close()

    # Method that will be activated when a deleted button is clicked. It will delete the address
    def on_delete_button_clicked(self) -> None:
        self.data.delete_address(self.address.id)
        self.wnd.on_search()
        self.close()


# Class that represent the main window that will be shown when the user is logged. It will allow the user to search
# for the addresses and add new addresses
class MainWindow(QWidget):
    addressInfoWindow: AddressInfoWindow
    addAddressWindow: AddAddressWindow

    def __init__(self, user_id: int, wnd: "LoginWindow") -> None:
        super().__init__()

        # Setting attributes to the window.
        self.setWindowTitle("Address Book")
        self.setGeometry(300, 300, 500, 500)

        # passing the arguments
        self.wnd = wnd

        # Creating the data object.
        self.data = Data(user_id)

        # Creating the widgets.
        layout = QGridLayout()
        self.resultsLayout = QVBoxLayout()
        self.searchBar = QLineEdit()
        results_panel = QFrame()
        results_scroll_area = QScrollArea()
        add_button = QPushButton("Add Address")

        # Setting attributes and connecting the widgets.
        self.setLayout(layout)
        results_panel.setLayout(self.resultsLayout)

        self.searchBar.returnPressed.connect(self.on_search)

        results_scroll_area.setWidgetResizable(True)
        results_scroll_area.setWidget(results_panel)

        add_button.clicked.connect(self.on_add_button_clicked)

        # Adding the widgets.
        layout.addWidget(self.searchBar, 0, 0)
        layout.addWidget(results_scroll_area, 1, 0)
        layout.addWidget(add_button, 2, 0)

        self.show()

    # Method activated when return is pressed in the search bar. Is search for addresses with matching names and
    # shows them.
    def on_search(self) -> None:
        for i in reversed(range(self.resultsLayout.count())):
            item = self.resultsLayout.itemAt(i)
            if item.widget():
                item.widget().deleteLater()

        text = self.searchBar.text().strip().lower()

        for address in self.data:
            if text in address.name:
                result = QPushButton(address.name)

                result.clicked.connect(partial(self.on_result_clicked, address))

                self.resultsLayout.addWidget(result)

    # Method that will be activated when a result is clicked. It will show a new window with information and tools
    # about the address
    def on_result_clicked(self, address: Address) -> None:
        self.addressInfoWindow = AddressInfoWindow(self, self.data, address)

    # Method that is called when the add address button is clicked. It will display a new window that will contain
    # tools to add the new address.
    def on_add_button_clicked(self) -> None:
        self.addAddressWindow = AddAddressWindow(self, self.data)

    def closeEvent(self, event: QCloseEvent) -> None:
        self.data.save_data()
        self.wnd.show()
        self.wnd.emailLineEdit.setText("")
        self.wnd.passwordLineEdit.setText("")
        event.accept()


# Class that represent the login window.
class LoginWindow(QWidget):
    mainWindow: MainWindow

    # constructor that will create the window
    def __init__(self) -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle("Address Book")
        self.setGeometry(300, 300, 400, 400)

        # creating the widgets
        layout = QGridLayout()
        self.emailLineEdit = QLineEdit()
        self.passwordLineEdit = QLineEdit()
        login_button = QPushButton("Login")
        register_button = QPushButton("Register")

        # setting attributes and connecting the widgets
        self.passwordLineEdit.setEchoMode(QLineEdit.EchoMode.Password)
        login_button.clicked.connect(self.on_login_button_clicked)
        register_button.clicked.connect(self.on_register_button_clicked)

        # adding the widgets
        layout.addWidget(QLabel("Email:"), 0, 0)
        layout.addWidget(self.emailLineEdit, 0, 1)
        layout.addWidget(QLabel("Password:"), 1, 0)
        layout.addWidget(self.passwordLineEdit, 1, 1)
        layout.addWidget(login_button, 2, 0)
        layout.addWidget(register_button, 2, 1)

        self.setLayout(layout)

    # Method activated when the login button is clicked. It will try to log the person, if it fails it will say the
    # user email or password is incorrect else it will open a new window with all the information
    def on_login_button_clicked(self) -> None:
        user_id = Data.login(self.emailLineEdit.text(), self.passwordLineEdit.text())
        if user_id is not None:
            self.mainWindow = MainWindow(user_id, self)
            self.hide()
        else:
            QMessageBox.warning(self, "Error", "Failed to register. The email or password is incorrect.")

    # Method that will register the user in the server. If a user exists with the same email, it will tell the user
    # that the email is linked with another account
    def on_register_button_clicked(self) -> None:
        if len(self.emailLineEdit.text()) == 0 or len(self.passwordLineEdit.text()) == 0:
            QMessageBox.warning(self, "Error", "The email and password should not be empty.")
            return

        if Data.add_new_user(self.emailLineEdit.text(), self.passwordLineEdit.text()):
            QMessageBox.information(self, "Registration Successful!", "You are now registered")
        else:
            QMessageBox.information(self, "Registration Failed!",
                                    "You are not registered. The email is linked with another account")


# Main function that will start the app.
def main() -> None:
    app = QApplication(sys.argv)
    window = LoginWindow()
    window.show()
    app.exec()


# Calling the main function.
if __name__ == '__main__':
    main()
