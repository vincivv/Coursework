# datatypes.py
#
# This file defines the data types used in the ORM framework. Each class represents
# a specific column type that can be used in the database schema. These types
# are then used by the ORM's base class to define columns in database tables.
#
# The `Column` class in the ORM will reference these data types to configure the
# column's behavior, such as validating values, determining the SQL representation,
# and enforcing constraints.
#
# For example,
#   - The `Integer` class represents an INTEGER column type in SQL.
#   - The `String` class represents a TEXT column type with an optional length constraint.
#   - The `Boolean` class represents a BOOLEAN column type.
#
# Students should implement the missing methods for each type (e.g., `get_sql`, `validate`)
# to ensure proper integration with the ORM and the generation of valid SQL queries.
#
# Example usage in the ORM base class:
#
#   class User:
#       id = Column(Integer, primary_key=True)   # An INTEGER primary key column
#       name = Column(String(255), nullable=False)  # A TEXT column with a length of 255, not nullable
#       is_active = Column(Boolean, default=True)  # A BOOLEAN column with a default value of True
#       created_at = Column(Date)  # A DATE column
#
#   This would represent a "User" table with columns: "id", "name", "is_active", and "created_at".
#   The ORM will use the data types (Integer, String, Boolean, Date) to validate values and generate
#   SQL queries when interacting with the database.

class Integer:
    def __init__(self, type="INTEGER", length=None):
        self.type = type
        self.length = length

    # TODO: Implement a method to validate if the integer type is within valid ranges (e.g., for different database systems)
    def validate(self):
        pass

    # TODO: Implement a method to get the SQL representation of the integer type (e.g., "INTEGER", "TINYINT", ...)
    def to_sql(self):
        pass


class String:
    def __init__(self, type='TEXT', length=None):
        self.type = type
        self.length = length

    # TODO: Implement a method to validate if the length is a positive integer and within valid database constraints
    def validate_length(self):
        pass

    # TODO: Implement a method to get the SQL representation of the string type, including length if provided (e.g., "TEXT(255), VARCHAR(255), CHAR(2) ...")
    def to_sql(self):
        pass

class Float:
    def __init__(self, type='FLOAT'):
        self.type = type

    # TODO: Implement a method to validate if the float type fits the allowed precision for the database (e.g., not exceeding maximum limits)
    def validate(self):
        pass

    # TODO: Implement a method to get the SQL representation of the float type (e.g., "REAL")
    def to_sql(self):
        pass

class Boolean:
    def __init__(self):
        self.type = "BOOLEAN"

    # TODO: Implement a method to validate if the boolean type is correctly represented (should only allow True, False, or NULL)
    def validate(self):
        pass

    # TODO: Implement a method to get the SQL representation of the boolean type (e.g., "BOOLEAN")
    def to_sql(self):
        pass

class Date:
    def __init__(self, type='DATE'):
        self.type = type

    # TODO: Implement a method to validate if the date type is correctly formatted (e.g., for DATE: "YYYY-MM-DD")
    def validate(self):
        pass

    # TODO: Implement a method to get the SQL representation of the date type (e.g., "DATE", "TIMESTAMP", "DATETIME"...)
    def to_sql(self):
        pass

class Blob:
    def __init__(self):
        self.type = "BLOB"

    # TODO: Implement a method to validate if the BLOB type is appropriately used for binary data storage
    def validate(self):
        pass

    # TODO: Implement a method to get the SQL representation of the BLOB type (e.g., "BLOB")
    def to_sql(self):
        pass