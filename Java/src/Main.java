import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.sql.*;
import java.util.*;

// Class that is used to represent information about a person.
class Address {
    private final int id;
    private String name;
    private String phone;
    private String email;
    private String city;
    private String neighbourhood;
    private String apartmentBuilding;
    private String street;
    private int floor;
    private int apartment;

    public Address(int id, String name, String phone, String email, String city, String neighbourhood, String apartmentBuilding, String street, int floor, int apartment) {
        this.id = id;
        this.name = name;
        this.phone = phone;
        this.email = email;
        this.city = city;
        this.neighbourhood = neighbourhood;
        this.apartmentBuilding = apartmentBuilding;
        this.street = street;
        this.floor = floor;
        this.apartment = apartment;
    }

    // Getters and setters to the attributes of the class.
    public int getId() {
        return id;
    }


    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPhone() {
        return phone;
    }

    public void setPhone(String phone) {
        this.phone = phone;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public String getNeighbourhood() {
        return neighbourhood;
    }

    public void setNeighbourhood(String neighbourhood) {
        this.neighbourhood = neighbourhood;
    }

    public String getApartmentBuilding() {
        return apartmentBuilding;
    }

    public void setApartmentBuilding(String apartmentBuilding) {
        this.apartmentBuilding = apartmentBuilding;
    }

    public String getStreet() {
        return street;
    }

    public void setStreet(String street) {
        this.street = street;
    }

    public int getFloor() {
        return floor;
    }

    public void setFloor(int floor) {
        this.floor = floor;
    }

    public int getApartment() {
        return apartment;
    }

    public void setApartment(int apartment) {
        this.apartment = apartment;
    }
}

// Class that is used to manage the data form the mysql server.
class Data implements Iterable<Address> {
    private static final String DB_URL = "jdbc:mysql://localhost:3306/AddressBook";
    private static final String DB_USER = "root";
    private static final String DB_PASSWORD = "Simone2006";


    private final ArrayList<Address> addresses = new ArrayList<>();
    private final int userId;

    // Static method that is used for trying to log in a person. It returns the id of the found person and if the person is not find it returns 0.
    public static int login(String email, String password) {
        int id = -1;
        try (Connection connection = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD); PreparedStatement preparedStatement = connection.prepareStatement("SELECT id FROM Users WHERE email = ? AND password = ?")) {

            preparedStatement.setString(1, email);
            preparedStatement.setString(2, password);

            try (ResultSet resultSet = preparedStatement.executeQuery()) {
                while (resultSet.next()) {
                    id = resultSet.getInt("id");
                }
            }

        } catch (SQLException e) {
            System.err.println("There was an error in the login function in data class!" + e);
            System.exit(1);
        }

        return id;
    }

    // Static method that is used to register a person. It checks if the email is the database and return true or false if it fails to add the new account.
    public static boolean register(String email, String password) {
        try (Connection connection = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD); PreparedStatement countUserStatement = connection.prepareStatement("SELECT COUNT(id) FROM Users WHERE email = ?"); PreparedStatement insertUserStatement = connection.prepareStatement("INSERT INTO Users(email,password) VALUES (?,?)")) {

            countUserStatement.setString(1, email);

            try (ResultSet resultSet = countUserStatement.executeQuery()) {
                while (resultSet.next()) {
                    if (resultSet.getInt("COUNT(id)") == 1) {
                        return false;
                    }
                }
            }

            insertUserStatement.setString(1, email);
            insertUserStatement.setString(2, password);
            insertUserStatement.executeUpdate();

        } catch (SQLException e) {
            System.err.println("There was an error in the register function in data class!" + e);
            System.exit(1);
        }

        return true;
    }

    // Constructor that need the user id and will retrieve the data.
    public Data(int userId) {
        this.userId = userId;
        retrieveData();
    }

    // Method that will retrieve all the addresses of the user.
    private void retrieveData() {
        try (Connection connection = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD); PreparedStatement preparedStatement = connection.prepareStatement("SELECT id,name,phone,email,city,neighbourhood,apartment_building,street,floor,apartment FROM Addresses WHERE user_id = ?")) {


            preparedStatement.setInt(1, userId);

            try (ResultSet resultSet = preparedStatement.executeQuery()) {
                while (resultSet.next()) {
                    int id = resultSet.getInt("id");
                    String name = resultSet.getString("name");
                    String phone = resultSet.getString("phone");
                    String email = resultSet.getString("email");
                    String city = resultSet.getString("city");
                    String neighbourhood = resultSet.getString("neighbourhood");
                    String apartmentBuilding = resultSet.getString("apartment_building");
                    String street = resultSet.getString("street");
                    int floor = resultSet.getInt("floor");
                    int apartment = resultSet.getInt("apartment");

                    addresses.add(new Address(id, name, phone, email, city, neighbourhood, apartmentBuilding, street, floor, apartment));
                }
            }


        } catch (SQLException e) {
            System.err.println("There was an error in function retrieve data in data class!" + e);
            System.exit(1);
        }
    }

    // Method that will save the data back to the sql server.
    public void saveData() {
        try (Connection connection = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD); PreparedStatement preparedStatement = connection.prepareStatement("UPDATE Addresses SET name = ?,phone = ?,email=?,city=?,neighbourhood=?,apartment_building=?,street=?,floor=?,apartment=? WHERE id = ?;")) {
            for (Address address : addresses) {
                preparedStatement.setString(1, address.getName());
                preparedStatement.setString(2, address.getPhone());
                preparedStatement.setString(3, address.getEmail());
                preparedStatement.setString(4, address.getEmail());
                preparedStatement.setString(5, address.getNeighbourhood());
                preparedStatement.setString(6, address.getApartmentBuilding());
                preparedStatement.setString(7, address.getStreet());
                preparedStatement.setInt(8, address.getFloor());
                preparedStatement.setInt(9, address.getApartment());
                preparedStatement.setInt(10, address.getId());
                preparedStatement.executeUpdate();
            }
        } catch (SQLException e) {
            System.err.println("There was an error in function save data in data class!" + e);
            System.exit(1);
        }
    }

    // Method that will delete an address form the list and form the sql server.
    public void deleteAddress(int id) {
        for (Address address : addresses) {
            if (address.getId() == id) {
                addresses.remove(address);
                break;
            }
        }

        try (Connection connection = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD); PreparedStatement preparedStatement = connection.prepareStatement("DELETE FROM Addresses WHERE id = ?")) {
            preparedStatement.setInt(1, id);
            preparedStatement.executeUpdate();

        } catch (SQLException e) {
            System.err.println("There was an error in the delete address function class data!" + e);
        }
    }

    // Method that will add a new address to the list and to the sql server.
    public void addAddress(String name, String phone, String email, String city, String neighbourhood, String apartmentBuilding, String street, int floor, int apartment) {
        int id = -1;
        try (Connection connection = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD);
             PreparedStatement preparedStatement = connection.prepareStatement("INSERT INTO Addresses(name, phone, email, city, neighbourhood, apartment_building, street, floor, apartment, user_id) VALUES (?,?,?,?,?,?,?,?,?,?)", Statement.RETURN_GENERATED_KEYS)) {
            preparedStatement.setString(1, name);
            preparedStatement.setString(2, phone);
            preparedStatement.setString(3, email);
            preparedStatement.setString(4, city);
            preparedStatement.setString(5, neighbourhood);
            preparedStatement.setString(6, apartmentBuilding);
            preparedStatement.setString(7, street);
            preparedStatement.setInt(8, floor);
            preparedStatement.setInt(9, apartment);
            preparedStatement.setInt(10, userId);
            preparedStatement.executeUpdate();

            try (ResultSet resultSet = preparedStatement.getGeneratedKeys()) {
                while (resultSet.next()) {
                    id = resultSet.getInt(1);
                }
            }

        } catch (SQLException e) {
            System.err.println("There was an error in the function add address in class data!" + e);
            System.exit(1);
        }

        if (id != -1) {
            addresses.add(new Address(id, name, phone, email, city, neighbourhood, apartmentBuilding, street, floor, apartment));
        }
    }

    // Overridden method used to iterate over all the addresses.
    @Override
    public Iterator<Address> iterator() {
        return addresses.iterator();
    }
}

class AddAddressWindow extends JFrame {
    private final MainWindow mainWindow;
    private final Data data;
    private final JTextField nameTextField = new JTextField();
    private final JTextField phoneTextField = new JTextField();
    private final JTextField emailTextField = new JTextField();
    private final JTextField cityTextField = new JTextField();
    private final JTextField neighbourhoodTextField = new JTextField();
    private final JTextField apartmentBuildingTextField = new JTextField();
    private final JTextField streetTextField = new JTextField();
    private final JTextField floorTextField = new JTextField();
    private final JTextField apartmentTextField = new JTextField();


    public AddAddressWindow(MainWindow mainWindow, Data data) {
        super();

        // Setting attributes to the window.
        this.setTitle("Add new address");
        this.setSize(500, 500);
        this.setLayout(new GridBagLayout());

        // Passing the arguments.
        this.mainWindow = mainWindow;
        this.data = data;

        // Creating the widgets.
        GridBagConstraints layout = new GridBagConstraints();
        JButton addAddressButton = new JButton("Add address");

        // Setting attributes and connecting the widgets
        nameTextField.setPreferredSize(new Dimension(250, 25));
        phoneTextField.setPreferredSize(new Dimension(250, 25));
        emailTextField.setPreferredSize(new Dimension(250, 25));
        cityTextField.setPreferredSize(new Dimension(250, 25));
        neighbourhoodTextField.setPreferredSize(new Dimension(250, 25));
        apartmentBuildingTextField.setPreferredSize(new Dimension(250, 25));
        streetTextField.setPreferredSize(new Dimension(250, 25));
        floorTextField.setPreferredSize(new Dimension(250, 25));
        apartmentTextField.setPreferredSize(new Dimension(250, 25));

        addAddressButton.addActionListener(e -> onAddAddressButtonClicked());

        // Adding the widgets.
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(new JLabel("Name:"), layout);

        layout.gridx = 1;
        this.add(nameTextField, layout);

        layout.gridx = 0;
        layout.gridy = 1;
        this.add(new JLabel("Phone:"), layout);

        layout.gridx = 1;
        this.add(phoneTextField, layout);

        layout.gridx = 0;
        layout.gridy = 2;
        this.add(new JLabel("Email:"), layout);

        layout.gridx = 1;
        this.add(emailTextField, layout);

        layout.gridx = 0;
        layout.gridy = 3;
        this.add(new JLabel("City:"), layout);

        layout.gridx = 1;
        this.add(cityTextField, layout);

        layout.gridx = 0;
        layout.gridy = 4;
        this.add(new JLabel("Neighbourhood:"), layout);

        layout.gridx = 1;
        this.add(neighbourhoodTextField, layout);

        layout.gridx = 0;
        layout.gridy = 5;
        this.add(new JLabel("Apartment building:"), layout);

        layout.gridx = 1;
        this.add(apartmentBuildingTextField, layout);

        layout.gridx = 0;
        layout.gridy = 6;
        this.add(new JLabel("Street:"), layout);

        layout.gridx = 1;
        this.add(streetTextField, layout);

        layout.gridx = 0;
        layout.gridy = 7;
        this.add(new JLabel("Floor:"), layout);

        layout.gridx = 1;
        this.add(floorTextField, layout);

        layout.gridx = 0;
        layout.gridy = 8;
        this.add(new JLabel("Apartment:"), layout);

        layout.gridx = 1;
        this.add(apartmentTextField, layout);

        layout.gridx = 0;
        layout.gridy = 9;
        this.add(addAddressButton, layout);

        this.setVisible(true);
    }

    private void onAddAddressButtonClicked() {
        try {
            Integer.parseInt(apartmentTextField.getText());
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "The value in apartment field should be a number!");
            return;
        }

        try {
            Integer.parseInt(floorTextField.getText());
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "The value in the floor text field should be a number!");
            return;
        }
        data.addAddress(nameTextField.getText(), phoneTextField.getText(), emailTextField.getText(), cityTextField.getText(), neighbourhoodTextField.getText(), apartmentBuildingTextField.getText(), streetTextField.getText(), Integer.parseInt(floorTextField.getText()), Integer.parseInt(apartmentTextField.getText()));
        mainWindow.onSearch();
        this.dispose();
    }
}

class AddressInfoWindow extends JFrame {
    private final Address address;
    private final Data data;
    private final MainWindow mainWindow;
    private final JTextField nameTextField = new JTextField();
    private final JTextField phoneTextField = new JTextField();
    private final JTextField emailTextField = new JTextField();
    private final JTextField cityTextField = new JTextField();
    private final JTextField neighbourhoodTextField = new JTextField();
    private final JTextField apartmentBuildingTextField = new JTextField();
    private final JTextField streetTextField = new JTextField();
    private final JTextField floorTextField = new JTextField();
    private final JTextField apartmentTextField = new JTextField();

    public AddressInfoWindow(MainWindow mainWindow, Data data, Address address) {
        super();

        // Setting attributes to the window.
        this.setTitle("Address indo");
        this.setSize(500, 500);
        this.setLayout(new GridBagLayout());

        // Passing the arguments.
        this.address = address;
        this.data = data;
        this.mainWindow = mainWindow;

        // Creating the widgets.
        GridBagConstraints layout = new GridBagConstraints();
        JButton saveButton = new JButton("Save");
        JButton deleteButton = new JButton("Delete");

        // Setting attributes and connecting the widgets.
        nameTextField.setPreferredSize(new Dimension(250, 25));
        nameTextField.setText(address.getName());

        phoneTextField.setPreferredSize(new Dimension(250, 25));
        phoneTextField.setText(address.getPhone());

        emailTextField.setPreferredSize(new Dimension(250, 25));
        emailTextField.setText(address.getEmail());

        cityTextField.setPreferredSize(new Dimension(250, 25));
        cityTextField.setText(address.getCity());

        neighbourhoodTextField.setPreferredSize(new Dimension(250, 25));
        neighbourhoodTextField.setText(address.getNeighbourhood());

        apartmentBuildingTextField.setPreferredSize(new Dimension(250, 25));
        apartmentBuildingTextField.setText(address.getApartmentBuilding());

        streetTextField.setPreferredSize(new Dimension(250, 25));
        streetTextField.setText(address.getStreet());

        floorTextField.setPreferredSize(new Dimension(250, 25));
        floorTextField.setText(Integer.toString(address.getFloor()));

        apartmentTextField.setPreferredSize(new Dimension(250, 25));
        apartmentTextField.setText(Integer.toString(address.getApartment()));

        saveButton.addActionListener(e -> AddressInfoWindow.this.onSaveButtonClicked());
        deleteButton.addActionListener(e -> AddressInfoWindow.this.onDeleteButtonClicked());

        // Adding the widgets.
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(new JLabel("Name:"), layout);

        layout.gridx = 1;
        this.add(nameTextField, layout);

        layout.gridx = 0;
        layout.gridy = 1;
        this.add(new JLabel("Phone:"), layout);

        layout.gridx = 1;
        this.add(phoneTextField, layout);

        layout.gridx = 0;
        layout.gridy = 2;
        this.add(new JLabel("Email:"), layout);

        layout.gridx = 1;
        this.add(emailTextField, layout);

        layout.gridx = 0;
        layout.gridy = 3;
        this.add(new JLabel("City:"), layout);

        layout.gridx = 1;
        this.add(cityTextField, layout);

        layout.gridx = 0;
        layout.gridy = 4;
        this.add(new JLabel("Neighbourhood:"), layout);

        layout.gridx = 1;
        this.add(neighbourhoodTextField, layout);

        layout.gridx = 0;
        layout.gridy = 5;
        this.add(new JLabel("Apartment building:"), layout);

        layout.gridx = 1;
        this.add(apartmentBuildingTextField, layout);

        layout.gridx = 0;
        layout.gridy = 6;
        this.add(new JLabel("Street:"), layout);

        layout.gridx = 1;
        this.add(streetTextField, layout);

        layout.gridx = 0;
        layout.gridy = 7;
        this.add(new JLabel("Floor:"), layout);

        layout.gridx = 1;
        this.add(floorTextField, layout);

        layout.gridx = 0;
        layout.gridy = 8;
        this.add(new JLabel("Apartment:"), layout);

        layout.gridx = 1;
        this.add(apartmentTextField, layout);

        layout.gridx = 0;
        layout.gridy = 9;
        this.add(saveButton, layout);

        layout.gridy = 10;
        this.add(deleteButton, layout);

        this.setVisible(true);
    }

    private void onSaveButtonClicked() {
        try {
            address.setApartment(Integer.parseInt(apartmentTextField.getText()));
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "The value in apartment should a number");
            return;
        }
        try {
            address.setFloor(Integer.parseInt(floorTextField.getText()));
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "The value in floor should be a number");
            return;
        }

        address.setName(nameTextField.getText());
        address.setPhone(phoneTextField.getText());
        address.setEmail(emailTextField.getText());
        address.setCity(cityTextField.getText());
        address.setNeighbourhood(neighbourhoodTextField.getText());
        address.setApartmentBuilding(apartmentBuildingTextField.getText());
        address.setStreet(streetTextField.getText());

        mainWindow.onSearch();
        this.dispose();
    }

    private void onDeleteButtonClicked() {
        data.deleteAddress(address.getId());
        mainWindow.onSearch();
        this.dispose();
    }
}

class MainWindow extends JFrame {
    private final JTextField searchBar = new JTextField();
    private final JPanel resultsPanel = new JPanel();
    private final LoginWindow loginWindow;
    private final Data data;

    public MainWindow(LoginWindow loginWindow, int userId) {
        super();

        // Setting attributes to the window.
        this.setTitle("Address book");
        this.setSize(500, 500);
        this.setLayout(new GridBagLayout());
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                MainWindow.this.onWindowClosing();
            }
        });

        // Passing the arguments.
        this.loginWindow = loginWindow;

        // Creating the class used for managing the data
        data = new Data(userId);

        // Creating the widgets.
        GridBagConstraints layout = new GridBagConstraints();
        JScrollPane resultsScrollPane = new JScrollPane(resultsPanel);
        JButton addAddressButton = new JButton("Add address");

        // Setting attributes and connecting the widgets.
        searchBar.addActionListener(e -> MainWindow.this.onSearch());
        searchBar.setPreferredSize(new Dimension(250, 25));

        resultsPanel.setLayout(new GridBagLayout());

        resultsScrollPane.setPreferredSize(new Dimension(300, 300));
        resultsScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        resultsScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);

        addAddressButton.addActionListener(e -> new AddAddressWindow(MainWindow.this, data));

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(searchBar, layout);

        layout.gridy = 1;
        this.add(searchBar, layout);

        layout.gridy = 2;
        this.add(resultsScrollPane, layout);

        layout.gridy = 3;
        this.add(addAddressButton, layout);

        this.setVisible(true);
    }

    public void onSearch() {
        resultsPanel.removeAll();

        String text = searchBar.getText().strip().toLowerCase();
        GridBagConstraints layout = new GridBagConstraints();
        layout.gridy = 0;
        layout.gridx = 0;
        layout.weightx = 1;
        layout.fill = GridBagConstraints.HORIZONTAL;

        for (Address address : data) {
            if (address.getName().contains(text)) {
                JButton result = new JButton(address.getName());
                result.addActionListener(e -> new AddressInfoWindow(this, data, address));

                resultsPanel.add(result, layout);
                layout.gridy++;
            }
        }

        resultsPanel.revalidate();
        resultsPanel.repaint();
    }

    private void onWindowClosing() {
        loginWindow.mainWindowClosed();
        data.saveData();
        this.dispose();
    }

}

class LoginWindow extends JFrame {
    private final JTextField emailTextField = new JTextField();
    private final JPasswordField passwordField = new JPasswordField();

    public LoginWindow() {
        super();

        // Setting attributes to the window.
        this.setTitle("Address book");
        this.setSize(350, 350);
        this.setLayout(new GridBagLayout());
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Creating the widgets.
        GridBagConstraints layout = new GridBagConstraints();
        JButton loginButton = new JButton("Login");
        JButton registerButton = new JButton("Register");

        // Setting attributes and connecting the widgets.
        emailTextField.setPreferredSize(new Dimension(250, 25));
        passwordField.setPreferredSize(new Dimension(250, 25));
        passwordField.setEchoChar('*');

        loginButton.addActionListener(e -> LoginWindow.this.onLoginButtonClicked());
        registerButton.addActionListener(e -> LoginWindow.this.onRegisterButtonClicked());

        // Adding thew widgets.
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(new JLabel("Email:"), layout);

        layout.gridx = 1;
        this.add(emailTextField, layout);

        layout.gridx = 0;
        layout.gridy = 1;
        this.add(new JLabel("Password"), layout);

        layout.gridx = 1;
        this.add(passwordField, layout);

        layout.gridx = 0;
        layout.gridy = 2;
        this.add(loginButton, layout);

        layout.gridx = 1;
        this.add(registerButton, layout);
    }

    private void onLoginButtonClicked() {
        int userId = Data.login(emailTextField.getText(), new String(passwordField.getPassword()));
        if (userId == -1) {
            JOptionPane.showMessageDialog(this, "Your email or password is incorrect!");
        } else {
            new MainWindow(this, userId);
            this.setVisible(false);
        }
    }

    private void onRegisterButtonClicked() {
        if (emailTextField.getText().isEmpty() || passwordField.getPassword().length == 0) {
            JOptionPane.showMessageDialog(this, "The email or the passwords can't be empty!");
            return;
        }
        if (Data.register(emailTextField.getText(), new String(passwordField.getPassword()))) {
            JOptionPane.showMessageDialog(this, "You are now registered!");
        } else {
            JOptionPane.showMessageDialog(this, "You are not registered! The email is already in use.");
        }
    }

    public void mainWindowClosed() {
        emailTextField.setText("");
        passwordField.setText("");
        this.setVisible(true);
    }
}

public class Main {
    public static void main(String[] args) {
        LoginWindow loginWindow = new LoginWindow();
        loginWindow.setVisible(true);
    }
}