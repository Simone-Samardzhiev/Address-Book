import sys

import mysql.connector


# Class that will represent the address information of a person.
class Address:
    def __init__(self, _id: int, name: str, phone: str, email: str, city: str, neighbourhood: str,
                 apartment_building: str,
                 street: str, floor: int, apartment: int) -> None:
        self.id = _id
        self.name = name
        self.phone = phone
        self.email = email
        self.city = city
        self.neighbourhood = neighbourhood
        self.apartment_building = apartment_building
        self.street = street
        self.floor = floor
        self.apartment = apartment


# Class used to manage the data from the database.
class Data:
    # Method that will check if the user exists in a database. If it exists, it returns the user id else it returns
    # none.
    @staticmethod
    def login(email: str, password: str) -> int | None:

        try:
            with mysql.connector.connect(user='root', password='Simone2006', host='localhost',
                                         database='AddressBook') as connection:
                cursor = connection.cursor()

                query = "SELECT id FROM Users WHERE email = %s AND password = %s"
                cursor.execute(query, (email, password))

                user = cursor.fetchone()
                cursor.close()
                if user is not None:
                    return user[0]
                else:
                    return None
        except mysql.connector.Error as err:
            print("There was an error in connecting to the database in the login function on class Data .", err)
            sys.exit(1)

    # Method that will register a new user to the database. If the email exists, it will return false else it will
    # return true
    @staticmethod
    def add_new_user(email: str, password: str) -> bool:

        try:
            with mysql.connector.connect(user='root', password='Simone2006', host='localhost',
                                         database='AddressBook') as connection:
                cursor = connection.cursor()

                query = "SELECT COUNT(*) FROM Users WHERE email = %s"
                cursor.execute(query, (email,))

                if cursor.fetchone()[0] == 0:
                    query = "INSERT INTO Users (email, password) VALUES (%s,%s)"
                    cursor.execute(query, (email, password))
                    connection.commit()
                    cursor.close()
                    return True
                else:
                    cursor.close()
                    return False

        except mysql.connector.Error as err:
            print("There was an error in connecting to the database in the add new user function in class Data .", err)
            sys.exit(1)

    # Constructor that will get all the addresses of a user.
    def __init__(self, user_id: int) -> None:
        self.user_id = user_id
        self.addresses: list[Address] = []
        self.retrieve_data()

    # Method that will retrieve all the addresses forms the server with the user id.
    def retrieve_data(self) -> None:
        try:
            with mysql.connector.connect(user='root', password='Simone2006', host='localhost',
                                         database='AddressBook') as connection:
                cursor = connection.cursor()
                query = (
                    "SELECT * FROM Addresses WHERE Addresses.user_id = %s")
                cursor.execute(query, (self.user_id,))

                data = cursor.fetchall()
                for row in data:
                    address = Address(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9])
                    self.addresses.append(address)

                cursor.close()
        except mysql.connector.Error as err:
            print("There was an error in connecting to the database in the retrieve data function in class Data .", err)
            sys.exit(1)

    # Method that will update the current data. It will update all the addresses that have the same id as the ones
    # that the retrieve data have gotten
    def save_data(self) -> None:
        try:
            with mysql.connector.connect(user='root', password='Simone2006', host='localhost',
                                         database='AddressBook') as connection:
                cursor = connection.cursor()

                for address in self.addresses:
                    query = ("UPDATE Addresses SET name = %s ,phone = %s, email = %s, city = %s, neighbourhood =%s, "
                             "apartment_building = %s, street = %s, floor = %s, apartment =%s WHERE id =%s")
                    cursor.execute(query, (
                        address.name, address.phone, address.email, address.city, address.neighbourhood,
                        address.apartment_building, address.street, address.floor, address.apartment, address.id))

                connection.commit()
                cursor.close()

        except mysql.connector.Error as err:
            print("There was an error in connecting to the database in the save data function in class Data .", err)
            sys.exit(1)

    # Method that will add a new address to a user. It inserts it into the server that gets the id and inserts it into
    # the list that keeps the addresses
    def add_address(self, name: str, phone: str, email: str, city: str, neighbourhood: str, apartment_building: str,
                    street: str,
                    floor: int, apartment: int) -> None:
        try:
            with mysql.connector.connect(user='root', password='Simone2006', host='localhost',
                                         database='AddressBook') as connection:
                cursor = connection.cursor()
                query = (
                    "INSERT INTO Addresses (name,phone, email, city,neighbourhood, apartment_building,street,floor,"
                    "apartment,user_id) VALUES (%s,%s, %s, %s, %s, %s,%s,%s,%s,%s)")
                cursor.execute(query,
                               (name, phone, email, city, neighbourhood, apartment_building, street, floor, apartment,
                                self.user_id))
                connection.commit()

                address = Address(cursor.lastrowid, name, phone, email, city, neighbourhood, apartment_building, street,
                                  floor, apartment)
                self.addresses.append(address)
        except mysql.connector.Error as err:
            print("There was an error in connecting to the database in the add address function in class .", err)
            sys.exit(1)

    # Method that will delete an address based on the id. It will delete it form the server and form the list
    def delete_address(self, _id: int) -> None:
        try:
            with mysql.connector.connect(user='root', password='Simone2006', host='localhost',
                                         database='AddressBook') as connection:
                cursor = connection.cursor()

                query = "DELETE FROM Addresses WHERE id = %s"
                cursor.execute(query, (_id,))
                connection.commit()

                for address in self.addresses:
                    if address.id == _id:
                        self.addresses.remove(address)
                        break
        except mysql.connector.Error as err:
            print("There was an error in connecting to the database in the delete address function in class .", err)
            sys.exit(1)

    # Simple overloaded method that will yield all the recipes in the list.
    def __iter__(self) -> Address:
        for address in self.addresses:
            yield address
