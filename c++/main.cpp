#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <vector>
#include <QString>

#include <QFile>
#include <QBitArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueRef>

// class that used to represent information about a person
class Person {
private:
    QString name;
    QString address;
    QString phone;
    QString email;
    int id;
public:
    Person(const QString &name, const QString &address, const QString &phone, const QString &email, int id) {
        this->name = name;
        this->address = address;
        this->phone = phone;
        this->email = email;
        this->id = id;
    }

    [[nodiscard]] QString getName() const {
        return name;
    }

    void setName(const QString &_name) {
        name = _name;
    }

    [[nodiscard]] QString getAddress() const {
        return address;
    }

    void setAddress(const QString &_address) {
        address = _address;
    }

    [[nodiscard]] QString getPhone() const {
        return phone;
    }

    void setPhone(const QString &_phone) {
        phone = _phone;
    }

    [[nodiscard]] QString getEmail() const {
        return email;
    }

    void setEmail(const QString &_email) {
        email = _email;
    }

    [[nodiscard]] int getId() const {
        return id;
    }

    void setId(int _id) {
        id = _id;
    }
};

// class that is used to handel the information
class Data {
private:
    // holder of all persons
    std::vector<Person> persons;
public:
    Data() {
        readData();
    }

    // method that will write back the data to the json file
    void writeData() {
        revalidateIds();

        QJsonArray jsonArray;

        for (const Person &person: persons) {
            QJsonObject object;
            object["name"] = person.getName();
            object["address"] = person.getAddress();
            object["phone"] = person.getPhone();
            object["email"] = person.getEmail();
            object["id"] = person.getId();
            jsonArray.append(object);
        }

        QJsonDocument jsonDocument(jsonArray);
        QByteArray byteArray = jsonDocument.toJson();

        QFile file("/Users/simonesamardzhiev/Desktop/My projects/Address Book/c++/data.json");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "The file couldn't be open for writing";
            return;
        }
        file.write(byteArray);
        file.close();
    }

    //  method that will delete person using the id
    void deletePerson(int _id) {
        for (int i = 0; i < persons.size(); i++) {
            if (persons[i].getId() == _id) {
                persons.erase(persons.begin() + i);
                break;
            }
        }
    }

    // method that will add a new person
    void addNewPerson(const QString &name, const QString &address, const QString &phone, const QString &email) {
        persons.emplace_back(name, address, phone, email, getNewId());
    }

    // method that will change the information of a person using the id
    void changePerson(const QString &name, const QString &address, const QString &phone, const QString &email, int id) {
        for (Person &person: persons) {
            if (person.getId() == id) {
                person.setName(name);
                person.setAddress(address);
                person.setPhone(phone);
                person.setEmail(email);
                break;
            }
        }
    }

    // using to iterate through the persons
    [[nodiscard]] std::vector<Person>::const_iterator begin() const {
        return persons.begin();
    }

    // using to iterate through the persons
    [[nodiscard]] std::vector<Person>::const_iterator end() const {
        return persons.end();
    }

private:
    // method that will read the data form the json file and store it into the vector
    void readData() {
        QFile file("/Users/simonesamardzhiev/Desktop/My projects/Address Book/c++/data.json");
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "The file couldn't be open for reading";
            return;
        }
        QByteArray byteArray = file.readAll();
        file.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray);
        QJsonArray jsonArray = jsonDocument.array();

        for (const QJsonValueRef &value: jsonArray) {
            QJsonObject object = value.toObject();
            persons.emplace_back(object.value("name").toString(), object.value("address").toString(),
                                 object.value("phone").toString(), object.value("email").toString(),
                                 object.value("id").toInt());
        }
    }

    // method that is called before writing the data to revalidate the ids if the persons
    void revalidateIds() {
        for (int i = 0; i < persons.size(); i++) {
            persons[i].setId(i);
        }
    }

    // method that will return a new unique id each time its called
    int getNewId() {
        if (persons.empty()) {
            return 0;
        } else {
            return persons.back().getId() + 1;
        }
    }
};

// class that used to show a window for adding a new person
class NewPersonWindow : public QWidget {
Q_OBJECT

private:
    // widgets that will be used to get the data for the user
    QLineEdit *nameLineEdit;
    QLineEdit *addressLineEdit;
    QLineEdit *phoneLineEdit;
    QLineEdit *emailLineEdit;
public:
    explicit NewPersonWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle("New person");
        this->setGeometry(300, 300, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto *layout = new QGridLayout;
        nameLineEdit = new QLineEdit;
        addressLineEdit = new QLineEdit;
        phoneLineEdit = new QLineEdit;
        emailLineEdit = new QLineEdit;
        auto *buttonAdd = new QPushButton("Add");

        // connecting the button
        connect(buttonAdd, &QPushButton::clicked, [=]() { onAddClicked(); });

        // adding the widgets
        layout->addWidget(new QLabel("Name :"), 0, 0);
        layout->addWidget(nameLineEdit, 0, 1);
        layout->addWidget(new QLabel("Address :"), 1, 0);
        layout->addWidget(addressLineEdit, 1, 1);
        layout->addWidget(new QLabel("Phone :"), 2, 0);
        layout->addWidget(phoneLineEdit, 2, 1);
        layout->addWidget(new QLabel("Email :"), 3, 0);
        layout->addWidget(emailLineEdit, 3, 1);
        layout->addWidget(buttonAdd, 4, 0);

        this->setLayout(layout);
        this->show();
    }

private slots:

    // method that collect the data by the widgets and emits(calls) a signal
    void onAddClicked() {
        emit addClicked(nameLineEdit->text(), addressLineEdit->text(), phoneLineEdit->text(), emailLineEdit->text());
        this->close();
    };
signals:

    // signal(function) that will be emitted(called) when the button is pressed
    void addClicked(const QString &name, const QString &address, const QString &phone, const QString &email);
};

// class that is used to show a person and change the information
class PersonWindow : public QWidget {
Q_OBJECT

private:
    int id;
    QLineEdit *nameLineEdit;
    QLineEdit *addressLineEdit;
    QLineEdit *phoneLineEdit;
    QLineEdit *emailLineEdit;
public:
    explicit PersonWindow(const Person &person, QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle(person.getName());
        this->setGeometry(400, 400, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        //  passing the argument
        id = person.getId();

        // creating the widgets
        auto *layout = new QGridLayout;
        nameLineEdit = new QLineEdit;
        addressLineEdit = new QLineEdit;
        phoneLineEdit = new QLineEdit;
        emailLineEdit = new QLineEdit;
        auto *buttonSave = new QPushButton("Save");
        auto *buttonDelete = new QPushButton("Delete");

        // setting attributes to the widgets
        nameLineEdit->setText(person.getName());
        addressLineEdit->setText(person.getAddress());
        phoneLineEdit->setText(person.getPhone());
        emailLineEdit->setText(person.getEmail());

        //connecting buttons
        connect(buttonSave, &QPushButton::clicked, [=]() { onSaveClicked(); });
        connect(buttonDelete, &QPushButton::clicked, [=]() { onDeleteClicked(); });

        // adding the widgets
        layout->addWidget(new QLabel("Name :"), 0, 0);
        layout->addWidget(nameLineEdit, 0, 1);
        layout->addWidget(new QLabel("Address :"), 1, 0);
        layout->addWidget(addressLineEdit, 1, 1);
        layout->addWidget(new QLabel("Phone :"), 2, 0);
        layout->addWidget(phoneLineEdit, 2, 1);
        layout->addWidget(new QLabel("Email :"), 3, 0);
        layout->addWidget(emailLineEdit, 3, 1);
        layout->addWidget(buttonSave, 4, 0);
        layout->addWidget(buttonDelete, 5, 0);

        this->setLayout(layout);
        this->show();
    }

private slots:

    void onSaveClicked() {
        emit saveClicked(nameLineEdit->text(), addressLineEdit->text(), phoneLineEdit->text(), emailLineEdit->text(),
                         id);
        this->close();
    }

    void onDeleteClicked() {
        emit deleteClicked(id);
        this->close();
    }

signals:

    void saveClicked(const QString &name, const QString &address, const QString &phone, const QString &email, int id);

    void deleteClicked(int id);
};

// class that is used to create the main window
class Window : public QWidget {
Q_OBJECT

private:
    // necessary global widgets and the data object
    Data data;
    QGridLayout *layout;
    QLineEdit *searchBar;
    std::vector<QPushButton *> results;
public:
    explicit Window(QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes the window
        this->setWindowTitle("Address book");
        this->setGeometry(250, 250, 500, 500);

        // creating the widgets
        layout = new QGridLayout;
        auto *buttonAdd = new QPushButton("Add new person");
        searchBar = new QLineEdit;

        // connect the widgets
        connect(buttonAdd, &QPushButton::clicked, [=]() { onAddClicked(); });
        connect(searchBar, &QLineEdit::returnPressed, [=]() { onSearch(); });

        // adding the widgets
        layout->addWidget(buttonAdd, 0, 0);
        layout->addWidget(searchBar, 1, 0);

        this->setLayout(layout);
    }


private slots:

    // method that will search for a person. It takes the text at the search bar and the tries to find a person that name start with
    void onSearch() {
        for (QPushButton *result: results) {
            layout->removeWidget(result);
            delete result;
        }
        results.clear();

        QString text = searchBar->text();
        int row = 2;

        for (const Person &person: data) {
            if (person.getName().startsWith(text)) {
                auto *result = new QPushButton(person.getName());
                results.push_back(result);

                connect(result, &QPushButton::clicked, [=]() { onResultClicked(person); });

                layout->addWidget(result, row, 0);
                row++;
            }
        }
    }

    // method that will show the window for adding new person
    void onAddClicked() const {
        auto *newPersonWindow = new NewPersonWindow();
        connect(newPersonWindow, &NewPersonWindow::addClicked, this, &Window::onAddButtonClicked);
    }

    // method that will be called when the signal addClicked is emitted and will add the new person
    void onAddButtonClicked(const QString &name, const QString &address, const QString &phone, const QString &email) {
        data.addNewPerson(name, address, phone, email);
        onSearch();
    }

    // method that will be called when a result is clicked. It will show a new window with information about the person
    void onResultClicked(const Person &person) const {
        auto *personWindow = new PersonWindow(person);
        connect(personWindow, &PersonWindow::deleteClicked, this, &Window::onDeleteClicked);
        connect(personWindow, &PersonWindow::saveClicked, this, &Window::onSaveClicked);
    }

    // method that is activated when the saveClicked signal is emitted. It will save all the changes made to the person
    void
    onSaveClicked(const QString &name, const QString &address, const QString &phone, const QString &email, int id) {
        data.changePerson(name, address, phone, email, id);
        onSearch();
    }

    // method that is called when the deleteClicked signal is emitted. It will delete the person
    void onDeleteClicked(int id) {
        data.deletePerson(id);
        onSearch();
    }

    // method that will write the data back to the json file when the main window is closed. That way the information will be saved;
    void closeEvent(QCloseEvent *event) override {
        data.writeData();
        QWidget::closeEvent(event);
    }
};

// main function
int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    Window window;
    window.show();
    return QApplication::exec();
}

#include "main.moc"