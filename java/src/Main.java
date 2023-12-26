import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.Iterator;

// class used for representing information about a person/contact
class Person {
    private String name;
    private String address;
    private String phone;
    private String email;
    private int id;

    public Person(String name, String address, String phone, String email, int id) {
        this.name = name;
        this.address = address;
        this.phone = phone;
        this.email = email;
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
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

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }
}

// class that will hold and manage the data
class Data implements Iterable<Person> {
    // list that will hold all the persons
    private ArrayList<Person> persons;

    public Data() {
        readData();
    }

    // method that will parse the json file
    private void readData() {
        try (FileReader file = new FileReader("/Users/simonesamardzhiev/Desktop/My projects/Address Book/java/data.json")) {
            Gson gson = new Gson();
            Type type = new TypeToken<ArrayList<Person>>() {
            }.getType();
            persons = gson.fromJson(file, type);
        } catch (IOException e) {
            System.err.println("There was an error reading the file");
            System.exit(1);
        }
    }

    // method that write the data back into the json file
    public void writeData() {
        revalidateIds();
        try (FileWriter file = new FileWriter("/Users/simonesamardzhiev/Desktop/My projects/Address Book/java/data.json")) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            gson.toJson(persons, file);
        } catch (IOException e) {
            System.err.println("There was an error in writing to the file");
            System.exit(1);
        }
    }

    // method that will update the ids before writing the data back to the json
    private void revalidateIds() {
        for (int i = 0; i < persons.size(); i++) {
            persons.get(i).setId(i);
        }
    }

    // method that will add a new person
    public void addPerson(String name, String address, String phone, String email) {
        persons.add(new Person(name, address, phone, email, getNewId()));
    }

    // method that will return a new unique id
    private int getNewId() {
        if (persons.isEmpty()) {
            return 0;
        } else {
            return persons.getLast().getId() + 1;
        }
    }

    // method that will delete a person based on the id
    public void deletePerson(int id) {
        for (Person person : persons) {
            if (person.getId() == id) {
                persons.remove(person);
                break;
            }
        }
    }

    // method that will change the information about a person using its id
    void changePerson(String name, String address, String phone, String email, int id) {
        for (Person person : persons) {
            if (person.getId() == id) {
                person.setName(name);
                person.setAddress(address);
                person.setPhone(phone);
                person.setEmail(email);
                break;
            }
        }
    }

    // method that allows to iterate over the persons
    @Override
    public Iterator<Person> iterator() {
        return persons.iterator();
    }

}

// class that will display a window for adding a new person
class NewPersonWindow extends JFrame {
    // necessary global widgets and variables
    private final Data data;
    private final Window window;

    private final TextField nameTextField;
    private final TextField addressTextField;
    private final TextField phoneTextField;
    private final TextField emailTextField;

    public NewPersonWindow(Data data, Window window) {
        super();

        // setting attributes to the window
        this.setTitle("New person");
        this.setSize(400, 400);
        this.setLayout(new GridBagLayout());

        // passing the arguments
        this.data = data;
        this.window = window;

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        nameTextField = new TextField();
        addressTextField = new TextField();
        phoneTextField = new TextField();
        emailTextField = new TextField();
        JButton addButton = new JButton("Add");

        // setting the attributes of the widgets and connecting the buttons
        nameTextField.setPreferredSize(new Dimension(300, 25));
        addressTextField.setPreferredSize(new Dimension(300, 25));
        phoneTextField.setPreferredSize(new Dimension(300, 25));
        emailTextField.setPreferredSize(new Dimension(300, 25));
        addButton.addActionListener(actionEvent -> NewPersonWindow.this.onAddClicked());

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(new JLabel("Name :"), layout);

        layout.gridx = 1;
        this.add(nameTextField, layout);

        layout.gridx = 0;
        layout.gridy = 1;
        this.add(new JLabel("Address :"), layout);

        layout.gridx = 1;
        this.add(addressTextField, layout);

        layout.gridx = 0;
        layout.gridy = 2;
        this.add(new JLabel("Phone :"), layout);

        layout.gridx = 1;
        this.add(phoneTextField, layout);

        layout.gridx = 0;
        layout.gridy = 3;
        this.add(new JLabel("Email :"), layout);

        layout.gridx = 1;
        this.add(emailTextField, layout);

        layout.gridx = 0;
        layout.gridy = 4;
        this.add(addButton, layout);

        this.setVisible(true);
    }

    // method that is called when the add button is clicked. It will get the text in the Text fields and create a new person
    void onAddClicked() {
        data.addPerson(nameTextField.getText(), addressTextField.getText(), phoneTextField.getText(), emailTextField.getText());
        window.onSearch();
        this.dispose();
    }
}

// class that will display information about a person and other functionality
class PersonWindow extends JFrame {
    // necessary global variables
    private final int id;
    private final Data data;
    private final Window window;

    private final TextField nameTextField;
    private final TextField addressTextField;
    private final TextField phoneTextField;
    private final TextField emailTextField;

    public PersonWindow(Person person, Data data, Window window) {
        super();

        // setting attributes to the window
        this.setTitle(person.getName());
        this.setSize(400, 400);
        this.setLayout(new GridBagLayout());

        // passing the arguments
        id = person.getId();
        this.data = data;
        this.window = window;

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        nameTextField = new TextField();
        addressTextField = new TextField();
        phoneTextField = new TextField();
        emailTextField = new TextField();
        JButton saveButton = new JButton("Save");
        JButton deleteButton = new JButton("Delete");

        // setting the attributes of the widgets and connecting the buttons
        nameTextField.setPreferredSize(new Dimension(300, 25));
        nameTextField.setText(person.getName());
        addressTextField.setPreferredSize(new Dimension(300, 25));
        addressTextField.setText(person.getAddress());
        phoneTextField.setPreferredSize(new Dimension(300, 25));
        phoneTextField.setText(person.getPhone());
        emailTextField.setPreferredSize(new Dimension(300, 25));
        emailTextField.setText(person.getEmail());
        saveButton.addActionListener(actionEvent -> PersonWindow.this.onSaveClicked());
        deleteButton.addActionListener(actionEvent -> PersonWindow.this.onDeleteClicked());

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(new JLabel("Name :"), layout);

        layout.gridx = 1;
        this.add(nameTextField, layout);

        layout.gridx = 0;
        layout.gridy = 1;
        this.add(new JLabel("Address :"), layout);

        layout.gridx = 1;
        this.add(addressTextField, layout);

        layout.gridx = 0;
        layout.gridy = 2;
        this.add(new JLabel("Phone :"), layout);

        layout.gridx = 1;
        this.add(phoneTextField, layout);

        layout.gridx = 0;
        layout.gridy = 3;
        this.add(new JLabel("Email :"), layout);

        layout.gridx = 1;
        this.add(emailTextField, layout);

        layout.gridx = 0;
        layout.gridy = 4;
        this.add(saveButton, layout);

        layout.gridy = 5;
        this.add(deleteButton, layout);

        this.setVisible(true);
    }

    // method that is called when the save button is clicked. It will update the person new information
    void onSaveClicked() {
        data.changePerson(nameTextField.getText(), addressTextField.getText(), phoneTextField.getText(), emailTextField.getText(), id);
        window.onSearch();
        this.dispose();
    }

    // method that will be called when the delete button is clicked. It will delete the person
    void onDeleteClicked() {
        data.deletePerson(id);
        window.onSearch();
        this.dispose();
    }

}

// class that will display the main window
class Window extends JFrame {
    // necessary global variables
    private final Data data = new Data();
    private final JTextField searchBar;
    private final ArrayList<JButton> results = new ArrayList<>();

    public Window() {
        super();

        // setting attributes to the window;
        this.setTitle("Address book");
        this.setSize(500, 500);
        this.setLayout(new GridBagLayout());
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                data.writeData();
                System.exit(0);
            }
        });

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        JButton buttonAdd = new JButton("Add");
        searchBar = new JTextField();

        // setting attributes of the widgets and connecting the buttons
        buttonAdd.addActionListener(actionEvent -> Window.this.onAddClicked());
        searchBar.addActionListener(actionEvent -> Window.this.onSearch());
        searchBar.setPreferredSize(new Dimension(150, 25));

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(buttonAdd, layout);

        layout.gridy = 1;
        this.add(searchBar, layout);
    }

    // method that is called when the add button is clicked. It will display the window responsible for adding a new person
    void onAddClicked() {
        new NewPersonWindow(data, this);
    }

    // method that is called when the user pres return/enter at the search bar. It will show all the found persons
    void onSearch() {
        for (JButton result : results) {
            this.getContentPane().remove(result);
        }
        results.clear();

        GridBagConstraints layout = new GridBagConstraints();
        layout.gridx = 0;
        layout.gridy = 2;
        String text = searchBar.getText();

        for (Person person : data) {
            if (person.getName().startsWith(text)) {
                JButton result = new JButton(person.getName());
                results.add(result);

                result.addActionListener(actionEvent -> Window.this.onResultClicked(person));

                this.add(result, layout);
                layout.gridy++;
            }
        }
        this.repaint();
        this.revalidate();
    }

    // method that is called when result is clicked. It will display a window with information and others functionality
    void onResultClicked(Person person) {
        new PersonWindow(person, data, this);
    }
}

// main method 
public class Main {
    public static void main(String[] args) {
        Window window = new Window();
        window.setVisible(true);
    }
}