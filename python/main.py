import json
import sys
from functools import partial

from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import QApplication, QWidget, QLabel, QPushButton, QLineEdit, QGridLayout


# class that will hold the json data
class Data:
    # list that will hold the data
    data: list[dict]

    def __init__(self) -> None:
        self.read_data()

    # method that will read the data form the json file
    def read_data(self) -> None:
        with open('data.json', 'r') as file:
            self.data = json.load(file)

    # method that will write the data back to the json file
    def write_data(self) -> None:
        self.revalidate_ids()
        with open('data.json', 'w') as file:
            json.dump(self.data, file, indent=4)

    # method that will update the ids of the person. Its called when the data will be written
    def revalidate_ids(self) -> None:
        for i in range(len(self.data)):
            self.data[i]['id'] = i

    # method that will delete the person based on the id
    def delete_person(self, _id: int) -> None:
        self.data = [person for person in self.data if person['id'] != _id]

    # method that will add new person
    def add_person(self, name: str, address: str, phone: str, email: str) -> None:
        self.data.append({'name': name, 'address': address, 'phone': phone, 'email': email, 'id': self.get_new_id()})

    # method that will return a new unique id. It's called each time a new person is created
    def get_new_id(self) -> int:
        try:
            return self.data[-1]['id'] + 1
        except IndexError:
            return 0

    # method that will change the attributes of a window
    def change_person(self, name: str, address: str, phone: str, email: str, _id: int) -> None:
        for person in self.data:
            if person['id'] == _id:
                person['name'] = name
                person['address'] = address
                person['phone'] = phone
                person['email'] = email
                break

    # special method that is called when trying the iterate over the object. It returns all the persons
    def __iter__(self) -> dict:
        for person in self.data:
            yield person


# class that will create the window for adding a new person
class NewPersonWindow(QWidget):
    nameLineEdit: QLineEdit
    addressLineEdit: QLineEdit
    phoneLineEdit: QLineEdit
    emailLineEdit: QLineEdit
    data: Data
    wnd: "Window"

    def __init__(self, data: Data, wnd: "Window") -> None:
        super().__init__()
        # setting attribute to the window
        self.setWindowTitle("New Person")
        self.setGeometry(300, 300, 500, 500)

        # passing the arguments
        self.data = data
        self.wnd = wnd

        # creating the widgets
        layout = QGridLayout()
        self.nameLineEdit = QLineEdit()
        self.addressLineEdit = QLineEdit()
        self.phoneLineEdit = QLineEdit()
        self.emailLineEdit = QLineEdit()
        button_add = QPushButton("Add")

        # connecting the button
        button_add.clicked.connect(self.on_add_clicked)

        # adding the widgets
        layout.addWidget(QLabel("Name :"), 0, 0)
        layout.addWidget(self.nameLineEdit, 0, 1)
        layout.addWidget(QLabel("Address :"), 1, 0)
        layout.addWidget(self.addressLineEdit, 1, 1)
        layout.addWidget(QLabel("Phone :"), 2, 0)
        layout.addWidget(self.phoneLineEdit, 2, 1)
        layout.addWidget(QLabel("Email :"), 3, 0)
        layout.addWidget(self.emailLineEdit, 3, 1)
        layout.addWidget(button_add, 4, 0)

        self.setLayout(layout)
        self.show()

    # method that is called when the button is clicked. It will add the new person
    def on_add_clicked(self) -> None:
        self.data.add_person(self.nameLineEdit.text(), self.addressLineEdit.text(), self.phoneLineEdit.text(),
                             self.emailLineEdit.text())
        self.wnd.on_search()
        self.close()


# window that will show information about a person
class PersonWindow(QWidget):
    nameLineEdit: QLineEdit
    addressLineEdit: QLineEdit
    phoneLineEdit: QLineEdit
    emailLineEdit: QLineEdit
    _id: int
    data: Data
    wnd: "Window"

    def __init__(self, person: dict, data: Data, wnd: "Window") -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle(person['name'])
        self.setGeometry(250, 250, 400, 400)

        # passing the arguments
        self._id = person['id']
        self.data = data
        self.wnd = wnd

        # creating the widgets
        layout = QGridLayout()
        self.nameLineEdit = QLineEdit(person['name'])
        self.addressLineEdit = QLineEdit(person['address'])
        self.phoneLineEdit = QLineEdit(person['phone'])
        self.emailLineEdit = QLineEdit(person['email'])
        button_save = QPushButton("Save")
        button_delete = QPushButton("Delete")

        # connecting the buttons
        button_save.clicked.connect(self.button_save_clicked)
        button_delete.clicked.connect(self.button_delete_clicked)

        # adding the widgets
        layout.addWidget(QLabel("Name :"), 0, 0)
        layout.addWidget(self.nameLineEdit, 0, 1)
        layout.addWidget(QLabel("Address :"), 1, 0)
        layout.addWidget(self.addressLineEdit, 1, 1)
        layout.addWidget(QLabel("Phone :"), 2, 0)
        layout.addWidget(self.phoneLineEdit, 2, 1)
        layout.addWidget(QLabel("Email :"), 3, 0)
        layout.addWidget(self.emailLineEdit, 3, 1)
        layout.addWidget(button_save, 4, 0)
        layout.addWidget(button_delete, 5, 0)

        self.setLayout(layout)
        self.show()

    # method that is called when the save button is clicked. It will change the person's attributes ¬
    def button_save_clicked(self) -> None:
        self.data.change_person(self.nameLineEdit.text(), self.addressLineEdit.text(), self.phoneLineEdit.text(),
                                self.emailLineEdit.text(), self._id)
        self.wnd.on_search()
        self.close()

    # method that is called when the delete button is clicked. It will remove the person
    def button_delete_clicked(self) -> None:
        self.data.delete_person(self._id)
        self.wnd.on_search()
        self.close()


# the main window of the app. It shows the persons and the other functionalities
class Window(QWidget):
    data = Data()
    searchBar: QLineEdit
    layout: QGridLayout
    results: list[QPushButton] = []
    wnd: PersonWindow | NewPersonWindow

    def __init__(self) -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle("Address book")
        self.setGeometry(250, 250, 500, 500)

        # creating the widgets
        self.layout = QGridLayout()
        button_add = QPushButton("Add new person")
        self.searchBar = QLineEdit()

        # connecting the widgets
        button_add.clicked.connect(self.on_add_clicked)
        self.searchBar.returnPressed.connect(self.on_search)

        # adding the widgets
        self.layout.addWidget(button_add, 0, 0)
        self.layout.addWidget(self.searchBar, 1, 0)

        self.setLayout(self.layout)

    # method that is called when the add button is clicked
    # It will show the window that is responsible for adding the person
    def on_add_clicked(self) -> None:
        self.wnd = NewPersonWindow(self.data, self)

    # method that is called when the user click return/enter on the search bar
    # It will show the person that have been found
    def on_search(self, ) -> None:
        for result in self.results:
            self.layout.removeWidget(result)
        self.results.clear()

        text = self.searchBar.text()
        row = 2

        for person in self.data:
            if person['name'].startswith(text):
                result = QPushButton(person['name'])
                self.results.append(result)

                result.clicked.connect(partial(self.on_result_clicked, person))

                self.layout.addWidget(result, row, 0)
                row += 1

    # method that is called when the result is clicked
    # it will show the window with information about the person and other functionalities
    def on_result_clicked(self, person: dict) -> None:
        self.wnd = PersonWindow(person, self.data, self)

    # method that is called when the main window is closed
    # it will write the data back to the json file so the changes will be saved
    def closeEvent(self, event: QCloseEvent) -> None:
        self.data.write_data()
        event.accept()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec())
