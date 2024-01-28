#include <iostream>
#include <vector>
#include <mysqlx/xdevapi.h>

#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QMessageBox>
#include <QFrame>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

using std::string;
using std::cerr;
using std::endl;

// Class used to represent and address information about a person.
class Address {
private:
    int id;
    string name;
    string phone;
    string email;
    string city;
    string neighbourhood;
    string apartmentBuilding;
    string street;
    int floor;
    int apartment;
public:
    Address(int id, string name, string phone, string email, string city,
            string neighbourhood, string apartmentBuilding, string street, int floor,
            int apartment) : id(id), name(std::move(name)), phone(std::move(phone)), email(std::move(email)),
                             city(std::move(city)), neighbourhood(std::move(neighbourhood)),
                             apartmentBuilding(std::move(apartmentBuilding)), street(std::move(street)), floor(floor),
                             apartment(apartment) {}

    // Getters and setters for the attributes.
    [[nodiscard]] int getId() const {
        return id;
    }


    [[nodiscard]] const string &getName() const {
        return name;
    }

    void setName(const string &_name) {
        Address::name = _name;
    }

    [[nodiscard]] const string &getPhone() const {
        return phone;
    }

    void setPhone(const string &_phone) {
        Address::phone = _phone;
    }

    [[nodiscard]] const string &getEmail() const {
        return email;
    }

    void setEmail(const string &_email) {
        Address::email = _email;
    }

    [[nodiscard]] const string &getCity() const {
        return city;
    }

    void setCity(const string &_city) {
        Address::city = _city;
    }

    [[nodiscard]] const string &getNeighbourhood() const {
        return neighbourhood;
    }

    void setNeighbourhood(const string &_neighbourhood) {
        Address::neighbourhood = _neighbourhood;
    }

    [[nodiscard]] const string &getApartmentBuilding() const {
        return apartmentBuilding;
    }

    void setApartmentBuilding(const string &_apartmentBuilding) {
        Address::apartmentBuilding = _apartmentBuilding;
    }

    [[nodiscard]] const string &getStreet() const {
        return street;
    }

    void setStreet(const string &_street) {
        Address::street = _street;
    }

    [[nodiscard]] int getFloor() const {
        return floor;
    }

    void setFloor(int _floor) {
        Address::floor = _floor;
    }

    [[nodiscard]] int getApartment() const {
        return apartment;
    }

    void setApartment(int _apartment) {
        Address::apartment = _apartment;
    }
};

// Class that will manage the data form the sql server.
class Data {
private:
    int userId;
    std::vector<Address> addresses;

public:
    // Method that is used to log in the app. It will return the user id if it found or -1 if it's not found;
    static int Login(const string &email, const string &password) {
        int id = -1;

        try {
            mysqlx::Session session("mysqlx://root:Simone2006@localhost:33060");
            mysqlx::SqlStatement sqlStatement = session.sql("USE AddressBook;");
            sqlStatement.execute();
            sqlStatement = session.sql("SELECT id FROM Users WHERE email= ? AND password= ?").bind(email, password);
            mysqlx::SqlResult sqlResult = sqlStatement.execute();

            if (sqlResult.count() != 0) {
                mysqlx::Row row = sqlResult.fetchOne();
                id = row[0].get<int>();
            }

            session.close();
        }
        catch (const mysqlx::Error &error) {
            cerr << "There was an error in the login function in data class: " << error << endl;
            exit(EXIT_FAILURE);
        }
        return id;
    }

    // Method that is used to register a new user. It returns true if the user is registered and false if the email is already in the system.
    static bool Register(const string &email, const string &password) {
        try {
            mysqlx::Session session("mysqlx://root:Simone2006@localhost:33060");
            mysqlx::SqlStatement sqlStatement = session.sql("USE AddressBook");
            sqlStatement.execute();
            sqlStatement = session.sql("SELECT COUNT(id) FROM Users WHERE email = ?;").bind(email);
            mysqlx::SqlResult sqlResult = sqlStatement.execute();

            mysqlx::Row row = sqlResult.fetchOne();
            if (!row[0].get<int>()) {
                sqlStatement = session.sql("INSERT INTO Users(email, password) VALUE (?, ?);").bind(email,
                                                                                                    password);
                sqlStatement.execute();
                return true;

            }

            session.close();
        }
        catch (const mysqlx::Error &error) {
            cerr << "There was an error in the register function in data class: " << error << endl;
            exit(EXIT_FAILURE);
        }

        return false;
    }

public:
    explicit Data(int userId) : userId(userId) {
        retrieveData();
    }

private:
    // Method that retrieve all the address that are linked to the user.
    void retrieveData() {
        try {
            mysqlx::Session session("mysqlx://root:Simone2006@localhost:33060");
            mysqlx::SqlStatement sqlStatement = session.sql("USE AddressBook");
            sqlStatement.execute();

            sqlStatement = session.sql(
                    "SELECT id, name, phone, email, city, neighbourhood, apartment_building, street, floor, apartment FROM Addresses WHERE user_id = ?;").bind(
                    userId);
            mysqlx::SqlResult sqlResult = sqlStatement.execute();

            for (mysqlx::Row row: sqlResult) {
                int id = row[0].get<int>();
                string name = row[1].get<string>();
                string phone = row[2].get<string>();
                string email = row[3].get<string>();
                string city = row[4].get<string>();
                string neighbourhood = row[5].get<string>();
                string apartmentBuilding = row[6].get<string>();
                string street = row[7].get<string>();
                int floor = row[8].get<int>();
                int apartment = row[9].get<int>();

                addresses.emplace_back(id, name, phone, email, city, neighbourhood, apartmentBuilding, street,
                                       floor, apartment);


            }

            session.close();
        }
        catch (const mysqlx::Error &error) {
            cerr << "There was an error in retrieve data method in class data " << error << endl;
            exit(EXIT_FAILURE);
        }
    }

public:
    // Method that will save the data back to the sql server.
    void saveData() {
        try {
            mysqlx::Session session("mysqlx://root:Simone2006@localhost:33060");
            mysqlx::SqlStatement sqlStatement = session.sql("USE AddressBook");
            sqlStatement.execute();

            for (const Address &address: addresses) {
                sqlStatement = session.sql(
                        "UPDATE Addresses SET name = ?,phone = ?, email = ?, city = ?,neighbourhood = ?,apartment_building = ?,street = ?,floor = ?, apartment = ? WHERE id = ? ;").bind(
                        address.getName(), address.getPhone(), address.getEmail(), address.getCity(),
                        address.getNeighbourhood(), address.getApartmentBuilding(), address.getStreet(),
                        address.getFloor(), address.getApartment(), address.getId());
                sqlStatement.execute();
            }

            session.close();
        }
        catch (const mysqlx::Error &error) {
            cerr << "There was an error in save data method in class data " << error << endl;
            exit(EXIT_FAILURE);
        }
    }

    // Method that will add a new address to the server, and it will be linked with the user.
    void addAddress(const string &name, const string &phone, const string &email, const string &city,
                    const string &neighbourhood, const string &apartmentBuilding, const string &street, int floor,
                    int apartment) {
        try {
            mysqlx::Session session("mysqlx://root:Simone2006@localhost:33060");
            mysqlx::SqlStatement sqlStatement = session.sql("USE AddressBook");
            sqlStatement.execute();
            sqlStatement = session.sql(
                    "INSERT INTO Addresses(name, phone, email, city, neighbourhood, apartment_building, street, floor, apartment, user_id) VALUES (?,?,?,?,?,?,?,?,?,?);").bind(
                    name, phone, email, city, neighbourhood, apartmentBuilding, street, floor, apartment, userId);
            sqlStatement.execute();

            sqlStatement = session.sql("SELECT LAST_INSERT_ID();");
            mysqlx::SqlResult sqlResult = sqlStatement.execute();

            mysqlx::Row row = sqlResult.fetchOne();
            addresses.emplace_back(row[0].get<int>(), name, phone, email, city, neighbourhood, apartmentBuilding,
                                   street, floor, apartment);

            session.close();

        }
        catch (const mysqlx::Error &error) {
            cerr << "There was an error in add address data method in class data " << error << endl;
            exit(EXIT_FAILURE);
        }
    }

    // Method that will delete an address based on the id.
    void deleteAddress(int id) {
        try {
            mysqlx::Session session("mysqlx://root:Simone2006@localhost:33060");

            for (int i = 0; i < addresses.size(); i++) {
                if (addresses[i].getId() == id) {
                    addresses.erase(addresses.begin() + i);
                }
            }
            mysqlx::SqlStatement sqlStatement = session.sql("USE AddressBook");
            sqlStatement.execute();
            sqlStatement = session.sql("DELETE FROM Addresses WHERE id = ?").bind(id);
            sqlStatement.execute();

            session.close();
        }
        catch (const mysqlx::Error &error) {
            cerr << "There was an error in delete address data method in class data " << error << endl;
            exit(EXIT_FAILURE);
        }
    }

    // Methods used for iterating over the addresses.
    [[nodiscard]] std::vector<Address>::const_iterator begin() const {
        return addresses.begin();
    }

    [[nodiscard]] std::vector<Address>::const_iterator end() const {
        return addresses.end();
    }

    std::vector<Address>::iterator begin() {
        return addresses.begin();
    }

    std::vector<Address>::iterator end() {
        return addresses.end();
    }
};

// Class used for creating the window for adding an address.
class AddAddressWindow : public QWidget {
Q_OBJECT

private:
    QLineEdit *nameLineEdit = new QLineEdit();
    QLineEdit *phoneLineEdit = new QLineEdit();
    QLineEdit *emailLineEdit = new QLineEdit();
    QLineEdit *cityLineEdit = new QLineEdit();
    QLineEdit *neighbourhoodLineEdit = new QLineEdit();
    QLineEdit *apartmentBuildingLineEdit = new QLineEdit();
    QLineEdit *streetLineEdit = new QLineEdit();
    QLineEdit *floorLineEdit = new QLineEdit();
    QLineEdit *apartmentLineEdit = new QLineEdit();
public:
    explicit AddAddressWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // Setting attributes.
        this->setWindowTitle("Add address");
        this->setGeometry(300, 300, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // Creating the widgets.
        auto layout = new QGridLayout();
        auto addButton = new QPushButton("Add");

        // Setting attributes and adding the widgets.
        this->setLayout(layout);

        connect(addButton, &QPushButton::clicked, [=]() { onAddButtonClicked(); });

        // Adding the widgets.
        layout->addWidget(new QLabel("Name"), 0, 0);
        layout->addWidget(nameLineEdit, 0, 1);
        layout->addWidget(new QLabel("Phone:"), 1, 0);
        layout->addWidget(phoneLineEdit, 1, 1);
        layout->addWidget(new QLabel("Email:"), 2, 0);
        layout->addWidget(emailLineEdit, 2, 1);
        layout->addWidget(new QLabel("City:"), 3, 0);
        layout->addWidget(cityLineEdit, 3, 1);
        layout->addWidget(new QLabel("Neighbourhood:"), 4, 0);
        layout->addWidget(neighbourhoodLineEdit, 4, 1);
        layout->addWidget(new QLabel("Apartment building:"), 5, 0);
        layout->addWidget(apartmentBuildingLineEdit, 5, 1);
        layout->addWidget(new QLabel("Street:"), 6, 0);
        layout->addWidget(streetLineEdit, 6, 1);
        layout->addWidget(new QLabel("Floor:"), 7, 0);
        layout->addWidget(floorLineEdit, 7, 1);
        layout->addWidget(new QLabel("Apartment :"), 8, 0);
        layout->addWidget(apartmentLineEdit, 8, 1);
        layout->addWidget(addButton, 9, 0);

        this->show();


    }

signals:

    // Signal that is emitted when the user clicks add.
    void addedAddress(const string &name, const string &phone, const string &email, const string &city,
                      const string &neighbourhood, const string &apartmentBuilding, const string &street, int floor,
                      int apartment);

private slots:

    // Method that is called when the user clicks add.
    // It will emmit the signal and close the window.
    void onAddButtonClicked() {
        bool parsed;
        int floor = floorLineEdit->text().toInt(&parsed);

        if (!parsed) {
            QMessageBox::warning(this, "Error", "The value of the floor should be a number");
            return;
        }

        int apartment = apartmentLineEdit->text().toInt(&parsed);

        if (!parsed) {
            QMessageBox::warning(this, "Error", "The value of the apartment should be a number");
            return;
        }

        string name = nameLineEdit->text().toStdString();
        string phone = phoneLineEdit->text().toStdString();
        string email = phoneLineEdit->text().toStdString();
        string city = cityLineEdit->text().toStdString();
        string neighbourhood = neighbourhoodLineEdit->text().toStdString();
        string apartmentBuilding = apartmentBuildingLineEdit->text().toStdString();
        string street = streetLineEdit->text().toStdString();

        emit addedAddress(name, phone, email, city, neighbourhood, apartmentBuilding, street, floor, apartment);
        this->close();
    }
};

// Class used to create the window with information about an address.
class AddressInfoWindow : public QWidget {
Q_OBJECT

private:

    int addressId;
    QLineEdit *nameLineEdit = new QLineEdit();
    QLineEdit *phoneLineEdit = new QLineEdit();
    QLineEdit *emailLineEdit = new QLineEdit();
    QLineEdit *cityLineEdit = new QLineEdit();
    QLineEdit *neighbourhoodLineEdit = new QLineEdit();
    QLineEdit *apartmentBuildingLineEdit = new QLineEdit();
    QLineEdit *streetLineEdit = new QLineEdit();
    QLineEdit *floorLineEdit = new QLineEdit();
    QLineEdit *apartmentLineEdit = new QLineEdit();

public:
    explicit AddressInfoWindow(const Address &address, QWidget *parent = nullptr) : QWidget(parent) {
        // Setting attributes.
        this->setWindowTitle("Address");
        this->setGeometry(300, 300, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // Passing the arguments.
        addressId = address.getId();

        // Creating the widgets.
        auto layout = new QGridLayout();
        auto saveButton = new QPushButton("Save");
        auto deleteButton = new QPushButton("Delete");

        // Setting attributes and connecting the widgets.
        this->setLayout(layout);

        nameLineEdit->setText(QString::fromStdString(address.getName()));
        phoneLineEdit->setText(QString::fromStdString(address.getPhone()));
        emailLineEdit->setText(QString::fromStdString(address.getEmail()));
        cityLineEdit->setText(QString::fromStdString(address.getCity()));
        neighbourhoodLineEdit->setText(QString::fromStdString(address.getNeighbourhood()));
        apartmentBuildingLineEdit->setText(QString::fromStdString(address.getApartmentBuilding()));
        streetLineEdit->setText(QString::fromStdString(address.getStreet()));
        floorLineEdit->setText(QString::number(address.getFloor()));
        apartmentLineEdit->setText(QString::number(address.getApartment()));

        connect(saveButton, &QPushButton::clicked, [=]() { onSaveButtonClicked(); });
        connect(deleteButton, &QPushButton::clicked, [=]() { onDeleteButtonClicked(); });

        // Adding the widgets.
        layout->addWidget(new QLabel("Name"), 0, 0);
        layout->addWidget(nameLineEdit, 0, 1);
        layout->addWidget(new QLabel("Phone:"), 1, 0);
        layout->addWidget(phoneLineEdit, 1, 1);
        layout->addWidget(new QLabel("Email:"), 2, 0);
        layout->addWidget(emailLineEdit, 2, 1);
        layout->addWidget(new QLabel("City:"), 3, 0);
        layout->addWidget(cityLineEdit, 3, 1);
        layout->addWidget(new QLabel("Neighbourhood:"), 4, 0);
        layout->addWidget(neighbourhoodLineEdit, 4, 1);
        layout->addWidget(new QLabel("Apartment building:"), 5, 0);
        layout->addWidget(apartmentBuildingLineEdit, 5, 1);
        layout->addWidget(new QLabel("Street:"), 6, 0);
        layout->addWidget(streetLineEdit, 6, 1);
        layout->addWidget(new QLabel("Floor:"), 7, 0);
        layout->addWidget(floorLineEdit, 7, 1);
        layout->addWidget(new QLabel("Apartment :"), 8, 0);
        layout->addWidget(apartmentLineEdit, 8, 1);
        layout->addWidget(saveButton, 9, 0);
        layout->addWidget(deleteButton, 10, 0);

        this->show();
    }

signals:

    // Signal that will be emitted when the user clicks save.
    void savedAddress(int id, const string &name, const string &phone, const string &email, const string &city,
                      const string &neighbourhood, const string &apartmentBuilding, const string &street, int floor,
                      int apartment);

    // Signal that will be emmit when the user clicks delete.
    void deletedAddress(int id);

private slots:

    // Method that is called when the user clicks save.
    // It will emmit the signal and close the window.
    void onSaveButtonClicked() {
        bool parsed;
        int floor = floorLineEdit->text().toInt(&parsed);

        if (!parsed) {
            QMessageBox::warning(this, "Error", "The value of the floor should be a number");
            return;
        }

        int apartment = apartmentLineEdit->text().toInt(&parsed);

        if (!parsed) {
            QMessageBox::warning(this, "Error", "The value of the apartment should be a number");
            return;
        }

        string name = nameLineEdit->text().toStdString();
        string phone = phoneLineEdit->text().toStdString();
        string email = phoneLineEdit->text().toStdString();
        string city = cityLineEdit->text().toStdString();
        string neighbourhood = neighbourhoodLineEdit->text().toStdString();
        string apartmentBuilding = apartmentBuildingLineEdit->text().toStdString();
        string street = streetLineEdit->text().toStdString();

        emit savedAddress(addressId, name, phone, email, city, neighbourhood, apartmentBuilding, street, floor,
                          apartment);
        this->close();
    }

    // Method that is called when the user clicks delete.
    // It will emit the signal and close the window.
    void onDeleteButtonClicked() {
        emit deletedAddress(addressId);
        this->close();
    }
};

// Class used to create the main window.
class MainWindow : public QWidget {
Q_OBJECT

private:
    QLineEdit *searchBar = new QLineEdit();
    QVBoxLayout *resultLayout = new QVBoxLayout();
    Data data;
public:
    explicit MainWindow(int userId, QWidget *parent = nullptr) : data(Data(userId)), QWidget(parent) {
        // Setting attributes to the window.
        this->setWindowTitle("Address book");
        this->setGeometry(300, 300, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // Creating the widgets.
        auto layout = new QGridLayout();
        auto resultFrame = new QFrame();
        auto resultScrollArea = new QScrollArea();
        auto addAddressButton = new QPushButton("Add address");

        // Setting attributes and connecting the widgets.
        this->setLayout(layout);
        resultFrame->setLayout(resultLayout);

        resultScrollArea->setWidget(resultFrame);
        resultScrollArea->setWidgetResizable(true);

        connect(searchBar, &QLineEdit::returnPressed, [=]() { onSearch(); });
        connect(addAddressButton, &QPushButton::clicked, [=]() { onAddAddressButtonClicked(); });

        // Adding the widgets.
        layout->addWidget(searchBar, 0, 0);
        layout->addWidget(resultScrollArea, 1, 0);
        layout->addWidget(addAddressButton, 2, 0);

        this->show();
    }

signals:

    // Signal that will be emmit when the window is closed.
    void mainWindowClosed();

private slots:

    // Method that is called when the user clicks enter the search bar.
    // It will search for the results and display them.
    void onSearch() {
        QLayoutItem *item;
        while ((item = resultLayout->takeAt(0)) != nullptr) {
            QWidget *widget = item->widget();
            if (widget != nullptr) {
                resultLayout->removeWidget(widget);
                delete widget;
            }
            delete item;
        }

        string text = searchBar->text().toLower().trimmed().toStdString();


        for (const Address &address: data) {
            if (address.getName().find(text) != string::npos) {
                auto result = new QPushButton(QString::fromStdString(address.getName()));

                connect(result, &QPushButton::clicked, [=]() { onResultClicked(address); });

                resultLayout->addWidget(result);
            }
        }
    }

    // Method that is called when the user clicks any of the results.
    // It will display the window with information and link its signals.
    void onResultClicked(const Address &address) {
        auto addressWindowInfo = new AddressInfoWindow(address);
        connect(addressWindowInfo, &AddressInfoWindow::savedAddress, this, &MainWindow::savedAddress);
        connect(addressWindowInfo, &AddressInfoWindow::deletedAddress, this, &MainWindow::deletedAddress);
    }

    // Method that is called when the user clicks save in AddressInfoWindow.
    // It will save the changes to the server and update the UI.
    void savedAddress(int id, const string &name, const string &phone, const string &email, const string &city,
                      const string &neighbourhood, const string &apartmentBuilding, const string &street, int floor,
                      int apartment) {
        for (Address &address: data) {
            if (address.getId() == id) {
                address.setName(name);
                address.setPhone(phone);
                address.setEmail(email);
                address.setCity(city);
                address.setNeighbourhood(neighbourhood);
                address.setApartmentBuilding(apartmentBuilding);
                address.setStreet(street);
                address.setFloor(floor);
                address.setApartment(apartment);
                onSearch();
                break;
            }
        }
    }

    // Method that is called when the user clicks delete in AddressInfoWindow.
    // It will delete the address from the server and update the Ui.
    void deletedAddress(int id) {
        data.deleteAddress(id);
        onSearch();
    }

    // Method that is called when the user clicks add address.
    // It will show the window and connect its signal.
    void onAddAddressButtonClicked() {
        auto addAddressWindow = new AddAddressWindow();
        connect(addAddressWindow, &AddAddressWindow::addedAddress, this, &MainWindow::addedAddress);

    }

    // Method called, when the user clicks add in AddAddressWindow, is clicked.
    // It will add the address to the server and refresh the UI.
    void addedAddress(const string &name, const string &phone, const string &email, const string &city,
                      const string &neighbourhood, const string &apartmentBuilding, const string &street, int floor,
                      int apartment) {
        data.addAddress(name, phone, email, city, neighbourhood, apartmentBuilding, street, floor, apartment);
        onSearch();
    }

protected:
    // Method called when the window is closed. It will save the changed made in the addresses, close
    // and emit the signal to open the login window.
    void closeEvent(QCloseEvent *event) override {
        emit mainWindowClosed();
        data.saveData();
        this->close();
    }
};

// Class that used to create the login window.
class LoginWindow : public QWidget {
Q_OBJECT

private:
    QLineEdit *emailLineEdit = new QLineEdit;
    QLineEdit *passwordEdit = new QLineEdit;
public:
    explicit LoginWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // Setting attributes to the window.
        this->setWindowTitle("Address app");
        this->setGeometry(300, 300, 400, 400);

        // Creating the widgets
        auto layout = new QGridLayout();
        auto loginButton = new QPushButton("Login");
        auto registerButton = new QPushButton("Register");

        // Setting attributes and connecting the widgets.
        this->setLayout(layout);
        passwordEdit->setEchoMode(QLineEdit::Password);
        connect(loginButton, &QPushButton::clicked, [=]() { onLoginButtonClicked(); });
        connect(registerButton, &QPushButton::clicked, [=]() { onRegisterButtonClicked(); });

        // Adding the widgets.
        layout->addWidget(new QLabel("Email:"), 0, 0);
        layout->addWidget(emailLineEdit, 0, 1);
        layout->addWidget(new QLabel("Password"), 1, 0);
        layout->addWidget(passwordEdit, 1, 1);
        layout->addWidget(loginButton, 2, 0);
        layout->addWidget(registerButton, 2, 1);
    }

private slots:

    // Method that will be called when the user clicks login. If the email and password are found, it will open a new window with the addresses,
    // otherwise it will tell the user that the email or the password is invalid.
    void onLoginButtonClicked() {
        string email = emailLineEdit->text().toStdString();
        string password = passwordEdit->text().toStdString();
        int userId = Data::Login(email, password);

        if (userId != -1) {
            auto mainWindow = new MainWindow(userId);
            connect(mainWindow, &MainWindow::mainWindowClosed, this, &LoginWindow::onMainWindowClosed);
            this->hide();
        } else {
            QMessageBox::warning(this, "Error", "The email or the password is wrong");
        }
    };

    // Method that is called when the user clicks the register button. If the email is linked to with an account,
    // it will tell the user that he won't be registered, otherwise it will register the user.
    void onRegisterButtonClicked() {
        string email = emailLineEdit->text().toStdString();
        string password = passwordEdit->text().toStdString();

        if (email.empty()) {
            QMessageBox::warning(this, "Error", "The email can't be empty");
            return;
        }

        if (password.empty()) {
            QMessageBox::warning(this, "Error", "The password can't be empty");
            return;
        }

        if (Data::Register(email, password)) {
            QMessageBox::information(this, "Registrations successful", "You are now registered");
        } else {
            QMessageBox::warning(this, "Registration unsuccessful", "The email is already in use");
        }
    }

    // Method that is called when the main window is closed. It will display the login window again
    // and will clean the line edits for the email and the password.
    void onMainWindowClosed() {
        this->show();
        emailLineEdit->setText("");
        passwordEdit->setText("");
    }
};

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    LoginWindow loginWindow;
    loginWindow.show();
    return QApplication::exec();
}

#include "main.moc"
